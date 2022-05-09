clc;clear all;
%set
%phase
Id = [1 0; 0 1];
ket0 = [1; 0];
ket1 = [0; 1];
ket00=ket0*ket0';
CNOT = [1 0 0 0; 0 0 0 1; 0 0 1 0; 0 1 0 0];
SWAP = [1 0 0 0; 0 0 1 0; 0 1 0 0; 0 0 0 1];
Hadamard = [1 1;1 -1]/sqrt(2);


theta=45; %角度
theta2=theta*pi/180;
FrozenQ_Bit=cos(theta2)*ket0 + sin(theta2)*ket1;
FrozenQ=FrozenQ_Bit*FrozenQ_Bit';

theta=70; %角度
theta3=(theta)*pi/180;
MessageQ_Bit=cos(theta3)*ket0 + sin(theta3)*ket1;
MessageQ=MessageQ_Bit*MessageQ_Bit';

p=0.05;
M1 = sqrt(1-p)*[1 0; 0 1];
M2 = sqrt(p)*[1 0; 0 -1];
%[0 1; 1 0]
%[1 0; 0 -1]
%[0 -1i; 1i 0]

%Input
Input=kron(MessageQ,FrozenQ);

%Encode
Density_AfterEncode=CNOT*Input*CNOT';

%Add Noise
K=0;
Density_AfterChannel=0;
qubit_num=2;
[out] = bin_listq1(qubit_num);
for i=1:length(out)
K=getError(out(i,:),M1,M2);
Density_AfterChannel = Density_AfterChannel + K * Density_AfterEncode * K';
end

%Decode

Density_AfterDecode=CNOT*Density_AfterChannel*CNOT';


[Density_AfterM1,p1,o1]=measure(Density_AfterDecode, [0 1],FrozenQ_Bit);
[Density_AfterM2,p2,o2]=measure(Density_AfterM1, [1 0],MessageQ_Bit);

Outcome=[o1 o2];
Fidelity=p2;
disp('MessageQ')
disp(MessageQ)
disp('Fidelity')
disp(Fidelity)
