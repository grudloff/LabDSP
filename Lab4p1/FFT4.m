function X=FFT4(x)
N=4;
Wn=W(N);
xeven=x(1:2:end);
xodd=x(2:2:end);
X2even=FFT2(xeven);
X2odd=FFT2(xodd);
X(1)=X2even(1)+X2odd(1);
X(2)=X2even(2)+Wn*X2odd(2);
X(3)=X2even(1)-X2odd(1);
X(4)=X2even(2)-Wn*X2odd(2);
end