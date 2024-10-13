#include <stdio.h>
#include <stdlib.h>

int stockage(void) {
    
    /*
     
                                DOCUMENTATION
     
     ---------------
     ÉTAPES
     ---------------
     
     ÉTAPE 1 : DIMENSIONS
     1) Lecture du fichier Pixmap.bin en ushort (16 bits) sur 3 bytes
     2) Stockage dans dimension[i] de la profondeur, largeur, hauteur
     
     ÉTAPE 2 : COULEURS
     4) Lecture du fichier Pixmap.bin en uchar (8bits) sur les bytes restants
     5) Stockage dans pixmap[i] de la couleur associée au pixel i
     6) Stockage dans frequence[i] de la fréquence de la couleur n°i
     7) Repérage des points de contrôle (frequence[i] == 4)
     8) Repérage des traces (50 <= frequence[i] <= 300)
     9) Tri des traces dans l'ordre décroissant
     10) Stockage des 5 premières traces dans traces_f[m]
     
     ÉTAPE 3 : COORDONNÉES
     11) Calcul des coordonnées (repère en haut à gauche) : x = (pixel n°i) / largeur, y = (pixel n°i) % largeur
     12) Transformation vers un repère en bas à gauche : x = (pixel n°i) / largeur, y = hauteur - (pixel n°i) % largeur
     13) Création du fichier Traces.txt
     14) Écriture des points de contrôle et coordonnées
     
     ---------------
     NOMENCLARUTURE
     ---------------
     
     dimensions[n] = profondeur / largeur / hauteur
     nb_pixels = nombre de pixels dans l'image
     pixel[i] = couleur du pixel n°i
     nb_couleur = nombre de couleurs possibles (voir : web-clut.jpg)
     frequence[j] = fréquence de la couleur n°j
     corner_check : vérifie la présence des points de contrôle (erreur si corner_check != 1)
     nb_trace = nombre de traces
     traces_sf[k] = couleur de la trace n°k (sf = sans filtre)
     traces_f[m] = couleur des 5 traces les plus nombreuses (f = filtre)
     coord_x : coordonnées x du pixel i
     coord_y : coordonnées y du pixel i
     
     ---------------
     ERREURS
     ---------------
     
     Erreur: Impossible d'ouvrir le fichier
     Erreur: Impossible de lire les dimensions
     Erreur: Les dimensions doivent être positives
     Erreur: La profondeur doit être égale à 8
     Erreur: Largeur et hauteur doivent être comprises entre 100 et 1000
     Erreur: Impossible d'allouer de la mémoire pour les pixels
     Erreur: Nombre incompatible de pixels lus
     Erreur: Impossible d'allouer de la mémoire pour l'histogramme des couleurs
     Erreur: Aucun points de contrôles trouvés
     Erreur: Aucunne trace trouvée
     Erreur: Impossible de créer Traces.txt
     
     !!! Traces.txt ne se créé pas depuis Xcode
     !!! gestion des erreurs pour l'écriture dans Traces.txt
     !!! gestion si deux traces ont la même fréquence
     
     */
    
    // ------------------------------------------------------------------------
    // ÉTAPE 1 : DIMENSIONS
    // ------------------------------------------------------------------------
    
    // Ouverture du fichier Pixmap.bin
    FILE *input = fopen("/Users/leogallacio/Downloads/Pixmap3/Pixmap.bin", "rb");
    if (input == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier\n");
        return -1;
    }
    
    // Création du tableau de stockage des dimensions
    unsigned short dimensions[3];
    
    // Lecture des 3 premières valeurs
    size_t dim_read = fread(dimensions, sizeof(unsigned short), 3, input);
    int largeur = dimensions[1];
    int hauteur = dimensions[2];
    if (dim_read != 3) {
        printf("Erreur: Impossible de lire les dimensions\n");
        fclose(input);
        return -1;
    }

    // Vérification que les dimensions sont positives
    for (int n = 0; n < 3; n++) {
        if (dimensions[n] <= 0) {
            printf("Erreur: Les dimensions doivent être positives\n");
            fclose(input);
            return -1;
        }
    }
    
    // Vérification que la profondeur est de 8
    if(dimensions[0]!=8){
        printf("Erreur: La profondeur doit être de 8\n");
        fclose(input);
        return -1;
    }

    // Vérification que largeur et hauteur ∈ [100; 1000]
    if ((dimensions[1] < 100 || dimensions[1] > 1000) || (dimensions[2] < 100 || dimensions[2] > 1000)){
        printf("Erreur: Largeur et hauteur doivent être comprises entre 100 et 1000\n");
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
        printf("Erreur: Impossible d'allouer de la mémoire pour les pixels\n");
        fclose(input);
        return -1;
    }

    // Lecture des pixels du fichier
    size_t pix_read = fread(pixel, sizeof(unsigned char), nb_pixels, input);
    if (pix_read != nb_pixels) {
        printf("Erreur: Nombre incompatible de pixels lus\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Création de l'histogramme à 256 entrées
    const int nb_couleur = 256;
    unsigned int *frequence = (unsigned int *)calloc(nb_couleur, sizeof(unsigned int));
    if (frequence == NULL) {
        printf("Erreur: Impossible d'allouer de la mémoire pour l'histogramme des couleurs\n");
        fclose(input);
        return -1;
    }
    
    // Remplissage de l'histogramme avec les fréquences de chaque couleurs
    for (int i = 0; i < nb_pixels; i++){
        frequence[pixel[i]]++;
    }
    
    // Affichage des points de contrôles
    // Calcul du nombre de traces
    int nb_trace = 0;
    int corner_check = 0;
    int traces_sf[100]; //sf = sans filtre
    for (int j = 0; j < nb_couleur; j++){
        if (frequence[j] == 4){
            printf("\nC: %d\n", j);
            corner_check++;
        }
        if ((50 <= frequence[j]) && (300 >= frequence[j])){
            traces_sf[nb_trace] = j;
            nb_trace++;
        }
    }
    
    // Vérification de l'existence des corners
    if (corner_check == 0) {
        printf("Erreur: Aucun points de contrôles trouvé\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Tri des traces dans l'ordre décroissant
    for (int k0 = 0; k0 < nb_trace; k0++) {
        for (int k1 = k0 + 1; k1 < nb_trace; k1++) {
            if (frequence[traces_sf[k0]] < frequence[traces_sf[k1]]) {
                int tmp = 0;
                tmp = traces_sf[k0];
                traces_sf[k0] = traces_sf[k1];
                traces_sf[k1] = tmp;
            }
        }
    }
    
    // Vérification de l'existence d'une trace
    if (nb_trace == 0) {
        printf("Erreur: Aucunne trace trouvée\n");
        free(pixel);
        fclose(input);
        return -1;
    }
    
    // Création d'un nouveau tableau de traces pour garder les 5 premiers
    int traces_f[5]; //f = filtre
    for (int m = 0; m < 5; m++){
        traces_f[m]=traces_sf[m];
    }

    // Affichage du nombre de traces avec les couleurs associées
    if (nb_trace > 5){
        printf("T: ");
        for (int m = 0; m < 5; m++){
            printf("%d ", traces_f[m]);
        }
    }
    else if (nb_trace <= 5){
        printf("T: ");
        for (int m = 0; m < nb_trace; m++){
            printf("%d ", traces_f[m]);
        }
    }
    printf("\n\n");
    
    // ------------------------------------------------------------------------
    // ÉTAPE 3 : COORDONNÉES
    // ------------------------------------------------------------------------

    // Affichage des pixels avec leurs coordonnées
    unsigned int coord_x = 0;
    unsigned int coord_y = 0;
        
    // Création du fichier Traces.txt
    FILE *output = fopen("Traces3.txt", "w");
    if (output == NULL) {
        printf("Erreur: Impossible de créer Traces.txt\n");
        return 1;
    }
    
    // Ecriture des coordonnées des points de contrôle
    fprintf(output, "Corners= [\n");
    for (int i = 0; i < nb_pixels; i++){
        coord_x = i % largeur;
        coord_y = hauteur - i / largeur;
        if ((frequence[pixel[i]] != 0) && (frequence[pixel[i]] == 4)){
            fprintf(output, "%d, %d;\n", coord_x, coord_y);
        }
    }
    fprintf(output, "];\n\n");
    
    // Ecriture des coordonnées des traces (ordre décroissant)
    if (nb_trace > 5){
        for (int i0 = 0; i0 < 5; i0++){
            fprintf(output, "C%d= [\n", i0);
            for (int i1 = 0; i1 < nb_pixels; i1++){
                coord_x = (i1) % largeur;
                coord_y = hauteur - (i1) / largeur;
                if (pixel[i1] == traces_f[i0]){
                    fprintf(output, "%d, %d;\n", coord_x, coord_y);
                }
            }
            fprintf(output, "];\n\n");
        }
    }
    else if (nb_trace <= 5){
        for (int i0 = 0; i0 < nb_trace; i0++){
            fprintf(output, "C%d= [\n", i0);
            for (int i1 = 0; i1 < nb_pixels; i1++){
                coord_x = (i1) % largeur;
                coord_y = hauteur - (i1) / largeur;
                if (pixel[i1] == traces_f[i0]){
                    fprintf(output, "%d, %d;\n", coord_x, coord_y);
                }
            }
            fprintf(output, "];\n\n");
        }
    }
    
    // Ecriture T ={C0 C1 ... };
    fprintf(output, "T ={");
    if (nb_trace > 5){
        for (int a = 0; a < 5; a++){
            fprintf(output, "C%d ", a);
        }
    }
    else if (nb_trace <= 5){
        for (int a = 0; a < nb_trace; a++){
            fprintf(output, "C%d ", a);
        }
    }
    fprintf(output, "};\n\n");
    
    // Fermeture des fichiers
    fclose(input);
    fclose(output);

    // Libérer la mémoire après utilisation
    free(pixel);
    free(frequence);

    return 1;
}

int main(void){
    stockage();
    return 0;
}
