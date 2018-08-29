% L138_PLOT_COMPLEX.M
% 
% MATLAB function to plot real and imaginary parts of
% complex floating point values read from L138 memory
% and written to TI Data Format .dat file using CCSv4.
%

function L138_plot_complex()

fname = input('enter filename ','s');
fid = fopen(fname,'rt');
dummy = fscanf(fid,'%x',4)
N = fscanf(fid,'%x',1)
memdata=fscanf(fid,'%f',N);
fclose(fid);          
figure;
set(gcf,'numberTitle','off')
set(gcf,'name','complex data read from L138 memory in CCSv4')
subplot(2,1,1);
plot(0:(N/2-1),memdata(1:2:N),'LineWidth',2.0);
grid on
xlabel('n','FontSize',12,'FontName','times');
ylabel('real','FontSize',12,'FontName','times');
subplot(2,1,2);
plot(0:(N/2-1),memdata(2:2:N),'LineWidth',2.0);
grid on
xlabel('n','FontSize',12,'FontName','times');
ylabel('imaginary','FontSize',12,'FontName','times');
