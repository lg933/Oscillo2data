function [X, Y, ParamStr, Yfit] = FindTrace(W, Width, Height)

% Tri des x dans l'ordre croissant
W_sorted = sortrows(W, 1);

% Normalisation autour de -1 et 1
X = W_sorted(:, 1)./Width;
Y = (W_sorted(:, 2) - Height/2)./(Height/2);

% Calcul du DC (composante continue)
DC = (max(Y)+min(Y))/2;

% Suppression du DC du signal
Y_DC = Y-DC;

% Fit sinusoïdal y = a⋅sin(b⋅x + c)
% a : amplitude
% b : fréquence angulaire (rad/s)
% c : phase
% sintype = fittype('a*sin(b*x+c)');
fitsin = fit(X, Y_DC, 'sin1');
Yfitsin = feval(fitsin, X)+DC;
RMSEsin = sqrt(mean((Y-Yfitsin).^2));

% Fit polynomial de degré 3 y = p1⋅x³ + p2⋅x² + p3⋅x + p4
fitpoly = fit(X, Y, 'poly3');
Yfitpoly = feval(fitpoly, X);
RMSEpoly = sqrt(mean((Y-Yfitpoly).^2));

% Choix du fit
if RMSEsin < RMSEpoly
    Yfit = Yfitsin;
    offset = (max(Yfit)+min(Yfit))/2;
    ParamStr = sprintf( ...
        'A: %.2f[v], DC: %.1f[v], F: %.2f[Hz], P: %.2f[deg], RMSE: %.2f', ...
        fitsin.a1, ...
        offset, ...
        fitsin.b1/(2*pi), ...
        fitsin.c1*180/pi, ...
        RMSEsin);
else 
    Yfit = Yfitpoly;
    ParamStr = sprintf( ...
        'P1: %.2f, P2: %.2f, P3: %.2f, P4: %.2f, RMSE: %.2f', ...
        fitpoly.p1, ...
        fitpoly.p2, ...
        fitpoly.p3, ...
        fitpoly.p4, ...
        RMSEpoly);
end
end
