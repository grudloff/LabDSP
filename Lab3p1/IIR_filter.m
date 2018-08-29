function [y,t]=IIR_filter(x,fs)
    r=0.99;
    w0=2.471;
    y(1)=(1-r)*x(1);
    y(2)=(1-r)*x(2)+2*cos(w0)*r*y(1);
    n=length(x);
    for i=3:n
        y(i)=(1-r)*x(i)+2*cos(w0)*r*y(i-1)-r^2*y(i-2);
    end
    t=1:n;
    t=t/fs;
end