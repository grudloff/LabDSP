%%
close all
w1=2*pi*100;
A1=0.5;
w2=2*pi*500;
A2=1.5;
Tmax=0.1;
N=4096;
fs=N/Tmax;
t=Tmax*(0:(N-1))/(N-1);

x1=A1*cos(w1*t);
x2=A2*cos(w2*t);
s=x1+x2+2*randn(1,N);

plot(1000*t,s);
ylabel('S(t)');
xlabel('t[ms]')

%No se puede sacar mucha informaci�n a simple vista de la se�al con ruido

%%
X=fft(x1+x2,N);
S=fft(s,N);
X=abs(X)/N;
S=abs(S)/N;

X=X(1:N/2+1);
X(2:end-1)=2*X(2:end-1);
S=S(1:N/2+1);
S(2:end-1)=2*S(2:end-1);

f = fs*(0:(N/2))/N;%freq normalizada

figure
ylabel('Magnitud')
plot(f,X)
hold on
plot(f,S)
xlabel('Frecuencia[Hz]')
xlim([0,1000]);

%La amplitud de los tonos no es la misma ya que en esa frecuencia tambien 
%hay una parte que es aportada por el ruido blanco

%%
close all

plot(f,mag2db(X))
title('Magnitud en dB')
ylabel('Magnitud[dB]')
hold on
plot(f,mag2db(S))
xlabel('Frecuencia[Hz]')
xlim([0,1000]);

figure

plot(f,mag2db(X/max(X)))
title('Magnitud en dB normalizada')
ylabel('Magnitud[dB]')
hold on
plot(f,mag2db(S/max(S)))
hold on
plot(f,ones(1,length(f))*mean(mag2db(S)))
xlabel('Frecuencia[Hz]')
xlim([0,1000]);

-mean(mag2db(S))
db2mag(-mean(mag2db(S)))
%Es 26.4 dB m�s grande, que es 20.87 en veces

%%
%close all
w1=2*pi*100;
A1=0.5;
w2=2*pi*500;
A2=1.5;
Tmax=1;
N=4096;
fs=N/Tmax;
t=Tmax*(0:(N-1))/(N-1);

x1=A1*cos(w1*t);
x2=A2*cos(w2*t);
s=x1+x2+2*randn(1,N);

X=fft(x1+x2,N);
S=fft(s,N);
X=abs(X)/N;
S=abs(S)/N;

X=X(1:N/2+1);
X(2:end-1)=2*X(2:end-1);
S=S(1:N/2+1);
S(2:end-1)=2*S(2:end-1);
f = fs*(0:(N/2))/N;%freq normalizada

figure

plot(f,mag2db(X/max(X)))
title('Magnitud en dB normalizada T_{max}=1[seg]')
ylabel('Magnitud[dB]')
hold on
plot(f,mag2db(S/max(S)))
hold on
plot(f,ones(1,length(f))*mean(mag2db(S)))
xlabel('Frecuencia[Hz]')
xlim([0,1000]);

-mean(mag2db(S))
db2mag(-mean(mag2db(S)))
%Se obtienen valores muy cercanos ????
%Piden que sea 1[seg] pero eso no cambia el N, aumentar N si disminuye el
%piso de ruido, aumentar el tiempo no.

%%
%Aumentando conjuntamente tiempo y cantidad de puntos en un factor de 10
%close all
w1=2*pi*100;
A1=0.5;
w2=2*pi*500;
A2=1.5;
Tmax=1;
N=2^nextpow2(10*4096);
fs=N/Tmax;
t=Tmax*(0:(N-1))/(N-1);

x1=A1*cos(w1*t);
x2=A2*cos(w2*t);
s=x1+x2+2*randn(1,N);

X=fft(x1+x2,N);
S=fft(s,N);
X=abs(X)/N;
S=abs(S)/N;

X=X(1:N/2+1);
X(2:end-1)=2*X(2:end-1);
S=S(1:N/2+1);
S(2:end-1)=2*S(2:end-1);
f = fs*(0:(N/2))/N;%freq normalizada

figure

plot(f,mag2db(X/max(X)))
title('Magnitud en dB normalizada T_{max}=1[seg]')
ylabel('Magnitud[dB]')
hold on
plot(f,mag2db(S/max(S)))
hold on
plot(f,ones(1,length(f))*mean(mag2db(S)))
xlabel('Frecuencia[Hz]')
xlim([0,1000]);

-mean(mag2db(S))
db2mag(-mean(mag2db(S)))

%Se obtiene un piso de ruido mucho m�s bajo del orden de 38db ,85.47 veces
%porque???????? (esto falta)