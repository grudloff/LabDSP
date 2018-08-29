load('test_training_signals.mat')
n=25/1000*fs; %200
[VUS,rms_v,A]=P4(test_signal,n,15);
plot(test_signal);
hold on
scatter(linspace(0,length(test_signal)-1,79),VUS)
plot(linspace(0,length(test_signal)-1,79),rms_v)

x1=exciteV(16000,200);
%%Sintesis de voz
sintesis=zeros(1,length(test_signal));
for i=1:length(VUS)
    if VUS(i)==1
        x=x1(200*(i-1)+1:200*(i-1)+200);
        sintesis(200*(i-1)+1:200*(i-1)+200)=filter(1,A(i,:),x);
        sintesis(200*(i-1)+1:200*(i-1)+200)=rms_v(i)*sintesis(200*(i-1)+1:200*(i-1)+200)/rms(sintesis(200*(i-1)+1:200*(i-1)+200));
    elseif VUS(i)==-1
        x=rand(1,n)-0.5;
        sintesis(200*(i-1)+1:200*(i-1)+200)=filter(1,A(i,:),x);
        sintesis(200*(i-1)+1:200*(i-1)+200)=rms_v(i)*sintesis(200*(i-1)+1:200*(i-1)+200)/rms(sintesis(200*(i-1)+1:200*(i-1)+200));
    else
        sintesis(200*(i-1)+1:200*(i-1)+200)=zeros(1,n);
    end
end
soundsc(sintesis,fs)
audiowrite('Pregunta4_3.wav',sintesis,fs)

%% 4) Deteccion de fundamental 

clear
close all
clc
load('test_training_signals.mat')
n=25/1000*fs; %200
[VUS,rms_v,A]=P4(test_signal,n,15);
%plot(test_signal);
hold on
%scatter(linspace(0,length(test_signal)-1,79),VUS)
%plot(linspace(0,length(test_signal)-1,79),rms_v)

%x1=exciteV(16000,200);
%%Sintesis de voz
sintesis=zeros(1,length(test_signal));
for i=1:length(VUS)
    if VUS(i)==1
        np=ceil(8000*(1/max_frec(test_signal(200*(i-1)+1:200*(i-1)+200))))
        x1=exciteV(16000,np)-mean(exciteV(16000,np));
        
        %bias=ceil(20*rand());
        bias=0;
        x=x1(200*(i-1)+1+bias:200*(i-1)+200+bias);
        sintesis(200*(i-1)+1:200*(i-1)+200)=filter(1,A(i,:),x);
        sintesis(200*(i-1)+1:200*(i-1)+200)=rms_v(i)*sintesis(200*(i-1)+1:200*(i-1)+200)/rms(sintesis(200*(i-1)+1:200*(i-1)+200));
    elseif VUS(i)==-1
        x=rand(1,n)-0.5;
        sintesis(200*(i-1)+1:200*(i-1)+200)=filter(1,A(i,:),x);
        sintesis(200*(i-1)+1:200*(i-1)+200)=rms_v(i)*sintesis(200*(i-1)+1:200*(i-1)+200)/rms(sintesis(200*(i-1)+1:200*(i-1)+200));
    else
        sintesis(200*(i-1)+1:200*(i-1)+200)=zeros(1,n);
    end
end
plot(sintesis)
soundsc(sintesis,fs)
audiowrite('Pregunta4_4.wav',sintesis,fs)

