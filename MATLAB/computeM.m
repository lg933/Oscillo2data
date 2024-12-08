function M = computeM(A,B,C,D, Width, Height)

% Définition des coordonnés initiaux A0 B0 C0 D0 (déformés)
A0 = A;
B0 = B;
C0 = C;
D0 = D;

% Définition des coordonnés finaux A1 B1 C1 D1 (reformés)
A1 = [0 0];
B1 = [Width 0];
C1 = [Width Height];
D1 = [0 Height];

% Définition matrice A du système Ax=b
A = [
    A0(1) A0(2) 1 0 0 0 -A1(1)*A0(1) -A1(1)*A0(2); 
    0 0 0 A0(1) A0(2) 1 -A1(2)*A0(1) -A1(2)*A0(2);
    B0(1) B0(2) 1 0 0 0 -B1(1)*B0(1) -B1(1)*B0(2); 
    0 0 0 B0(1) B0(2) 1 -B1(2)*B0(1) -B1(2)*B0(2);
    C0(1) C0(2) 1 0 0 0 -C1(1)*C0(1) -C1(1)*C0(2); 
    0 0 0 C0(1) C0(2) 1 -C1(2)*C0(1) -C1(2)*C0(2);
    D0(1) D0(2) 1 0 0 0 -D1(1)*D0(1) -D1(1)*D0(2); 
    0 0 0 D0(1) D0(2) 1 -D1(2)*D0(1) -D1(2)*D0(2);
    ];

% Définition du vecteur b = coordonnés finaux (reformé)
b = [A1 B1 C1 D1];

% Calcul du vecteur x = contient les coeff de la matrice M
x = A\b';

% Construction de la matrice de transformation M à partir de x
M = [x' 1];
M = reshape(M, 3, 3)';

end