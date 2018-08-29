function h(n,N,wc)
    if (n>=0&&N<=(N-1))
        return wc/pi*sinc(wc/pi*(n-(N-1)/2));
    end
    return 0;
end
        