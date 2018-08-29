%%
% (1)
close all
info_besh=audioinfo('besh.wav');
[besh,Fs]=audioread('besh.wav');

t=0:1/Fs:info_besh.Duration;
t = t(1:end-1);


subplot 311
plot(t,besh,'b')
xlabel('Tiempo')
ylabel('besh')

subplot 312
plot(t,besh,'r')
xlabel('Tiempo')
ylabel('/e/')
xlim([0.065 0.18])

subplot 313
plot(t,besh,'black')
xlabel('Tiempo')
ylabel('/sh/')
xlim([0.22 0.3])

%%
% (2)
vocal=besh(0.065*Fs:0.18*Fs);
audiowrite('Lab2p1_vocal.wav',vocal,Fs)

%%
%(3)
chop_audio('besh.wav','new_besh.wav')

%%
%(4)
close all 
%Parte IV 3) AM con h=0.5
F_c=500;%Hz
F_m=50;%Hz
h=0.5;
A_c=1/(1+h);%Amplitud portadora
Fs=20000;
t_max=400*10^-3;
t=0:1/Fs:t_max;
t=t(1:end-1);

m=sin(2.*pi.*F_m.*t);
x_c=A_c.*[1+h.*m].*cos(2.*pi.*F_c.*t);
subplot 221
plot(t,x_c)
title('modulación AM, h=0.5')
xlabel('tiempo [ms]')
ylabel('amplitud')

audiowrite('Lab2p1_AM.wav',x_c,Fs)
%%
clear
%FM con h=4
F_c=500;%1/ms
F_m=50;%1/ms
w_m=F_m*2*pi;
h=4;
w_d=h*w_m;
A_c=1;%Amplitud portadora

Fs=20000;
t_max=400*10^-3;
t=0:1/Fs:t_max;
t=t(1:end-1);
m=sin(2.*pi.*F_m.*t);
p=size(t);
x_c=zeros(p(2),1);
t_m=1/Fs;
x_c=A_c.*cos((2.*pi.*F_c.*t)+w_d*cumtrapz(m)*t_m);
subplot 223
plot(t,x_c)
title('modulación FM, h=4')
xlabel('tiempo [ms]')
ylabel('amplitud')

audiowrite('Lab2p1_FM.wav',x_c,Fs)

%%
chop_audio('Lab2p1_AM.wav','Lab2p1_AM.wav')
chop_audio('Lab2p1_FM.wav','Lab2p1_FM.wav')

info=audioinfo('Lab2p1_AM.wav');
[x,Fs]=audioread('Lab2p1_AM.wav');

t=0:1/Fs:info.Duration;
t = t(1:end-1);

subplot 222
plot(t,x)
title('modulación AM, h=0.5, cropped')
xlabel('tiempo [ms]')
ylabel('amplitud')

info=audioinfo('Lab2p1_FM.wav');
[x,Fs]=audioread('Lab2p1_FM.wav');

t=0:1/Fs:info.Duration;
t = t(1:end-1);

subplot 224
plot(t,x)
title('modulación FM, h=4, cropped')
xlabel('tiempo [ms]')
ylabel('amplitud')