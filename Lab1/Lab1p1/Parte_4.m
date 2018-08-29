%%
close all
%Parte IV 1) con h=0.5
F_c=500;%Hz
F_m=50;%Hz
h=0.5;
A_c=1/(1+h);%Amplitud portadora
muestras=1000;%Fs=20000=4*500
tiempo=50;%ms%50*1e-3;%s
t=linspace(0,tiempo,muestras);
m=sin(2.*pi.*F_m.*t/1000);
x_c=A_c.*[1+h.*m].*cos(2.*pi.*F_c.*t/1000);
plot(t,x_c)
hold on
plot(t,m)
title('Modulación AM, h=0.5')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
hold off

%%
%Parte IV 1) con h=0.8
close all
h=0.8;
A_c=1/(1+h);
m=sin(2.*pi.*F_m.*t/1000);
x_c=A_c.*[1+h.*m].*cos(2.*pi.*F_c.*t/1000);
figure
plot(t,x_c)
hold on
plot(t,m)
title('Modulación AM, h=0.8')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
hold off

%%
close all
%Parte IV 2) con h=4
F_c=500/1000;%1/ms
F_m=50/1000;%1/ms
w_m=F_m*2*pi;
w_d=4*w_m;
h=w_d/w_m;
A_c=1;%Amplitud portadora

muestras=1000; %Fs=20000=4*500
tiempo=50;%ms%50*1e-3;%s
t=linspace(0,tiempo,muestras);
m=sin(2.*pi.*F_m.*t);
p=size(t);
x_c=zeros(p(2),1);
% sum=0;
t_m=tiempo/muestras;%ms
% for i=1:p(2)
%     sum=sum+(m(i)*t_m);
%     x_c(i)=A_c.*cos((2.*pi.*F_c.*t(i))+w_d*sum);
% end
x_c=A_c.*cos((2.*pi.*F_c.*t)+w_d*cumtrapz(m)*t_m);

plot(t,x_c)
hold on
plot(t,m)
title('Modulación FM, h=4')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
hold off

%%
%Parte IV 3) AM con h=0.5
F_c=500;%Hz
F_m=50;%Hz
h=0.5;
A_c=1/(1+h);%Amplitud portadora
muestras=1000;
tiempo=50;%ms%50*1e-3;%s
t=linspace(0,tiempo,muestras);
m=sin(2.*pi.*F_m.*t/1000);
x_c=A_c.*[1+h.*m].*cos(2.*pi.*F_c.*t/1000);
subplot(4,1,3)
plot(t,x_c)
title('Modulación AM, h=0.5')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
subplot(4,1,1)
plot(t,m)
title('Señal modulante')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
subplot(4,1,2)
plot(t,x_c./[1+h.*m])
title('Señal portadora')
xlabel('Tiempo [ms]')
ylabel('Amplitud')


clear
%Parte IV 3) FM con h=4
F_c=500/1000;%1/ms
F_m=50/1000;%1/ms
w_m=F_m*2*pi;
w_d=4*w_m;
h=w_d/w_m;
A_c=1;%Amplitud portadora

muestras=1000;
tiempo=50;%ms%50*1e-3;%s
t=linspace(0,tiempo,muestras);
m=sin(2.*pi.*F_m.*t);
p=size(t);
x_c=zeros(p(2),1);
% sum=0;
t_m=tiempo/muestras;%ms
% for i=1:p(2)
%     sum=sum+(m(i)*t_m);
%     x_c(1,i)=A_c.*cos((2.*pi.*F_c.*t(i))+w_d*sum);
% end
x_c=A_c.*cos((2.*pi.*F_c.*t)+w_d*cumtrapz(m)*t_m);
subplot(4,1,4)
plot(t,x_c)
title('Modulación FM, h=4')
xlabel('Tiempo [ms]')
ylabel('Amplitud')
hold off
%%
close all
%-----------(4)-------------
%Para este punto se debe correr Parte_3.m
%Hay que hacer esto pa todos y hacer una tabla
V={'Señal','Energia','RMS'};
[e,rms]=e_rms(s1_1,1/fs_1);
V=[V;{'S1_1',e,rms}];
[e,rms]=e_rms(s1_2,1/fs_1);
V=[V;{'S1_2',e,rms}];
[e,rms]=e_rms(s2_2,1/fs_1);
V=[V;{'S2_2',e,rms}];
[e,rms]=e_rms(s_3a,1/fs_1);
V=[V;{'S_3a',e,rms}];
[e,rms]=e_rms(s_3b,1/fs_1);
V=[V;{'S_3b',e,rms}];
[e,rms]=e_rms(s_3c,1/fs_1);
V=[V;{'S_3c',e,rms}];
[e,rms]=e_rms(s_4a,1/fs_4a);
V=[V;{'S_4a',e,rms}];
[e,rms]=e_rms(s_r_4a,1/fs_4a);
V=[V;{'S_r_4a',e,rms}];
[e,rms]=e_rms(s_4b,1/fs_4b);
V=[V;{'S_4b',e,rms}];
[e,rms]=e_rms(s_r_4b,1/fs_4b);
V=[V;{'S_r_4b',e,rms}];
[e,rms]=e_rms(s_4c,1/fs_4c);
V=[V;{'S_4c',e,rms}];
[e,rms]=e_rms(s_r_4c,1/fs_4c);
V=[V;{'S_r_4c',e,rms}];
[e,rms]=e_rms(s_5,1/fs_5);
V=[V;{'S_5',e,rms}];
[e,rms]=e_rms(s_r_5,1/fs_5);
V=[V;{'S_r_5',e,rms}];
[e,rms]=e_rms(s_6,1/fs_6);
V=[V;{'S_6',e,rms}];
[e,rms]=e_rms(s_r_6,1/fs_6);
V=[V;{'S_r_6',e,rms}];
xlswrite('EyRMS.xls',V)