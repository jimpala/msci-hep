% heatmesh.m
%%%%%%%%%%%%%%%%%%%%%%%%%%%
% A script to extract the gridearch csv data file
% and plot a heatmap.


% x = 1:3;
% y = 1:3;
% z = linspace(3,9,3);
% 
% [xgrid, ygrid, zgrid] = meshgrid(x,y,z);
% xgrid;
% ygrid;
% zgrid;

% Import table. Drop first column and row.
table = csvread('lowpt_2jet.csv',1,1);

nbinsx = size(unique(table(:,1)))
nbinsy = size(unique(table(:,2)))

nCuts = table(:,1);
nTrees = table(:,2);
ROCint = table(:,3);

hold on;

grid = [nCuts, nTrees];
hist3(grid, [nbinsx(1), nbinsy(1)])

h = pcolor(