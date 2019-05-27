fs = 100; %100 Hz
nyq = 100/2; %50 Hz
fc = 5; %Hz
Wn = fc/nyq;
nMAF = 10;
wMAF = ones(1,nMAF)*1/nMAF;
figure;
freqz(wMAF)

wFIR = fir1(6,Wn);
figure; freqz(wFIR)

figure;
data = readtable('DSPgraphs.xlsx');
plot(data{:,2:5});
legend('Raw','MAF','IIR','FIR')
title('All')
figure;
subplot(2,2,1);
plot(data{:,2});
axis([0 100 -16500 -15000])
title('Raw')
subplot(2,2,2);
plot(data{:,3});
axis([0 100 -16500 -15000])
title('MAF')
subplot(2,2,3);
plot(data{:,4});
axis([0 100 -16500 -15000])
title('IIR')
subplot(2,2,4);
plot(data{:,5});
axis([0 100 -16500 -15000])
title('FIR')

% looks like IIR is best, few calculations and good looking signal