%%
N=8;
x1=[1 0 0 0 0 0 0 0];
x2=[1 1 1 1 1 1 1 1];
x3=[exp(-1i*2*pi*1/N) exp(-1i*2*pi*2/N) exp(-1i*2*pi*3/N) exp(-1i*2*pi*4/N) exp(-1i*2*pi*5/N) exp(-1i*2*pi*6/N) exp(-1i*2*pi*7/N) exp(-1i*2*pi*8/N)];
x4=[cos(2*pi*1/N) cos(2*pi*2/N) cos(2*pi*3/N) cos(2*pi*4/N) cos(2*pi*5/N) cos(2*pi*6/N) cos(2*pi*7/N) cos(2*pi*8/N)];
x=[x1' x2' x3' x4'];
for k=1:4
    subplot(4,2,2*k)
    stem(abs(fftshift(fft(x(:,k),N)))/N)
    if (2*k==2) title('FFT'); end
    subplot(4,2,2*k-1)
    stem(abs(fftshift(DFTsum(x(:,k))))/N)
    if (2*k-1==1) title('DFTsum'); end 
end 
%% 3
long=150;
N=linspace(2,long);
N=5000;
timefft=zeros(1,length(N));
timedft=zeros(1,length(N));
if (N==5000)
    n=linspace(0,N-1,N);
    x1=cos(2*pi*100*n/N);
    a=abs(fft(x1(1:500)));
    b=abs(DFTsum(x1(1:500)));
    e=a-b;
    figure
    plot(e)
    title('Error FFT y DFTsum')
    xlabel('Muestras')
else
    for i=1:length(N)
        n=linspace(0,N(i)-1,N(i));
        x1=cos(2*pi*100*n/N(i));
        tic;
        a=abs(fft(x1,N(i)));
        timefft(i)=toc;
        tic;
        b=abs(DFTsum(x1));
        timedft(i)=toc;
    end
    x=linspace(1,length(N));
    plot(x,timefft,x,timedft)
    title('Tiempo de procesamiento')
    xlabel('Cantidad de puntos')
    ylabel('Tiempo(s)')
    legend('FFT','DFTsum')
    
end

