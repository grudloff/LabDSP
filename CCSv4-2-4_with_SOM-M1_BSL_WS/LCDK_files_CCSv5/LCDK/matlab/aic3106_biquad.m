% AIC3106_BIQUAD.M
%
% MATLAB function to write C program statements
% to program left and right channel biquads in AIC3106 codec.
% Assumes that coefficients of a fourth order IIR filter
% have been designed using fdatool and exported to workspace
% as two matrices. These are passed to function as coeff and
% gain.
%
% First matrix coeff has format
%
% [ b10 b11 b12 a10 a11 a12
%   b20 b21 b22 a20 a21 a22
% ] 
%
% where bij is the bj coefficient in the ith stage.
%
% Second matrix gain contains gains for the two stages of the
% fourth order filter. fdatool generates three gains - one for
% each second order stage and an output gain. 
%
% This function calls function l138_make_hex_str() defined in
% file L138_MAKE_HEX_STR.M.
%
% Details of AIC3106 registers in TI document SLAS509E.
%
function aic3106_biquad(coeff,gain)
%
fname = input('enter filename for C program statements ','s');
fid = fopen(fname,'wt');
fprintf(fid,'// %s\n',fname);
fprintf(fid,'// this file was generated automatically using');
fprintf(fid,' function aic3106_biquad.m\n',fname);
fn_name = 'AIC3106_writeRegister';
% left channel audio effects filter coefficients
a = l138_make_hex_str(round(coeff(1,1)*gain(1)*(2^15 - 1)));
fprintf(fid,'%s( 1,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s( 2,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(1,2)*gain(1)*2^14));
fprintf(fid,'%s( 3,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s( 4,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(1,3)*gain(1)*(2^15 - 1)));
fprintf(fid,'%s( 5,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s( 6,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,1)*gain(2)*(2^15 - 1)));
fprintf(fid,'%s( 7,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s( 8,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,2)*gain(2)*2^14));
fprintf(fid,'%s( 9,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(10,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,3)*gain(2)*(2^15 - 1)));
fprintf(fid,'%s(11,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(12,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(1,5)*2^14));
fprintf(fid,'%s(13,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(14,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(1,6)*(2^15 - 1)));
fprintf(fid,'%s(15,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(16,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(2,5)*2^14));
fprintf(fid,'%s(17,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(18,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(2,6)*(2^15 - 1)));
fprintf(fid,'%s(19,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(20,0x%s%s);\n',fn_name,a(3),a(4));
% right channel audio effects filter coefficients  
a = l138_make_hex_str(round(coeff(1,1)*gain(1)*(2^15 - 1)));
fprintf(fid,'%s(27,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(28,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(1,2)*gain(1)*2^14));
fprintf(fid,'%s(29,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(30,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(1,3)*gain(1)*(2^15 - 1)));
fprintf(fid,'%s(31,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(32,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,1)*gain(2)*(2^15 - 1)));
fprintf(fid,'%s(33,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(34,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,2)*gain(2)*2^14));
fprintf(fid,'%s(35,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(36,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(coeff(2,3)*gain(2)*(2^15 - 1)));
fprintf(fid,'%s(37,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(38,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(1,5)*2^14));
fprintf(fid,'%s(39,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(40,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(1,6)*(2^15 - 1)));
fprintf(fid,'%s(41,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(42,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(2,5)*2^14));
fprintf(fid,'%s(43,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(44,0x%s%s);\n',fn_name,a(3),a(4));
a = l138_make_hex_str(round(-coeff(2,6)*(2^15 - 1)));
fprintf(fid,'%s(45,0x%s%s);\n',fn_name,a(1),a(2));
fprintf(fid,'%s(46,0x%s%s);\n',fn_name,a(3),a(4));  
fclose(fid);
