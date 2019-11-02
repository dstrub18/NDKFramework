function ndkPlot(input, output, nonlinearCurrents, nonlinearVoltages, states)

subplot(2,3,1); plot(output); title("Outputs");
subplot(2,3,2); plot(input'); title("Inputs");
subplot(2,3,3); plot(nonlinearCurrents'); title("Currents");
subplot(2,3,4); plot(nonlinearVoltages'); title("Voltages");
subplot(2,3,5); plot(states');title("States");