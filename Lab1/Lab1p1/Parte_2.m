close all
clear all
for i=[[100;1/10;1] [30;1/3;2] [20;1/2;3] [9;10/9;4]]
    subplot(4,1,i(3))
    n=0:1:i(1);
    y=sin(2*pi*i(2)*n);
    stem(n,y);
    title(strcat('Señal (',int2str(i(3)),')'))
    ylim([-1 1])
    xlabel('Muestra')
    ylabel('Amplitud')
end