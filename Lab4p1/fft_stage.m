function X=fft_stage(x)
    N=length(x)
    if(N==2)
        X=FFT2(x);
        return
    end
    xeven=x(1:2:end);
    xodd=x(2:2:end);
    Xeven=fft_stage(xeven);
    Xodd=fft_stage(xodd);
    Wn=W(N);
    X=zeros(1,N);
    for k=1:N/2
        X(k)=Xeven(k)+Wn^(k-1)*Xodd(k);
        X(k+N/2)=Xeven(k)-Wn^(k-1)*Xodd(k);
    end
end