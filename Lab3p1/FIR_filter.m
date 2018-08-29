function [y,t]=FIR_filter(x,fs)
    w0=1.323;
    h=[1 -2*cos(w0) 1]/sin(w0);
    y=[];
    n=length(x);
    m=length(h);
    x=[x, zeros(1,m)];
    h=[h,zeros(1,n)];
    h=h(end:-1:1);
    for i=1:(n+m)
        y(i)=x(1:i)*h(end-i+1:end)';
    end
    t=1:(n+m);
    t=t/fs;
end
