%%
%(1)
clc
clear
close all

N=8;
for i=1:N
    if(i==1) x1(i)=1; else x1(i)=0; end
    x2(i)=1;
    x3(i)=exp(-1i*2*pi*i/N);
end
x=[x1;x2;x3];

REPS=100000;
for i=1:3
    figure
    display(strcat('100 iteraciones DFTdc para i=',num2str(i)))
    tic
    for k=1:REPS
        X=DFTdc(x(i,:));
    end
    t1=toc/REPS
    subplot 211
    plot(fftshift(abs(X/N)))
    ylabel(strcat('DFTdc(x_',num2str(i),')'))
    
    display(strcat('100 iteraciones fft para i=',num2str(i)))
    tic 
    for k=1:REPS
        X=fft(x(i,:));
    end
    t2=toc/REPS
    subplot 212
    plot(fftshift(abs(X/N)))
    ylabel(strcat('fft(x_',num2str(i),')'))
    t1/t2
end

%%
clc
close all
%(2)

for i=1:3
    figure
    display(strcat('100 iteraciones FFT8 para i=',num2str(i)))
    tic
    for k=1:REPS
        X=FFT8(x(i,:));
    end
    t1=toc/REPS
    subplot 211
    plot(fftshift(abs(X/N)))
    ylabel(strcat('FFT8(x_',num2str(i),')'))
    
    display(strcat('100 iteraciones fft para i=',num2str(i)))
    tic 
    for k=1:REPS
    X=fft(x(i,:));
    end
    t2=toc/REPS
    subplot 212
    plot(fftshift(abs(X/N)))
    ylabel(strcat('fft(x_',num2str(i),')'))
    t1/t2
end

%% (3_1)


%X=fft_stage(x)

%ACA COPIE LO DE ARRIBA PA VER SI FUNCIONA

clear
close all

N=8;
for i=1:N
    if(i==1) x1(i)=1; else x1(i)=0; end
    x2(i)=1;
    x3(i)=exp(-1i*2*pi*i/N);
end
x=[x1;x2;x3];

REPS=100;
for i=1:3
    figure
    display(strcat('100 iteraciones FFT_stage para i=',num2str(i)))
    tic
    for k=1:REPS
        X=fft_stage(x(i,:));
    end
    t1=toc/REPS
    subplot 211
    plot(fftshift(abs(X/N)))
     ylabel(strcat('fft_stage(x_',num2str(i),')'))
    
    display(strcat('100 iteraciones fft para i=',num2str(i)))
    tic 
    for k=1:REPS
    X=fft(x(i,:));
    end
    t2=toc/REPS
    subplot 212
    plot(fftshift(abs(X/N)))
    ylabel(strcat('fft(x_',num2str(i),')'))
    t1/t2
end
%% 3_2
long=6;
aux=linspace(1,long,long);
N=2.^aux;
%N=5000;
timefft=zeros(1,length(N));
timeffts=zeros(1,length(N));

for i=1:length(N)
    n=linspace(0,N(i)-1,N(i));
    x1=cos(2*pi*100*n/N(i));
    
    tic;
    a=abs(fft(x1,N(i)));
    timefft(i)=toc;
    
    tic;
    b=abs(fft_stage(x1));
    timeffts(i)=toc;
    
end
x=linspace(1,length(N));
plot(N,timefft,N,timeffts)
title('Tiempo de procesamiento')
xlabel('Cantidad de puntos')
ylabel('Tiempo(s)')
legend('fft()','fft\_stage()')


