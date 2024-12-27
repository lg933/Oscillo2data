function GraphicsDisplay(X, Y, ParamStr, Yfit, Title, MarkerSize, MarkerShape, Colors, i)

% Création du graphique
figure(1);
hold on;

% Grille X
for k = [-0.8, -0.6, -0.4 -0.2, 0.2, 0.4, 0.6, 0.8]
    plot([0, 1], [k, k], 'Color', [0.925, 0.925, 0.925], 'LineWidth', 0.05, 'HandleVisibility', 'off');
end

% Grille Y
for k = [0.1, 0.2, 0.3, 0.4, 0.6, 0.7, 0.8, 0.9]
    plot([k, k], [-1, 1], 'Color', [0.925, 0.925, 0.925], 'LineWidth', 0.05, 'HandleVisibility', 'off');
end

% Grille centre
plot([0, 1], [0, 0], 'Color', [0.5, 0.5, 0.5], 'LineWidth', 0.3, 'HandleVisibility', 'off');
plot([0.5, 0.5], [-1, 1], 'Color', [0.5, 0.5, 0.5], 'LineWidth', 0.3, 'HandleVisibility', 'off');

% Cadre
plot([0, 1], [-1, -1], 'k-', 'LineWidth', 1, 'HandleVisibility', 'off');
plot([0, 1], [1, 1], 'k-', 'LineWidth', 1, 'HandleVisibility', 'off');
plot([0, 0], [-1, 1], 'k-', 'LineWidth', 1, 'HandleVisibility', 'off');
plot([1, 1], [-1, 1], 'k-', 'LineWidth', 1, 'HandleVisibility', 'off');

% Conversion des couleurs RGB au format MATLAB
Colors = Colors./255;

% Plot des points initiaux
plot(X, Y, MarkerShape, 'color', Colors(i, :), 'MarkerSize', MarkerSize, 'HandleVisibility', 'off');

% Plot points du fit
plot(X, Yfit, '-', 'color', Colors(i, :), 'LineWidth', 0.5,'DisplayName', ParamStr);

% Titre
time = datetime('now', 'Format', 'dd.MM.yyyy - HH:mm:ss'); % date
titleformat = sprintf("Src : '%s' @ %s", Title, time);     % Src
title(titleformat, 'Color', 'blue');                       % Construction du titre

% Label axe Y
text(-0.0065, 0.71, '0.2[v]/div', 'Rotation', 90, 'HorizontalAlignment', 'left', 'VerticalAlignment', 'bottom');

% Label axe X
text(1, -1.10375, '0.1[s]/div', 'HorizontalAlignment', 'right', 'VerticalAlignment', 'bottom');
pbaspect([1 1 1]);

% Légende
ax = gca;
ax.Position = [0.1, 0.275, 0.8, 0.697]; % ajout espace graphique-légende
lgd = legend('show', 'Location', 'southoutside', 'Orientation', 'vertical');
lgd.Units = 'normalized';
lgd.Position = [0.239, 0.1, 0.522, 0.1]; % position légende

% Grille
xlim([0 1]);  % limite x du plot
ylim([-1 1]); % limite y du plot
grid on;
set(gca,'XTick',[]); % suppression des ticks x
set(gca,'YTick', 0); % suppression des ticks y
end
