% L138_LOGFFT.M
% 
% MATLAB function to read (sample) values saved from L138
% memory using CCSv4 and plot FFT magnitude on log scale
% CCSv4 data should be 32-bit floating point saved in
% TI Data Format .dat file
%

function L138_logfft()

fname = input('enter filename ','s');
fs = input('enter sampling frequency in Hz ');
xscale = input('linear (0) or log (1) frequency scale? ');
fid = fopen(fname,'rt');
dummy = fscanf(fid,'%x',4)
N = fscanf(fid,'%x',1);
mydata=fscanf(fid,'%f',N);
fclose(fid);          
g = abs(fft(mydata));
ff = 0:fs/N:(fs/2-fs/N);
figure(1)
plot(ff,20*log10(g(1:(N/2))),'LineWidth',2.0);
grid on
xlabel('frequency (Hz)','FontSize',12,'FontName','times');
ylabel('magnitude (dB)','FontSize',12,'FontName','times');
if xscale == 1
  set(gca,'FontSize',12,'XScale','log','FontName','times');
else
  set(gca,'FontSize',12,'XScale','lin','FontName','times');
end
tt = 0:1/fs:(N-1)/fs;
figure(2)
str = [num2str(N),' sample values read from file'];
disp(str);
PTS = input('enter number of sample values to plot ');
plot(tt(1:PTS),mydata(1:PTS),'LineWidth',2.0);
grid on
xlabel('time (s)','FontSize',12,'FontName','times');
ylabel('sample value','FontSize',12,'FontName','times');
set(gca,'FontSize',12,'FontName','times');
