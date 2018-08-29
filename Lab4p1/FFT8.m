function X=FFT8(x)
N=8;
Wn=W(N);
xeven=x(1:2:end);
xodd=x(2:2:end);
X4even=FFT4(xeven);
X4odd=FFT4(xodd);
X(1)=X4even(1)+X4odd(1);
X(2)=X4even(2)+Wn*X4odd(2);
X(3)=X4even(3)+Wn^2*X4odd(3);
X(4)=X4even(4)+Wn^3*X4odd(4);
X(5)=X4even(1)-X4odd(1);
X(6)=X4even(2)-Wn*X4odd(2);
X(7)=X4even(3)-Wn^2*X4odd(3);
X(8)=X4even(4)-Wn^3*X4odd(4);

end