ts = wrev(ts); % reverse ts

[tt, xx] = meshgrid(xs', ts');

f = figure;
set(f, 'Visible', 'off');

subplot(121), surf(tt, xx, V)
title('Portfolio value with \alpha allocation')
xlabel('x')
ylabel('Time')
zlabel('X')

alphas = [zeros(size(alphas, 1), 1), alphas];
alphas = [alphas; zeros(1, size(alphas, 2))];
subplot(122), surf(tt, xx, alphas)
title('Optimal allocation \alpha')
xlabel('x')
ylabel('Time')
zlabel('\alpha')
view(2)
colorbar()

saveas(gcf, output)