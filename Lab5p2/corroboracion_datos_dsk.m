close all
clear all
load('toma_datos.mat')

figure
yyaxis left
stem(LPC_dsp)
yyaxis right
LPCcoef=lpc(x,15);
stem(LPCcoef)
title('Coeficientes lpc dsp vs matlab')
%X=zeros(80,1);
%X(1)=1;
%X(40)=1;
%X=X-mean(X);
X=[32400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     32400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400,
     -400];
 
 figure
 
 plot(LPCcoef-LPC_dsp')
 title('Error Coeficientes lpc')
%y=filter(1,LPCcoef,X);
%figure
%yyaxis left
%plot(salida)
%yyaxis right 
%plot(y)
%title('Salida DSP vs Salida Matlab')