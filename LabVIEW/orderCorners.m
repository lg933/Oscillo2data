function [Width, Height, A,B,C,D] = orderCorners(Corners)

% Définition des dimensions Width & Height
Width = max(Corners(:, 1));
Height = max(Corners(:, 2));

%Calcul distance Corners-Origine (en bas à gauche)
distances = zeros(1, 4);
for n = 1:4
    distances(n) = sqrt(Corners(n, 1)^2 + Corners(n, 2)^2); % distance euclidienne
end

% Assigne le point le plus proche de (0,0) au corner A
idxA = find(distances == min(distances)); % index du corner avec la plus petite distance
Tmp = Corners(1,:);                       % stockage corner 1 dans variable temporaire
Corners(1,:) = Corners(idxA, :);          % échange corner 1 avec corner dont distance est la plus faible
Corners(idxA, :) = Tmp;                   % échange avec corner 1

xA = Corners(1, 1); % définition abscisse corner A
yA = Corners(1, 2); % définition ordonnée corner A

% Calcul des angles formés par chaque Corners avec A
angleB = atan2((Corners(2, 2)-yA), (Corners(2, 1)-xA));
angleC = atan2((Corners(3, 2)-yA), (Corners(3, 1)-xA));
angleD = atan2((Corners(4, 2)-yA), (Corners(4, 1)-xA));

% Tri des lignes
Corners = [               % redéfinition de Corners 
    angleB Corners(2, :); % ajout des angles
    angleC Corners(3, :); % en 1ère colonne
    angleD Corners(4, :); % pour utiliser sortrows
    ];

Corners = sortrows(Corners); % tri

% Réarrangement de Corners
Corners(:,1) = [];

% Version triée de Corners
Corners = [
    xA yA;
    Corners(1, :);
    Corners(2, :);
    Corners(3, :);
    ];

% Valeurs retournées
A = Corners( 1, :);
B = Corners( 2, :);
C = Corners( 3, :);
D = Corners( 4, :);

end
