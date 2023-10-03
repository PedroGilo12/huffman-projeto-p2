% Leitura dos dados do arquivo AVL
avl_data = dlmread('output_avl.txt', '\t');
avl_n = avl_data(:, 1);
avl_comparisons = avl_data(:, 2);

% Leitura dos dados do arquivo Degenerate
degenerate_data = dlmread('output_degenerate.txt', '\t');
degenerate_n = degenerate_data(:, 1);
degenerate_comparisons = degenerate_data(:, 2);

% Cria o gráfico de dispersão (apenas pontos) para a Árvore AVL (pontos vermelhos e menores)
subplot(1, 2, 1);
scatter(avl_n, avl_comparisons, 10, 'r', 'filled'); % Tamanho 10 e cor vermelha
title('Árvore AVL');
xlabel('Tamanho de Entrada (n)');
ylabel('Comparações');
grid on;

% Cria o gráfico de dispersão (apenas pontos) para a Árvore Binária Degenerada (pontos menores)
subplot(1, 2, 2);
scatter(degenerate_n, degenerate_comparisons, 10, 'filled'); % Tamanho 10
title('Árvore Binária Degenerada');
xlabel('Tamanho de Entrada (n)');
ylabel('Comparações');
grid on;

% Ajusta o tamanho da janela de plotagem
set(gcf, 'Position', [100, 100, 1200, 500]);

