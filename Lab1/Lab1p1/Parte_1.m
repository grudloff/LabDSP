close all;
t = 0:2:60;
y = sin(t/6);
plot(t,y)

subplot 311
stem(t,y)
grid on
title('stem señal normal')
subplot 312
plot(t,y,'g')
grid on
y = sin(t/6);
title('plot señal normal')
subplot 313
t = 0:(2*5):60;
y = sin(t/6);
plot(t,y,'c','LineWidth',4)
hold on
stem(t,y,'m')
title('plot y stem señal submuestreada')