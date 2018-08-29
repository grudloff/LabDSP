function y=chorus(x,Fs,wet,F,D,M)
    size_x=size(x);
    size_x=size_x(:,1);
    y=wet*x;
    for i=1:size_x
        D_real=ceil(M*Fs+D*Fs*sin(2*pi*i/Fs*F));
        if(i-D_real>0)
            y(i)=(1-wet)*x(i)+wet*x(i-D_real);
        end
    end
end