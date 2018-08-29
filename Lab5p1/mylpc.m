function a=mylpc(x,p)
    aux=xcorr(x,p);
    rx=aux(p+2:end);
    
    Rx=toeplitz(aux(p+1:end-1));
    a=Rx\rx; %inv(Rx)*rx
    a=[1;-a]';
end