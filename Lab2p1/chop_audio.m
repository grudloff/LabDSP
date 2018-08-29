function chop_audio(old_name,new_name)
    info_y=audioinfo(old_name);
    [y,Fs]=audioread(old_name);

    t=0:1/Fs:info_y.Duration;
    t = t(1:end-1);
    
    a=figure;
    plot(t,y)
    x=ginput(2);
    x=sort(x);
    x(1)=max(0,x(1));
    x(2)=min(t(end),x(2));
    y=y(x(1)*Fs:x(2)*Fs);
    audiowrite(new_name,y,Fs)
    close (a.Number)
  
