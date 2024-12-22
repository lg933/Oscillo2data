function GraphicsDisplay(X, Y, ParamStr, Yfit, Title, MarkerSize, MarkerShape, Colors, i)

% Graphique
figure(1);
hold on;

Colors = Colors./255;

% Points initiaux
plot(X, Y, MarkerShape, 'color', Colors(i, :), 'MarkerSize', MarkerSize, 'HandleVisibility', 'off');

% Points du fit
plot(X, Yfit, '-', 'color', Colors(i, :), 'LineWidth', 0.5,'DisplayName', ParamStr);

% Legend
legend('show', 'Location', 'southoutside', 'Orientation', 'vertical');

% Titre
time = datetime('now', 'Format', 'dd.MM.yyyy - HH:mm:ss');
title_format = sprintf("Src : '%s' @ %s", Title, time);
title(title_format);

% Axes
xlabel('0.1[s]/div');
ylabel('0.2[v]/div');
pbaspect([1 1 1]);

% Grid
grid on;
xlim([0 1]);
ylim([-1 1]);
xticks(linspace(0, 1, 11));
yticks(linspace(-1, 1, 11));
end
