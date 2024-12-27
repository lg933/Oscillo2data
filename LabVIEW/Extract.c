#include <stdio.h>
#include <stdlib.h>

/*
                     AUTEURS
 
 Léo Gallacio
 Alexandre Sampaio
 Fabio Feig
 
*/

/*
                   DESCRIPTION
 
 ÉTAPE 1 : DIMENSIONS (Lignes 20-60)
 1. Lecture du fichier Pixmap.bin en ushort (16 bits) sur 2 bytes
 2. Stockage dans dimension[i] de la profondeur, largeur, hauteur
 
 ÉTAPE 2 : COULEURS (Lignes 61-162)
 3. Lecture du fichier Pixmap.bin en uchar (8 bits) sur les bytes restants
 4. Stockage dans pixmap[i] de la couleur associée au pixel i
 5. Stockage dans frequence[i] de la fréquence de la couleur n°i
 6. Repérage des points de contrôle (frequence[i] == 4)
 7. Repérage des traces (50 ≤ frequence[i] ≤ 300)
 8. Tri des traces dans l’ordre décroissant
 9. Stockage des 5 premières traces dans traces_f[m]
 
 ÉTAPE 3 : COORDONNÉES (Lignes 163-252)
 10. Calcul des coordonnées (repère en haut à gauche)
 11. Transformation vers un repère en bas à gauche
 12. Création du fichier Traces.txt
 13. Écriture des coordonnées des points de contrôle et des traces
 
 */

/*
                  NOMENCLATURE
 
 dimensions[n] = profondeur / largeur / hauteur
 nb_pixels = nombre de pixels dans l'image
 pixel[i] = couleur du pixel n°i
 nb_couleur = nombre de couleurs possibles (256)
 frequence[j] = fréquence de la couleur n°j
 corner_check : vérifie la présence des points de contrôle (erreur si corner_check != 1)
 nb_trace = nombre de traces
 traces_sf[k] = couleur de la trace n°k (sf = sans filtre)
 traces_f[m] = couleur des 5 traces les plus nombreuses (f = filtre)
 coord_x : coordonnées x du pixel i
 coord_y : coordonnées y du pixel i
 
*/

