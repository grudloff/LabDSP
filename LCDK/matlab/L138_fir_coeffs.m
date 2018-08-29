% L138_FIR_COEFFS.M
% MATLAB function to write FIR filter coefficients
% in format suitable for use in L138 eXperimenter programs
% L138_fir_intr.c and L138_firprn_intr.c
% written by Donald Reay
%

function L138_fir_coeffs(coeff)

coefflen=length(coeff);
fname = input('enter filename for coefficients ','s');
fid = fopen(fname,'wt');
fprintf(fid,'// %s\n',fname);
fprintf(fid,'// this file was generated using ');
fprintf(fid,'function L138_fir_coeffs.m\n');
fprintf(fid,'\n#define N %d\n',coefflen);
fprintf(fid,'\nfloat h[N] = { \n');
% j is used to count coefficients written to current line
% in output file
j=0;
% i is used to count through coefficients
for i=1:coefflen  
% if six coeffs have been written to current line
% then start new line
  if j>5    
    j=0; 
    fprintf(fid,'\n');  
  end  
% if this is the last coefficient then simply write
% its value to the current line
% else write coefficient value, followed by comma
  if i==coefflen
   fprintf(fid,'%2.4E',coeff(i));
  else
    fprintf(fid,'%2.4E,',coeff(i));  
    j=j+1;
  end
end
fprintf(fid,'\n};\n');
fclose(fid);          