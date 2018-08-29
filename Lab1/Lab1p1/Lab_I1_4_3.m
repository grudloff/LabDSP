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
title('modulación AM, h=0.5')
xlabel('tiempo [ms]')
ylabel('amplitud')
subplot(4,1,1)
plot(t,m)
title('Señal modulante')
xlabel('tiempo [ms]')
ylabel('amplitud')
subplot(4,1,2)
plot(t,x_c./[1+h.*m])
title('Señal portadora')
xlabel('tiempo [ms]')
ylabel('amplitud')

%%
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
x_c=zeros(1,p(2));
sum=0;
t_m=tiempo/muestras;%ms
for i=1:p(2)
    sum=sum+(m(i)*t_m);
    x_c(1,i)=A_c.*cos((2.*pi.*F_c.*t(i))+w_d*sum);
end
subplot(4,1,4)
plot(t,x_c(1,:))
title('modulación FM, h=4')
xlabel('tiempo [ms]')
ylabel('amplitud')
hold off