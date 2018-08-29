function n=zero_crossing(x)
    n=0;
    for i=1:(length(x)-1)
        if(x(i)*x(i+1)<0)
            n=n+1;
        end
    end
end