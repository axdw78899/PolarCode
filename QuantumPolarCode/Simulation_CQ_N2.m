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

%CQ_channel
theta=0; %角度
theta2=theta*pi/180;
FrozenQ_Bit=cos(theta2)*ket0 + sin(theta2)*ket1;
FrozenQ=FrozenQ_Bit*FrozenQ_Bit';

theta=randi([0 90]); %角度
theta3=(theta)*pi/180;
MessageQ_Bit=cos(theta3)*ket0 + sin(theta3)*ket1;
MessageQ=MessageQ_Bit*MessageQ_Bit';

p=0.05;
M1 = sqrt(1-p)*[1 0; 0 1];
M2 = sqrt(p)*[0 1; 1 0];
%[0 1; 1 0]
%[1 0; 0 -1]
%[0 -1i; 1i 0]

qubit_num=1;%有幾個qubit
FrozenQ_AfterNoise = 0;
MessageQ_AfterNoise = 0;
%add noisy
K=0;
[out] = bin_listq1(qubit_num);
for i=1:length(out)
K=getError(out(i,:),M1,M2);
FrozenQ_AfterNoise = FrozenQ_AfterNoise + K * FrozenQ * K';
end

%add noisy
K=0;
[out] = bin_listq1(qubit_num);
for i=1:length(out)
K=getError(out(i,:),M1,M2);
MessageQ_AfterNoise = MessageQ_AfterNoise + K * MessageQ * K';
end
%%%

Density_AfterChannel=kron(MessageQ_AfterNoise,MessageQ_AfterNoise); %{0 1} 經過CNOT-> {1 1} 經過CQ-> {rho1 rho1}
Density_AfterDecode=CNOT*Density_AfterChannel*CNOT';                %{rho1 rho1} 經過解碼CNOT->

[Density_M1,p1,o1]=measure(Density_AfterDecode, [1 0],ket0);        %做 Measurement
[Density_M2,p2,o2]=measure(Density_M1, [0 1],MessageQ_Bit);
Density_M1(find(Density_M1==0))=[];
Density_After_M1 = reshape(Density_M1,[2,2]);
Fidelity=trace(Density_After_M1*MessageQ);%等於p2
disp('MessageQ')
disp(MessageQ)
if(o1==0)
    disp('MessageQ After Measurement')
    disp(Density_After_M1)
else
    disp('Failed!')
end


disp('Fidelity')
disp(Fidelity)
