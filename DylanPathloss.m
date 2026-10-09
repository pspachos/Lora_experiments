clear all;
close all;


%% READ CSV FILES %%

Bdata20 = readtable('BLE_20.csv'); 
Bdata40 = readtable('BLE_40.csv'); 
Bdata60 = readtable('BLE_60.csv'); 
Bdata80 = readtable('BLE_80.csv'); 
Bdata100 = readtable('BLE_100.csv'); 
Bdata120 = readtable('BLE_120.csv'); 
Bdata140 = readtable('BLE_140.csv'); 
Bdata160 = readtable('BLE_160.csv'); 
Bdata180 = readtable('BLE_180.csv'); 
Bdata200 = readtable('BLE_200.csv'); 




%% READ RSSI %%

Brssi20 = Bdata20.Var3;
Brssi40 = Bdata40.Var3;
Brssi60 = Bdata60.Var3;
Brssi80 = Bdata80.Var3;
Brssi100 = Bdata100.Var3;
Brssi120 = Bdata120.Var3;
Brssi140 = Bdata140.Var3;
Brssi160 = Bdata160.Var3;
Brssi180 = Bdata180.Var3;
Brssi200 = Bdata200.Var3;





%% FIND MEAN RSSI %%

Bdis20 = mean(Brssi20);
Bdis40 = mean(Brssi40);
Bdis60 = mean(Brssi60);
Bdis80 = mean(Brssi80);
Bdis100 = mean(Brssi100);
Bdis120 = mean(Brssi120);
Bdis140 = mean(Brssi140);
Bdis160 = mean(Brssi160);
Bdis180 = mean(Brssi180);
Bdis200 = mean(Brssi200);





%% FIND STD VALUES %%

Bstd20= std(Brssi20);
Bstd40= std(Brssi40);
Bstd60= std(Brssi60);
Bstd80= std(Brssi80);
Bstd100= std(Brssi100);
Bstd120= std(Brssi120);
Bstd140= std(Brssi140);
Bstd160= std(Brssi160);
Bstd180= std(Brssi180);
Bstd200= std(Brssi200);




%% FITTING CURVE  %%


distance=[0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8 2];

Brssi= [Bdis20 Bdis40 Bdis60 Bdis80 Bdis100 Bdis120 Bdis140 Bdis160 Bdis180 Bdis200];
Bstds= [Bstd20 Bstd40 Bstd60 Bstd80 Bstd100 Bstd120 Bstd140 Bstd160 Bstd180 Bstd200];

distance_2=[0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8 2];

Brssi_2= [Bdis20 Bdis40 Bdis60 Bdis80 Bdis100 Bdis120 Bdis140 Bdis160 Bdis180 Bdis200];



f = '-10*n*log10(x)+C'

[Bcurve, goodness, output] = fit(distance', Brssi', f)

h=plot(Bcurve,'b',distance,Brssi,'bs');
hold on;
errorbar(distance,Brssi, Bstds,'bs', 'LineWidth', 2 );
grid on;
xlim([0 1.6])
ylim([-60 0])
set(h,'markers',8, 'linewidth',2);
set(gca,'fontsize',14, 'XTick', [0:0.2:1.6]);
xlabel('Distance (meters)', 'FontWeight', 'bold'); 
ylabel('RSSI (dBm)', 'FontWeight', 'bold');
legend('BLE - Raw Data', 'BLE - Fitted Curve');



