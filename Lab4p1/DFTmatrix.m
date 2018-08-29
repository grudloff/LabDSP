function X=DFTmatrix(x)
    N=length(x);
    A=DFTmatrixA(N);
    X=(x*A);
end