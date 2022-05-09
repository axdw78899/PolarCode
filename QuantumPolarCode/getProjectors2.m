function [ Projector1, Projector2 ] = getProjectors2( numOfQBits, qBitToMeasure,measure_operater)
    % Constants
    ket0 = measure_operater;
    ket1 = null(measure_operater.');
    %ket1 = measure_operater;
    %ket0 = -null(measure_operater.');

    IdentityMatrix = [1 0; 0 1];   
    
    for iterator = 1:1:numOfQBits
        if (iterator == 1)
            if (qBitToMeasure(iterator) == 1)
                Projector1 = ket0 * ket0';
                Projector2 = ket1 * ket1';
            else
                Projector1 = IdentityMatrix;
                Projector2 = IdentityMatrix;
            end
        else
            if (qBitToMeasure(iterator) == 1)
                Projector1 = kron(Projector1, ket0 * ket0');
                Projector2 = kron(Projector2, ket1 * ket1');
            else
                Projector1 = kron(Projector1, IdentityMatrix);
                Projector2 = kron(Projector2, IdentityMatrix);
            end
        end
    end

end