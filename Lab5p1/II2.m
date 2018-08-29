%% Parte2
%clear
%clc %comentar para la parte 3 (comentar tambien para la 1)
load('vowels.mat')
p=15;
AR_a=lpc(vowel_a,p);
AR_e=lpc(vowel_e,p);
AR_i=lpc(vowel_i,p);
AR_o=lpc(vowel_o,p);
AR_u=lpc(vowel_u,p);

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

