function [uni] = getError(qBit,M1,M2)
num=length(qBit);
 for i=1:1:num
     if (qBit(i) == 0)
        A(:,:,i)=M1;
     else
        A(:,:,i)=M2;
     end
 end
a=A(:,:,1);
for j=1:num-1
    a=kron(A(:,:,j+1),a);
end
uni=a;
end