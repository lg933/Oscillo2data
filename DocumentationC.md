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
     Erreur: Nombre de pixels lus incompatible avec les dimensions
     Erreur: Impossible d'allouer de la mémoire pour l'histogramme des couleurs
     Erreur: Aucun points de contrôles trouvés
     Erreur: Aucunne trace trouvée
     Erreur: Impossible de créer Traces.txt
