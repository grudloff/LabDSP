function y=DFTsum(x)
N=length(x);
y=zeros(1,N);
for k=1:N
    sum=0;
    for p=0:N-1
        sum=sum+x(p+1)*exp((-1i*2*pi*(k-1)*p)/N);
    end
    y(k)=sum;
end
