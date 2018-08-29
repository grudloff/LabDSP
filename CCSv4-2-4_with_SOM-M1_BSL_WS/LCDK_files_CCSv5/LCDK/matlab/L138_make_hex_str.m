% L138_MAKE_HEX_STR.M
%
% function to convert integer value in range -32768 to 32767
% into 4-digit hex string.
%

function hex_str = l138_make_hex_str(value)
if value < 0
    hex_str = sprintf('%04x',value+2^16);
else
    hex_str = sprintf('%04x',value);
end

