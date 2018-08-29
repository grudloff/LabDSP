% L138_IIR_INT_COEFFS.M
% INTEGER COEFFICIENT VERSION
% MATLAB function to write SOS IIR filter coefficients
% in format suitable for use in L138 eXperimenter program
% L138_iir_intr.c
% assumes that coefficients have been exported from
% fdatool as two matrices
% first matrix has format
% [ b10 b11 b12 a10 a11 a12
%   b20 b21 b22 a20 a21 a22
%   ...
% ] 
% where bij is the bj coefficient in the ith stage
% second matrix contains gains for each stage
%
function L138_iir_int_coeffs(coeff,gain)
%
num_sections=length(gain)-1;
fname = input('enter filename for coefficients ','s');
fid = fopen(fname,'wt');
fprintf(fid,'// %s\n',fname);
fprintf(fid,'// this file was generated automatically using function L138_iir_int_coeffs.m\n',fname);
fprintf(fid,'\n#define NUM_SECTIONS %d\n',num_sections);
% first write the numerator coefficients b
% i is used to count through sections
fprintf(fid,'\nint b[NUM_SECTIONS][3] = { \n');
for i=1:num_sections
  if i==num_sections
      fprintf(fid,'{%d, %d, %d} };\n',round(coeff(i,1)*gain(i)*2^15),round(coeff(i,2)*gain(i)*2^15),round(coeff(i,3)*gain(i)*2^15));
  else
      fprintf(fid,'{%d, %d, %d},\n',round(coeff(i,1)*gain(i)*2^15),round(coeff(i,2)*gain(i)*2^15),round(coeff(i,3)*gain(i)*2^15));
  end
end
% then write the denominator coefficients a
% i is used to count through sections
fprintf(fid,'\nint a[NUM_SECTIONS][3] = { \n');
for i=1:num_sections
  if i==num_sections
      fprintf(fid,'{%d, %d, %d} };\n',round(coeff(i,4)*2^15),round(coeff(i,5)*2^15),round(coeff(i,6)*2^15));
  else
      fprintf(fid,'{%d, %d, %d},\n',round(coeff(i,4)*2^15),round(coeff(i,5)*2^15),round(coeff(i,6)*2^15));
  end
end
fclose(fid);          
