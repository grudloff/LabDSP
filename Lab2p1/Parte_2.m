%%
% (1)
close all
info=audioinfo('gtr-jazz.wav');
[x,Fs]=audioread('gtr-jazz.wav');
t=0:1/Fs:info.Duration;
t = t(1:end-1);

a=0.3;b=0.05;G=1;
%a=0.2;b=0.05;G=1;
%a=0.1;b=0.05;G=1;
y=overload(x,a,b,G);

subplot 211
plot(t,x)
xlabel('Tiempo')
ylabel('x')

soundsc(x,Fs);
pause(10)

subplot 212
plot(t,y)
xlabel('Tiempo')
ylabel('overload(x)')

soundsc(y,Fs);

figure
a=0.6;b=0.05;G=1;
plot(t,sin(t));
hold on
plot(t,overload(sin(t),a,b,G));
xlabel('Tiempo')
ylabel('x y overload(x)')
%%
% (2)
b=0.3;
N=4;
M=0.125;
y=delay_v1(x,Fs,N,M,b);%b

%N=10
%M=0.250;
%y=delay_v2(x,Fs,N,M,b);%b^k

sound(x,Fs);
pause(6);
subplot 211
plot(t,x(:,1))
xlabel('Tiempo')
ylabel('x')

subplot 212
plot(t,y(:,1))
xlabel('Tiempo')
ylabel('delay(x)')

sound(y,Fs);
%%
%(3)
P=1
Q=3;
Fs
x_d=resample(x,P,Q);
Fs_d=P/Q*Fs

b=0.4;
N=4;
M=0.250;
y=delay_v1(x_d,Fs_d,N,M,b);%b


pause(6)
soundsc(y,Fs_d)

%%
%(4)
[x,Fs]=audioread('gtr-jazz.wav');
x=x(:,1);
size_x=size(x);
size_x=size_x(:,1);

wet=0.3;
F=0.5;
D=0.001;
M=0.02;
y=chorus_v2(x,Fs,wet,F,D,M);

soundsc(y,Fs)
