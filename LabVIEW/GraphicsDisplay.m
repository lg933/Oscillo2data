function GraphicsDisplay(X, Y, ParamStr, Yfit, Title, MarkerSize, MarkerShape, Colors, i)

% Création graphique
figure(1);
hold on;

% Conversion RGB colors au format MATLAB
Colors = Colors./255;

% Plot des points initiaux
plot(X, Y, MarkerShape, 'color', Colors(i, :), 'MarkerSize', MarkerSize, 'HandleVisibility', 'off');

% Plot points du fit
plot(X, Yfit, '-', 'color', Colors(i, :), 'LineWidth', 0.5,'DisplayName', ParamStr);

% Legend
legend('show', 'Location', 'southoutside', 'Orientation', 'vertical');

% Titre
time = datetime('now', 'Format', 'dd.MM.yyyy - HH:mm:ss');
titleformat = sprintf("Src : '%s' @ %s", Title, time);
title(titleformat, 'Color', 'blue');

% Axes
xlabel('0.1[s]/div');
ylabel('0.2[v]/div');
pbaspect([1 1 1]);

% Grille
grid on;
xlim([0 1]);
ylim([-1 1]);
xticks(linspace(0, 1, 11));
yticks(linspace(-1, 1, 11));
end
