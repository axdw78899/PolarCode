function [out] = bin_listq1(nsize)
 n=2^nsize;%行數
 w=zeros(n,nsize);
 for m=1:n
    w(m,:)=bitget(m-1,nsize:-1:1);
 end
 out=w;
end
