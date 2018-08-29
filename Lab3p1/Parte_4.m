%%
%(1)
close all
M=512;
wc=2*pi/3
i=0;
for N=[21 101 1001]
    i=i+1;
    n=0:(N-1);
    h=wc/pi*sinc(wc/pi*(n-(N-1)/2));
    [H,w]=DTFT(h,512);
%     figure()
    subplot(2,3,i)
    plot(w,abs(H))
    ylabel('Magnitud')
    title(strcat('N=',num2str(N)))
    xlim([-pi pi]);
    subplot(2,3,i+3)
    plot(w,angle(H))
    ylabel('Fase')
    xlim([-pi pi]);
    
end

