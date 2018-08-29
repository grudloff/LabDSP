%%
%(1)
close all
fs=8*1000;
Wn=1.5*1000/fs;
N=70;

B1=fir1(N,Wn, blackman(N+1));
figure
freqz(B1)
title('Blackman')
B2=fir1(N,Wn, rectwin(N+1));
figure
freqz(B2)
title('Rectangular')

%%
%(2)
close all
fs=8*1000;
Wn=1.5*1000/fs;
N=70;
%Es firpmord no firpmor!!!!!!
%firpmord(F,A,DEV)
% frecuencias , amplitudes y desviación(ripple)
[n,fo,ao,w]=firpmord([Wn Wn+0.02],[1 0],[0.0001 0.0001]);
B1=firpm(N,fo, ao);
figure
freqz(B1)

%%
%(3)
close all
N=70;
B1 = fir2(N,[0 0.1 0.15 0.35 0.4 0.65 0.75 1],[1 1 0 0 0.5 0.5 0 0]);
figure
[H,w]=DTFT(B1,512);
subplot 211
plot(w(512/2:end)/pi,abs(H(512/2:end)));
ylabel('Magnitud')
xlabel('Frecuencia normalizada')
xlim([0 1]);
title('fir2 para N=70');
N=150;
B2 = fir2(N,[0 0.1 0.15 0.35 0.4 0.65 0.75 1],[1 1 0 0 0.5 0.5 0 0]);
[H,w]=DTFT(B2,512);
subplot 212
plot(w(512/2:end)/pi,abs(H(512/2:end)))
ylabel('Magnitud')
xlabel('Frecuencia normalizada')
xlim([0 1])
title('fir2 para N=150')