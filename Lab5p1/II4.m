%% parte 4 (II)
clc
clear all
close all
%II1
Fs=8000;
Tmax=1;
Fp=100;
N=Tmax*Fs;
Np=Fs/Fp;
X=exciteV(N,Np);

%II2
load('vowels.mat')
p=15;
AR_a=mylpc(vowel_a,p);
AR_e=mylpc(vowel_e,p);
AR_i=mylpc(vowel_i,p);
AR_o=mylpc(vowel_o,p);
AR_u=mylpc(vowel_u,p);

figure
freqz(1,AR_a,1000)
title('filtro AR vocal a')
figure
freqz(1,AR_e,1000)
title('filtro AR vocal e')
figure
freqz(1,AR_i,1000)
title('filtro AR vocal i')
figure
freqz(1,AR_o,1000)
title('filtro AR vocal o')
figure
freqz(1,AR_u,1000)
title('filtro AR vocal u')

%II3
ymy_a=filter(1,AR_a,X);
ymy_e=filter(1,AR_e,X);
ymy_i=filter(1,AR_i,X);
ymy_o=filter(1,AR_o,X);
ymy_u=filter(1,AR_u,X);

figure
periodogram(ymy_a,'onesided')
title('Espectro de vocal A, generada')
figure
periodogram(ymy_e,'onesided')
title('Espectro de vocal E, generada')
figure
periodogram(ymy_i,'onesided')
title('Espectro de vocal I, generada')
figure
periodogram(ymy_o,'onesided')
title('Espectro de vocal O, generada')
figure
periodogram(ymy_u,'onesided')
title('Espectro de vocal U, generada')

soundsc(ymy_a)
pause(2)
soundsc(ymy_e)
pause(2)
soundsc(ymy_i)
pause(2)
soundsc(ymy_o)
pause(2)
soundsc(ymy_u)
pause(2)
%%
audiowrite('vocalA_generada_mylpc.wav',ymy_a,Fs)
audiowrite('vocalE_generada_mylpc.wav',ymy_e,Fs)
audiowrite('vocalI_generada_mylpc.wav',ymy_i,Fs)
audiowrite('vocalO_generada_mylpc.wav',ymy_o,Fs)
audiowrite('vocalU_generada_mylpc.wav',ymy_u,Fs)
