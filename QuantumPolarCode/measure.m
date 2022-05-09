function [ responseDensityMatrix,prob,outcome] = measure(systemDensityMatrix,qBitToMeasure,Q_Bit)
sizeDensityMatrix = length(systemDensityMatrix);
numOfQBits = log2(sizeDensityMatrix);
[Projector1, Projector2] = getProjectors2( numOfQBits, qBitToMeasure,Q_Bit);
prob=trace(Projector1 * systemDensityMatrix * Projector1);
    if (rand() < prob)
        responseDensityMatrix = (Projector1 * systemDensityMatrix * Projector1)/trace(Projector1 * systemDensityMatrix * Projector1);
        outcome=0;
    else
        responseDensityMatrix = (Projector2 * systemDensityMatrix * Projector2)/trace(Projector2 * systemDensityMatrix * Projector2);
        outcome=1;
    end
end
