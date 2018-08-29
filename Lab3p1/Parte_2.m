%%
%(1)
close all
fs=10000;
for i=[pi/6 pi/3 pi/2]
    figure('Name',strcat('\theta=pi/',num2str(pi/i)),'NumberTitle','off')
    title(strcat('\theta=',i));
    H=tf([1 -2*cos(i) 1],[sin(i) 0 0],1/fs);
    subplot 311
    impulseplot(H/fs)
    h=impulse(H/fs);
    xlim([0 2*2*pi/fs/i]);
    subplot 312
    
    [H,w] = DTFT(h,512);
    plot(w,abs(H)/length(h));
    xlim([-3.1415 3.1415])
    ylabel('Amplitud')
    xlabel('\omega [rad]')
    
    subplot 313
    plot(w,20*log10(abs(H))/length(h));
    xlim([-3.1415 3.1415])
    ylabel('Amplitud [dB]')
    xlabel('\omega [rad]')
    
end

%Es como el Oscilador biquad con theta=w0=2*pi*f/fs
%Es un eliminafrecuencia de w0

%%
%(2)
close all
%fs=10000;
for r=[0.99 0.9 0.7]
    i=pi/3;
    figure('Name',strcat('\theta=pi/',num2str(pi/i),',r=',num2str(r)),'NumberTitle','off')
    title(strcat('\theta=',i));
    H=tf([(1-r) 0 0],[1 -2*cos(i)*r r^2],1/fs);
    subplot 211
    stepplot(H)
    subplot 212
    bodeplot(H)
end

%Es un pasabanda, w0=2*pi*f/fs, el ancho de banda proporcional a r