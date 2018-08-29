function [e,rms]=e_rms(S,dt)
    e=trapz(S.^2)*dt;
    rms=sqrt(mean(S.^2)*dt);
end