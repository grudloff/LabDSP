function y=max_frec(x)
X=fft(x);
fs=8000;
bin=fs/(length(x)/2);
i_i=ceil(50/bin);
i_s=ceil(150/bin);
[C,I]=max(X(i_i:i_s));
y=(I+i_i)*bin;
