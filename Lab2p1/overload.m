function y=overload(X,a,b,G)
y=arrayfun(@(x) overload_element(x,a,b,G),X);
end

function y=overload_element(x,a,b,G)
    if(abs(x)>=a) 
        y=G*(b*x+sign(x)*(1-b)*a); 
    else
        y=G*x;
    end
end