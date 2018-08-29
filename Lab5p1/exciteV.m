function X=exciteV(N,Np)
    X=zeros(1,N);
    for i=1:Np:N
        X(i)=1;
    end
end