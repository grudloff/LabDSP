%%
%(1)
close all
load('nspeech.mat');
x=nspeech;
[X,w] = DTFT(x,0);
X=X/length(X);

plot(w,abs(X));
ylabel('Amplitud')
xlabel('\omega [rad]')
xlim([-pi pi])
%soundsc(x,fs);% es un SOL#6 (1661,22Hz)
%pause(3)
%w0=1.323;
%H=tf([1 -2*cos(w0) 1],[sin(w0) 0 0],1/fs);
%[y,t]=lsim(H,x);
%Filtro convolución con h
[y,t]=FIR_filter(x,fs);
figure
subplot 231
plot(t,y);
ylabel('FIR\_filter()')
xlim([0.1 0.3])
xlabel('[seg]')
length(y)
soundsc(y,fs) %please get rid of this pitch??
[Y,w] = DTFT(y,0);
Y=abs(Y)/length(Y);
w=w*fs/(2*pi);
subplot 232
plot(w,Y);
ylabel('Amplitud')
xlabel('[rad/seg]')
xlim([0 fs/2])
subplot 233
Y=20*log10(Y);
plot(w,Y);
ylabel('Amplitud [dB]')
xlabel('[Hz]')
xlim([0 fs/2])
pause(3)

%filtro comando filter
w0=1.323;
y=filter([1 -2*cos(w0) 1],[sin(w0) 0 0],x);
subplot 234
t=1:1:length(y);
t=t/fs;
plot(t,y);
ylabel('filter()')
xlim([0.1 0.3])
xlabel('[seg]')
length(y)
soundsc(y,fs)
[Y,w] = DTFT(y,0);
Y=abs(Y)/length(Y);
w=w*fs/(2*pi);
subplot 235


plot(w,Y);
ylabel('Amplitud')
xlabel('[Hz]')
xlim([0 fs/2])
subplot 236

Y=20*log10(Y);
plot(w,Y);
ylabel('Amplitud[dB]')
xlabel('[Hz]')
xlim([0 fs/2])

% n filter y n+m el hecho por nosotros

%%
%(2)
close all
load('pcm.mat');
x=pcm;
[X,w] = DTFT(x,0);
X=X/length(X);

%plot(w,X);
%xlim([0 pi])

[y,t]=IIR_filter(x,fs);
subplot 221
plot(t,y);
ylabel('IIR\_filter()')
xlim([0 0.5])
xlabel('[seg]')
length(y)
soundsc(y,fs) %please get rid of this pitch??
[Y,w] = DTFT(y,0);
Y=Y/length(Y);
subplot 222
w=w*fs/(2*pi);
Y=20*log10(Y);
plot(w,Y);
ylabel('[dB]')
xlabel('[rad/seg]')
xlim([0 fs/2])
pause(3)

%filtro comando filter
w0=2.471;
r=0.99;
y=filter([(1-r) 0 0],[1 -2*cos(w0)*r r^2],x);
subplot 223
t=1:1:length(y);
t=t/fs;
plot(t,y);
ylabel('filter()')
xlim([0 0.5])
xlabel('[seg]')
length(y)
soundsc(y,fs)
[Y,w] = DTFT(y,0);
Y=Y/length(Y);
subplot 224
w=w*fs/(2*pi);
Y=20*log10(Y);
plot(w,Y);
ylabel('[dB]')
xlabel('[rad/seg]')
xlim([0 fs/2])