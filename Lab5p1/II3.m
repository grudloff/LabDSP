%% Parte 3
II1
II2
y_a=filter(1,AR_a,X);
y_e=filter(1,AR_e,X);
y_i=filter(1,AR_i,X);
y_o=filter(1,AR_o,X);
y_u=filter(1,AR_u,X);
%% sonidos
close all

soundsc(y_a)
pause(2)
soundsc(y_e)
pause(2)
soundsc(y_i)
pause(2)
soundsc(y_o)
pause(2)
soundsc(y_u)
pause(2)

audiowrite('vocalA_generada.wav',y_a,Fs)
audiowrite('vocalE_generada.wav',y_e,Fs)
audiowrite('vocalI_generada.wav',y_i,Fs)
audiowrite('vocalO_generada.wav',y_o,Fs)
audiowrite('vocalU_generada.wav',y_u,Fs)

%% espectro
figure
periodogram(y_a,'onesided')
title('Espectro de vocal A, generada')
figure
periodogram(y_e,'onesided')
title('Espectro de vocal E, generada')
figure
periodogram(y_i,'onesided')
title('Espectro de vocal I, generada')
figure
periodogram(y_o,'onesided')
title('Espectro de vocal O, generada')
figure
periodogram(y_u,'onesided')
title('Espectro de vocal U, generada')



