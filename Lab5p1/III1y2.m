%%
clc 
clear all
close all
%(1)

load('test_training_signals.mat')
soundsc(training_signal)
plot(training_signal)
[x, y, button] = ginput;
save('parte_3_VUS','x','y','button')
%%
load('test_training_signals.mat')
load('parte_3_VUS')
cantidad_de_cruces=zeros(1,length(x)/2);
rms_values=zeros(1,length(x)/2);
for i=1:2:(length(x)-1)
    a=ceil(x(i));
    b=ceil(x(i+1));
    if(button(i)==1)
        %V
        plot(a:b,training_signal(a:b),'r')
    elseif(button(i)==2)
        %U
        plot(a:b,training_signal(a:b),'g')
    else
        %S
        plot(a:b,training_signal(a:b),'b')
    end
    cantidad_de_cruces((i+1)/2)=zero_crossing(training_signal(a:b));
    rms_values((i+1)/2)=RMS(training_signal(a:b));
    hold on
end

%%
%(2)
close all
tipo=button(1:2:end);
pointsize = 15;
scatter(cantidad_de_cruces, rms_values, pointsize, tipo);
[x, y] = ginput(4);
%%
b1=y(1);
b2=y(4);
m1=(y(2)-y(1))/(x(2)-x(1));
m2=(y(4)-y(3))/(x(4)-x(3));
%%
%separación por rms y cruces por cero
scatter(cantidad_de_cruces, rms_values, pointsize, tipo);
load('clusters')
x=1:1400;
hold on
plot(b1+x*m1)
hold on
plot(b2+x*m2)
ylim([0 0.18])
xlim([0 1400])
%%
%separación por rms
[x, y] = ginput(2);
%%
%silención y0 - y1
y0=0;
y1=y(1);
%U y1 -y2
y2=y(2);
%V para arriba

%%
%separación por cruces por cero
[x, y] = ginput(2);
%%
%V x0 - x1
x0=0;
x1=x(1);
%silencio x1 -x2
x2=x(2);
%U para arriba

%%
save('clusters','b1','b1','m1','m2','y0','y1','y2','x0','x1','x2');
%%
load('test_training_signals.mat')
n=25/1000*fs;%tamaño ventanas
N=floor(length(test_signal)/n);
test=zeros(N,3);

%UVS ->{1,2,3}
for i=1:N;
    cut=test_signal(1+(i-1)*n:i*n);
    rms(i)=RMS(cut);
    czero(i)=zero_crossing(cut);
    %rms y czero
    test(i,1)=2;
    if(b2+czero(i)*m2-rms(i)>=0)
        test(i,1)=1; end
    if(b1+czero(i)*m1-rms(i)>=0)
        test(i,1)=3; end
    %rms
    test(i,2)=2;
    if(rms(i)<y2)
        test(i,2)=1; end
    if(rms(i)<y1) 
        test(i,2)=3; end
    %czero
    test(i,3)=1;
    if(czero(i)<x2)
        test(i,3)=3; end
    if(czero(i)<x1) 
        test(i,3)=2; end
end

%%
plot(test_signal)
hold on
stem((1:N)*n, ones(1,N), 'MarkerFaceColor', 'none', 'MarkerEdgeColor', 'none', 'LineWidth', 0.01);

%%
subplot 311
scatter(czero, rms,15 ,test(:,1));
subplot 312
scatter(czero, rms,15 ,test(:,2));
subplot 313
scatter(czero, rms,15 ,test(:,3));
