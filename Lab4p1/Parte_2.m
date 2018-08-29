%%
%(1)
close all
w0=2*pi*100;
fs=5000;
Ts=1/fs;



N=500;
w=-pi:0.001:pi;
X1=zeros(1,length(w));
X2=zeros(1,length(w));
for i=1:length(w)
    for n=1:N
        X1(i)=X1(i)+sin(w0*Ts*n)*exp(-1i*w(i)*n);
        X2(i)=X2(i)+cos(w0*Ts*n)*exp(-1i*w(i)*n);
    end
end

subplot 121
plot(w,mag2db(abs(X1)));
title('X1')
xlim([-pi,pi]);
ylabel('mag[db]')
xlabel('\omega[rad/seg]')
ylim([-80,50]);
hold on
subplot 122
plot(w,mag2db(abs(X2)));
title('X2')
xlim([-pi,pi]);
xlabel('\omega[rad/seg]')
ylim([-80,50]);
hold on

w_inf=[-w0*Ts,w0*Ts];
X1_inf=[-pi/1i,pi/1i];%1i=j
X2_inf=[pi,pi];
subplot 121
stem(w_inf,mag2db(abs(X1_inf)));
hold on
subplot 122
stem(w_inf,mag2db(abs(X2_inf)));


%%
close all
%(2)
N=500;
n=1:N;
x1=sin(w0*Ts*n);
x2=cos(w0*Ts*n);
N=4096;
X1=fft(x1,N)/500*2*pi; %se divide por el largo de la muestra y multiplica por 2pi.
X2=fft(x2,N)/500*2*pi;

f = (1-(N/2):(N/2))/N;%freq normalizada
subplot 221
plot(f,abs(X1));
ylabel('Amplitud')
title('(a)')
subplot 223
plot(f,abs(X2));
ylabel('Amplitud')
xlabel('[-f_s/2,f_s/2]')

X1=[X1(N/2+1:end),X1(1:N/2)];
X2=[X2(N/2+1:end),X2(1:N/2)];
f = (0:(N-1))/N;%freq normalizada

subplot 222
plot(f,abs(X1));
title('(b)')
subplot 224
plot(f,abs(X2));
xlabel('[0,f_s]')

%%
%(3)
i=0;
figure(1)
for N=[256,500,2048]
    i=i+1;
    X1=fft(x1,N)/N*2*pi;
    w = 2*pi*(1-(N/2):(N/2))/N;
    
    subplot(3,2,2*i-1)
    plot(w,abs(X1))
    ylabel(strcat('N=',num2str(N)))
    if i==1
        title('Magnitud')
    end
    xlim([-pi,pi]);
    subplot(3,2,2*i)
    plot(w,real(X1))
    hold on
    plot(w,imag(X1))
    if i==1
        title('Parte Real & Imaginaria')
    end
    xlim([-pi,pi]);
end

figure(2)
i=0;
for N=[256,500,2048]
    i=i+1;
    X2=fft(x2,N)/N*2*pi;
    w = 2* pi*(1-(N/2):(N/2))/N;
    
    subplot(3,2,2*i-1)
    plot(w,abs(X2))
    ylabel(strcat('N=',num2str(N)))
    if i==1
        title('Magintud')
    end
    xlim([-pi,pi]);
    subplot(3,2,2*i)
    plot(w,real(X2))
    hold on
    plot(w,imag(X2))
    if i==1
        title('Parte Real & Imaginaria')
    end
    xlim([-pi,pi]);
end
