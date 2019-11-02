function [input, output, nonlinearCurrents, nonlinearVoltages, state] = ndkSolve( ...
          audioInput, ndkModel, nonlinFunction, jacobian, initialGuess, maxCount, tol, plotting)

N = length(audioInput);

A = ndkModel.A; B = ndkModel.B; C = ndkModel.C;
D = ndkModel.D; E = ndkModel.E; F = ndkModel.F;
G = ndkModel.G; H = ndkModel.H; K = ndkModel.K;

input = ones(ndkModel.numSources, N) .* ndkModel.sourceVoltages;
input(1, :) = audioInput;

output = zeros(ndkModel.numOutputs,N);
state = zeros(ndkModel.numReactives,N);
nonlinearVoltages = zeros(ndkModel.numNonlinears,N);
nonlinearCurrents = zeros(ndkModel.numNonlinears,N);

for n = 2 : N
    if (ndkModel.numNonlinears ~= 0)
        p = G*state(:,n-1) + H*input(:, n);
        step = 1;

        v = nonlinearVoltages(:,n-1);
        if n == 2
           v = initialGuess;
        end
        counter = maxCount;

        while norm(step) > tol && counter > 0
            J = K*jacobian(v) - eye(ndkModel.numNonlinears);    

            ni = p + K*nonlinFunction(v) - v; 
            step = J\ni;
            v = v - step;
            counter = counter - 1;
        end
        nonlinearVoltages(:,n) = v;
        nonlinearCurrents(:,n) = nonlinFunction(v);
    
        output(n) = D*state(:,n-1) + E*input(:,n) + F*nonlinFunction(v);
        state(:,n) = A*state(:,n-1) + B*input(:,n) + C*nonlinFunction(v); 
    else
        output(n) = D*state(:,n-1) + E*input(:,n);
        state(:,n) = A*state(:,n-1) + B*input(:,n);
    end
end