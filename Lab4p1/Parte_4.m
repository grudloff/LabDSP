%%
clear all
close all
load('nspeech.mat');
s=nspeech;
N=2^nextpow2(length(s));
S=fft(s,N);

S1=S;
S1=S1(1:N/2+1);
S1(2:end-1)=2*S1(2:end-1);
f = fs*(0:(N/2))/N;%freq normalizada

plot(f,mag2db(S1))
title('nspeech original')
ylabel('Magnitud[dB]')
xlabel('freq [Hz]')

%freq tono puro en 1684

theta=2*pi*1685/fs;
h=[1,-2*cos(theta),1];
H=fft(h,N);

X=H.*S;
x=ifft(X,'symmetric');

X=X(1:N/2+1);
X(2:end-1)=2*X(2:end-1);
f = fs*(0:(N/2))/N;%freq normalizada

figure
plot(f,mag2db(X))
title('nspeech filtrado')
ylabel('Magnitud[dB]')
xlabel('freq [Hz]')

figure
t=(0:length(s)-1)/fs;
plot(t,s)
xlim([0 length(s)/fs])
title('nspeech original en el tiempo')
ylabel('Magnitud')
xlabel('t[seg]')
soundsc(s,fs)

pause(1.6)

figure
t=(0:N-1)/fs;
plot(t,x)
xlim([0 length(s)/fs])
title('nspeech filtrada en el tiempo')
ylabel('Magnitud')
xlabel('t[seg]')
soundsc(x,fs)

%se requiere un N mayor el largo de la señal origianl

%la version extendida tiene mayor largo ya que se utiliza N en potencia de
%2 lo que evita poder usar el mismo largo de la señal original

%no se elimina del todo el tono molesto, esto se puede notar en la fft,
%pero no se nota viendo la señal en el tiempo o escuchandola.
