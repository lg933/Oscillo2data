function W = applyM(M, C)

% Stockage des coordonnées XY dans U
U = [C(:,1) C(:,2)];

% Ajout d'une 3e colonne de 1 (passage coordonnées homogènes)
U(:,3)=1;

% Transposition de U
T = U';

% Produit matrice-vecteur de M et U'
V = M*T;

% Transposition de V
V = V';

% Conversion coordonnées homogènes vers cartésiens (diviser x et y par z)
W = [V(:,1)./V(:,3) V(:,2)./V(:,3)];

end