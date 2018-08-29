function X=DFTdc(x)
    N=length(x);
    xeven=x(1:2:end);
    xodd=x(2:2:end);
    Xeven=DFTsum(xeven);
    Xodd=DFTsum(xodd);
    X=zeros(1,N);
    for k=1:N/2
        X(k)=Xeven(k)+exp(-1i*2*pi*(k-1)/N)*Xodd(k);
        X(k+N/2)=Xeven(k)-exp(-1i*2*pi*(k-1)/N)*Xodd(k);
    end
end