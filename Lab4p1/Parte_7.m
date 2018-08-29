%%
%(1)
clc
REPS=1000;
for N=[2,4, 8]
    tic;
    for i=1:REPS
        dftmtx(N);
    end
    toc
    
    for i=1:REPS
        DFTmatrixA(N);
    end
    toc
        
end

%hacer tabla con valores

%%
%(2)
close all
for N=[4,32,256]
    A=DFTmatrixA(N);
    figure
    clear A_aux;
    A_aux(:,:,1)=real(A);
    A_aux(:,:,3)=imag(A);
    A_aux(:,:,2)=zeros(N);
    imagesc(A_aux)
end

%A es una matriz simetrica.
%Desde arriba hacia abajo señales "polarizacion ciruclar" desde freq 0 
%hasta el maximo y luego valores negativos

%% (3)
N=8;
x1=[1 0 0 0 0 0 0 0];
x2=[1 1 1 1 1 1 1 1];
x3=[exp(-1i*2*pi*1/N) exp(-1i*2*pi*2/N) exp(-1i*2*pi*3/N) exp(-1i*2*pi*4/N) exp(-1i*2*pi*5/N) exp(-1i*2*pi*6/N) exp(-1i*2*pi*7/N) exp(-1i*2*pi*8/N)];
x4=[cos(2*pi*1/N) cos(2*pi*2/N) cos(2*pi*3/N) cos(2*pi*4/N) cos(2*pi*5/N) cos(2*pi*6/N) cos(2*pi*7/N) cos(2*pi*8/N)];


for x=[x1' x2' x3' x4']
    x';
    rms(fft(x')-DFTmatrix(x'))^2;
    rms(fft(x')-(dftmtx(N)*x)')^2;
end
%porque usaste RMS? creo q se hace con immse.
immse(fft(x'),DFTmatrix(x'))
immse(fft(x'),(dftmtx(N)*x)') 
immse(fft(x'),DFTsum(x'))

%hacer tabla
%% 4
long=7;
aux=linspace(1,long,long);
N=2.^aux;
%N=5000;
timefft=zeros(1,length(N));
timedft=zeros(1,length(N));
timedftm=zeros(1,length(N));

for i=1:length(N)
    n=linspace(0,N(i)-1,N(i));
    x1=cos(2*pi*100*n/N(i));
    
    tic;
    a=abs(fft(x1,N(i)));
    timefft(i)=toc;
    
    tic;
    b=abs(DFTsum(x1));
    timedft(i)=toc;
    
    tic;
    b=abs((dftmtx(N(i))*x1'));
    timedftm(i)=toc;
    
end
x=linspace(1,length(N));
plot(N,timefft,N,timedft,N,timedftm)
title('Tiempo de procesamiento')
xlabel('Cantidad de puntos')
ylabel('Tiempo(s)')
legend('FFT','DFTsum','DFTmtx')
