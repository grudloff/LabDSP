%close all
%(1)
Fs=8000;
Tmax=1;
Fp=100;
N=Tmax*Fs;
Np=Fs/Fp;
X=exciteV(N,Np);
%soundsc(X);
%periodogram(X,'onesided')