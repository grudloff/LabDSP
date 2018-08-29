%-----------(1)-----------
%%
close all
fs_1=8*500;%nyquist
tmax=1;
t=0:1/fs_1:tmax;
t=t(1:end-1);
size t
s1_1=sin(2*pi*50*t);
s2_1=sin(2*pi*500*t);
subplot 211
plot(t*1000,s1_1);
title('S1');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 50])
soundsc(s1_1,fs_1);
subplot 212
plot(t*1000,s2_1);
hold on
title('S2');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 50])
pause(1)
soundsc(s2_1,fs_1);
%-----------(2)-----------

%%
close all
s1_2=sin(2*pi*50*t+pi/2);
s2_2=sin(2*pi*500*t+pi/2);
subplot 211
plot(t*1000,s1_2,'g');
title('S1');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
soundsc(s1_2,fs_1);
xlim([0 50]);
subplot 212
plot(t*1000,s2_2,'r');
title('S2');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 50]);
pause(1)
soundsc(s2_2,fs_1);
%-----------(3)-----------

%%
close all
tmax=2;
t=0:1/fs_1:tmax;
t=t(1:end-1);
s_3a=sin(2*pi*50*t)+sin(2*pi*500*t);
%subplot 311
plot(t*1000,s_3a,'g');
title('S3');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
soundsc(s_3a,fs_1);
pause(2)
soundsc(sin(2*pi*50*t),fs_1);
xlim([0 50]);
pause(2)

% La fundamental seria 50 Hz ya que 500 Hz es un armonico de esta (10 veces la fundamental).

s_3b=sin(2*pi*200*t)+sin(2*pi*300*t);
%subplot 312
%plot(t,s_3b,'g');
soundsc(s_3b,fs_1);
pause(2)
soundsc(sin(2*pi*100*t),fs_1);

%xlim([0 0.05]);
pause(2)
% Con el caso de 200 Hz y 300 Hz una no es multiplo de la otra pero su maximo 
% factor comun es el 100, por lo que su fundamental seria 100 Hz ya que ambos son armonicos 
% de esta.

s_3c=sin(2*pi*200*t)+sin(2*pi*203*t);
%subplot 313
%plot(t,s_3c,'g');
soundsc(s_3c,fs_1);

%xlim([0 0.05]);
% Con 200 y 203 sucede lo mismo pero la fundamental seria 1 Hz ya que no hay factores comunes.

%-----------(4)-----------

%%
close all
fs_1=8*500;%nyquist
tmax=1;
t=0:1/fs_1:tmax;
t=t(1:end-1);
s_4a=sin(2*pi*500*t);
fs_4a=fs_1;

s_r_4a=s_4a+2*(rand(1,tmax*fs_4a)-0.5);
subplot 311
plot(t*1000,s_4a)
hold on
plot(t*1000,s_r_4a)
title('Fs');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 10]);
soundsc(s_r_4a,fs_4a);

pause(2)

fs_4b=fs_4a*2;
t=0:1/fs_4b:(tmax-1/fs_4b);
s_4b=sin(2*pi*500*t);
s_r_4b=s_4b+2*(rand(1,tmax*fs_4b)-0.5);
subplot 312
plot(t*1000,s_4b)
hold on
plot(t*1000,s_r_4b)
title('2Fs');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 10]);
soundsc(s_r_4b,fs_4b);

pause(2)

fs_4c=fs_4a*6;
t=0:1/fs_4c:tmax;
t=t(1:end-1);
s_4c=sin(2*pi*500*t);

s_r_4c=s_4c+2*(rand(1,tmax*fs_4c)-0.5);
subplot 313
plot(t*1000,s_4c)
hold on
plot(t*1000,s_r_4c)
title('6Fs');
xlabel('Tiempo [ms]')
ylabel('Amplitud')
xlim([0 10]);
soundsc(s_r_4c,fs_4c);


%-----------(5)-----------

%%
close all
fs_5=fs_1;
t=0:1/fs_5:tmax-1/fs_5;
s_5=sin(2*pi*500*t);
s_r_5=s_5.*(0.5+0.5*rand(1,tmax*fs_5));

plot(t*1000,s_5)
hold on
plot(t*1000,s_r_5)
xlim([0 10]);
xlabel('Tiempo [ms]')
ylabel('Amplitud')
soundsc(s_r_5,fs_5);

%-----------(6)-----------

%%
close all
fs_6=fs_1;
t=0:1/fs_6:tmax-1/fs_6;
s_6=sin(2*pi*500*t);
s_r_6=sin(2*pi*500*t-pi/2*rand(1,tmax*fs_6));

plot(t*1000,s_6)
hold on
plot(t*1000,s_r_6)
soundsc(s_r_6,fs_6);
xlim([0 10]);

%%
%-----------(7)-----------
soundsc(s_r_4a,fs_4a);
pause(2)
soundsc(s_r_5,fs_5);
pause(2)
soundsc(s_r_6,fs_6);

%en el primer punto se tiene ruido aditivo, en el siguiente ruido
%multiplicativo y finalmente se tiene ruido en fase.

%todos estos suenan bastante similar, ruido blanco donde uno nota mas las 
%altas frecuencias, la unica diferencia que logro captar es en el punto 5 
%es que suena mas bajo el ruido o tal vez con menos altos (ruido rosa?)