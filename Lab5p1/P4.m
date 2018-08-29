%% Funcion de la pregunta 4
% x: es la entrada. n: tamaño de ventana en la cual x se dividirá. p: orden del LPC 
function [test_RMSyZC, rms_v, lpc_m]=P4(x,n,p)
N=floor(length(x)/n);
test_RMSyZC=zeros(1,N);
b1=0.0416;
b2=0.0223+0.043;%la suma es un bias
m1=-2.2515e-5;
m2=-1.6572e-5+(-1.6572e-5)*0.15;%la suma es un bias
rms_v=zeros(1,N);
lpc_m=zeros(N,p+1);
for i=1:N;
    cut=x(1+(i-1)*n:i*n);
    rms_v(i)=rms(cut);
    czero(i)=zero_crossing(cut);
    %VUS ->{1,-1,0}
    test_RMSyZC(i)=1;
    if(b2+czero(i)*m2-rms_v(i)>=0)
        test_RMSyZC(i)=-1;
    end
    if(b1+czero(i)*m1-rms_v(i)>=0)
        test_RMSyZC(i)=0;
    end
    lpc_m(i,:)=mylpc(cut,p);
end
