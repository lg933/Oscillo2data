function [Width, Height, A,B,C,D] = orderCorners(Corners)

% Définition des dimensions Width & Height
Width = max(Corners(:, 1));
Height = max(Corners(:, 2));

%Calcul distance Corners-Origine (en bas à gauche)
distances = zeros(1, 4);
for n = 1:4
    distances(n) = sqrt(Corners(n, 1)^2 + Corners(n, 2)^2);
end

% Assigne le point le plus proche de (0,0) au point A
idxA = find(distances == min(distances));
Tmp = Corners(1,:);
Corners(1,:) = Corners(idxA, :);
Corners(idxA, :) = Tmp;

xA = Corners(1, 1);
yA = Corners(1, 2);

% Calcul des angles formés par chaque Corners avec A
angleB = atan2((Corners(2, 2)-yA), (Corners(2, 1)-xA));
angleC = atan2((Corners(3, 2)-yA), (Corners(3, 1)-xA));
angleD = atan2((Corners(4, 2)-yA), (Corners(4, 1)-xA));

angle = [angleB angleC angleD];

% Tri par bulle des angles (ordre croissant)
for n = 1:length(angle)-1
    for m = 1:length(angle)-n
        if angle(m) > angle(m + 1)
            % Tri des angles
            tmp1 = angle(m);
            angle(m) = angle(m + 1);
            angle(m + 1) = tmp1;
            % Tri des Corners
            tmp2 = Corners(m+1, :);
            Corners(m+1, :) = Corners(m+2, :);
            Corners(m+2, :) = tmp2;
        end
    end
end

% Affichage
A = Corners( 1, :);
B = Corners( 2, :);
C = Corners( 3, :);
D = Corners( 4, :);

end
