clear all;
close all;

target_distances = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 300, 400, 500]; % Add or remove numbers as needed

cd('BLE_DATA/');

Brssi = struct();

for i = 1:length(target_distances)
    dist = target_distances(i);
    

    filename = sprintf('rssi_data_%d.csv', dist);
    
    fieldName = sprintf('d%d', dist);
    
    % Var3 should have the RSSI
    Brssi.(fieldName) = readtable(filename).Var3;

    currentData = Brssi.(fieldName);
    
    % Calculate statistics
    Bdis(i) = mean(currentData);
    Bstd(i) = std(currentData);
    Bvar(i) = var(currentData);
end


cd('../LORA_DATA/');

Lrssi = struct();

for i = 1:length(target_distances)
    dist = target_distances(i);
    

    filename = sprintf('rssi_data_%d.csv', dist);
    
    fieldName = sprintf('d%d', dist);
    
    % Var3 should have the RSSI
    Lrssi.(fieldName) = readtable(filename).Var2;

    currentData = Lrssi.(fieldName);
    
    % Calculate statistics
    Ldis(i) = mean(currentData);
    Lstd(i) = std(currentData);
    Lvar(i) = var(currentData);
end


cd('../WIFI_DATA/');

Wrssi = struct();

for i = 1:length(target_distances)
    dist = target_distances(i);
    

    filename = sprintf('rssi_data_%d.csv', dist);
    
    fieldName = sprintf('d%d', dist);
    
    % Var3 should have the RSSI
   Wrssi.(fieldName) = readtable(filename).Var2;

    currentData = Wrssi.(fieldName);
    
    % Calculate statistics
    Wdis(i) = mean(currentData);
    Wstd(i) = std(currentData);
    Wvar(i) = var(currentData);
end

ft = fittype('-10*n*log10(x) + C');

[Bcurve, goodness, output] = fit(target_distances(:), Bdis(:), ft)

[Lcurve, goodness, output] = fit(target_distances(:), Ldis(:), ft)

[Wcurve, goodness, output] = fit(target_distances(:), Wdis(:), ft)



figure('Position', [600 700 700 500]);

h1 = plot(Bcurve, 'b-', target_distances, Bdis, 'bs');
set(h1, 'DisplayName', 'BLE');
hold on;

errorbar(target_distances, Bdis, Bstd, 'bs', 'LineWidth', 2, 'HandleVisibility', 'off');
hold on;

h2 = plot(Lcurve, 'g-', target_distances, Ldis, 'gs');
set(h2, 'DisplayName', 'LORA');

hold on;

errorbar(target_distances, Ldis, Lstd, 'gs', 'LineWidth', 2, 'HandleVisibility', 'off');
hold on;

h3 = plot(Wcurve, 'r-', target_distances, Wdis, 'rs');
set(h3, 'DisplayName', 'WIFI');

hold on;

errorbar(target_distances, Wdis, Wstd, 'rs', 'LineWidth', 2, 'HandleVisibility', 'off');
hold on;

grid on;
xlabel('Distance');
ylabel('RSSI (dBm)');
title('Path Loss Fit');

cd('../')