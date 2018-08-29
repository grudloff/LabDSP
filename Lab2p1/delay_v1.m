function y=delay_v1(x,Fs,N,M,b)
    y=x;
    for j=1:length(x);
        for i=1:N
            Delay=i*M*Fs;
            if((j-Delay)>0)
                y(j)=y(j)+b*x(j-Delay);
            end
        end
    end
end