int stockage(const char *pixmap_path, const char *traces_path) {
    
    // ------------------------------------------------------------------------
    // ÉTAPE 1 : DIMENSIONS
    // ------------------------------------------------------------------------
    
    // Ouverture du fichier Pixmap.bin
    FILE *input = fopen(pixmap_path, "rb");
    if (input == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier\n");
        return -1;
    }
    
    // Création du tableau de stockage des dimensions
    unsigned short dimensions[3];
    
    // Lecture des 3 premières valeurs
    size_t dim_read = fread(dimensions, sizeof(unsigned short), 3, input);
    int largeur = dimensions[1]; // détermination de la largeur
    int hauteur = dimensions[2]; // détermination de la hauteur
    if (dim_read != 3) {
        fprintf(stderr, "Erreur: Impossible de lire les dimensions\n");
        fclose(input);
        return -1;
    }

    // Vérification que les dimensions sont positives
    for (int n = 0; n < 3; n++) {
        if (dimensions[n] <= 0) {
            fprintf(stderr, "Erreur: Les dimensions doivent être positives\n");
            fclose(input);
            return -1;
        }
    }
    
    // Vérification que la profondeur est de 8
    if(dimensions[0]!=8){
        fprintf(stderr, "Erreur: La profondeur doit être de 8\n");
        fclose(input);
        return -1;
    }

    // Vérification que largeur et hauteur ∈ [100; 1000]
    if ((dimensions[1] < 100 || dimensions[1] > 1000) || (dimensions[2] < 100 || dimensions[2] > 1000)){
        fprintf(stderr, "Erreur: Largeur et hauteur doivent être comprises entre 100 et 1000\n");
        fclose(input);
        return -1;
    }
    
    // ------------------------------------------------------------------------
    // ÉTAPE 2 : COULEURS
    // ------------------------------------------------------------------------
    
    // Calcul du nombre de pixel dans l'image
    int nb_pixels = largeur * hauteur;

    // Allocation mémoire pour les pixels
    unsigned char *pixel = (unsigned char *)malloc(nb_pixels * sizeof(unsigned char));
    if (pixel == NULL) {
        fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire pour les pixels\n");
        fclose(input);
        return -1;
    }

    // Lecture des pixels du fichier
    size_t pix_read = fread(pixel, sizeof(unsigned char), nb_pixels, input);
    if (pix_read != nb_pixels) {
        fprintf(stderr, "Erreur: Nombre de pixels lus incompatible avec les dimensions\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Création de l'histogramme à 256 entrées
    const int nb_couleur = 256;
    unsigned int *frequence = (unsigned int *)calloc(nb_couleur, sizeof(unsigned int));
    if (frequence == NULL) {
        fprintf(stderr, "Erreur: Impossible d'allouer de la mémoire pour l'histogramme des couleurs\n");
        fclose(input);
        return -1;
    }
    
    // Remplissage de l'histogramme avec les fréquences de chaque couleurs
    for (int i = 0; i < nb_pixels; i++){
        frequence[pixel[i]]++;
    }
    
    // Calcul du nombre de traces
    int nb_trace = 0;
    int corner_check = 0;
    int traces_sf[100]; //sf = sans filtre
    int corners_color = 0;
    for (int j = 0; j < nb_couleur; j++){
        if (frequence[j] == 4) corner_check++;
        if (corner_check == 1 && frequence[j] == 4) corners_color = j; // couleur des corners
        if ((50 <= frequence[j]) && (300 >= frequence[j])){ // couleur des traces
            traces_sf[nb_trace] = j;
            nb_trace++;
        }
    }
    
    // Vérification de l'existence des corners
    if (corner_check == 0) {
        fprintf(stderr, "Erreur: Aucun points de contrôles trouvé\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Vérification de l'existence d'uniquement 4 corners
    if (corner_check > 1) {
        fprintf(stderr, "Erreur: Corners de différentes couleurs trouvés\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Affichage de la couleur des corners
    printf("C: %d\n", corners_color);
    
    // Tri des traces dans l'ordre décroissant
    for (int k0 = 0; k0 < nb_trace; k0++) {
        for (int k1 = k0 + 1; k1 < nb_trace; k1++) {
            if (frequence[traces_sf[k0]] < frequence[traces_sf[k1]]) { // tri par bulles
                int tmp = 0;
                tmp = traces_sf[k0];
                traces_sf[k0] = traces_sf[k1]; // faire remonter la plus grande valeur
                traces_sf[k1] = tmp;
            }
        }
    }
    
    // Vérification de l'existence d'une trace
    if (nb_trace == 0) {
        fprintf(stderr, "Erreur: Aucunne trace trouvée\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Vérification nombre de traces <= 10
    if (nb_trace > 10) {
        fprintf(stderr, "Erreur: Plus de 10 traces trouvées\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Création d'un nouveau tableau de traces pour garder les 5 premiers
    int traces_f[5]; // f = filtre
    for (int m = 0; m < 5; m++){
        traces_f[m]=traces_sf[m];
    }

    // Affichage du nombre de traces avec les couleurs associées
    if (nb_trace > 5){ // cas nb_trace > 5
        printf("T: ");
        for (int m = 0; m < 5; m++){
            printf("%d ", traces_f[m]);
        }
    }
    else if (nb_trace <= 5){ // cas nb_trace <= 5
        printf("T: ");
        for (int m = 0; m < nb_trace; m++){
            printf("%d ", traces_f[m]);
        }
    }
    
    // ------------------------------------------------------------------------
    // ÉTAPE 3 : COORDONNÉES
    // ------------------------------------------------------------------------

    // Affichage des pixels avec leurs coordonnées
    unsigned int coord_x = 0;
    unsigned int coord_y = 0;
        
    // Création du fichier Traces.txt
    FILE *output = fopen(traces_path, "w");
    if (output == NULL) {
        fprintf(stderr, "Erreur: Impossible de créer Traces.txt\n");
        return 1;
    }
    
    // Variable err pour vérifier que fprintf écrit dans Traces.txt
    int err = 0;

    // Ecriture des coordonnées des Corners
    err += (fprintf(output, "Corners= [\n") < 0);
    for (int i = 0; i < nb_pixels; i++){
        coord_x = i % largeur; // calcul de la coordoonée x du pixel i
        coord_y = hauteur - i / largeur; // calcul de la coordoonée y du pixel i
        if ((frequence[pixel[i]] != 0) && (frequence[pixel[i]] == 4)){
            err += (fprintf(output, "%d, %d;\n", coord_x, coord_y) < 0); // écriture et vérification de err
        }
    }
    err += (fprintf(output, "];\n\n") < 0);

    // Ecriture des coordonnées des traces (ordre décroissant)
    if (nb_trace > 5){ // cas nb_trace > 5
        for (int i0 = 0; i0 < 5; i0++){
            err += (fprintf(output, "C%d= [\n", i0) < 0);
            for (int i1 = 0; i1 < nb_pixels; i1++){
                coord_x = i1 % largeur; // calcul de la coordoonée x du pixel i1
                coord_y = hauteur - i1 / largeur; // calcul de la coordoonée y du pixel i1
                if (pixel[i1] == traces_f[i0]){ // couleur du pixel i1 doit être celle de la trace i0
                    err += (fprintf(output, "%d, %d;\n", coord_x, coord_y) < 0);
                }
            }
            err += (fprintf(output, "];\n\n") < 0);
        }
    } else if (nb_trace <= 5){ // cas nb_trace <= 5
        for (int i0 = 0; i0 < nb_trace; i0++){
            err += (fprintf(output, "C%d= [\n", i0) < 0);
            for (int i1 = 0; i1 < nb_pixels; i1++){
                coord_x = i1 % largeur; // calcul de la coordoonée x du pixel i1
                coord_y = hauteur - i1 / largeur; // calcul de la coordoonée y du pixel i1
                if (pixel[i1] == traces_f[i0]){ // couleur du pixel i1 doit être celle de la trace i0
                    err += (fprintf(output, "%d, %d;\n", coord_x, coord_y) < 0);
                }
            }
            err += (fprintf(output, "];\n\n") < 0);
        }
    }

    // Ecriture de T ={C0 C1 ... };
    err += (fprintf(output, "T ={") < 0);
    if (nb_trace > 5){ // cas nb_trace > 5
        for (int a = 0; a < 5; a++){
            err += (fprintf(output, "C%d ", a) < 0);
        }
    } else if (nb_trace <= 5){ // cas nb_trace <= 5
        for (int a = 0; a < nb_trace; a++){
            err += (fprintf(output, "C%d ", a) < 0);
        }
    }
    err += (fprintf(output, "};\n\n") < 0);

    // Vérification s'il y a eu des erreurs lors de l'écriture dans Traces.txt
    if (err > 0) {
        fprintf(stderr, "Erreur lors de l'écriture dans Traces.txt\n");
        fclose(output);
        return -1;
    }

    // Fermeture des fichiers
    fclose(input);
    fclose(output);

    // Libérer la mémoire après utilisation
    free(pixel);
    free(frequence);

    return 0;
}

int main(int argc, char *argv[]){
    // Vérification du nombre d'arguments
    if (argc < 3) {
        fprintf(stderr, "Arguments requis: ./Extract '/chemin/vers/pixmap.bin' '/chemin/vers/traces.txt'\n");
        return -1;
    }
    
    // Appel fonction "stockage" avec les chemins en argument
    return stockage(argv[1], argv[2]);
}
