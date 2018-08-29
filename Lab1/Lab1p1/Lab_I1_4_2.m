%Parte IV 2) con h=4
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
plot(t,x_c(1,:))
hold on
plot(t,m)
title('modulación FM, h=4')
xlabel('tiempo [ms]')
ylabel('amplitud')
hold off





