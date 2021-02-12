% display all the data in a meaningful graphical manner 
% Take in the CSV file and read out the individual communs for each
% varaible
clear
clc
[nums] = csvread("genData.csv");

Generation = nums(:, 1);
MaxFitness = nums(:, 2);
AvgFitness = nums(:, 3);    
GenDiversity = MaxFitness - AvgFitness;

xmax = max(Generation) + 2 ;
ymax = 1.2; yhj
figure 
plot (Generation,MaxFitness,'red',Generation,AvgFitness,'blue', Generation, GenDiversity, 'green')
xlim ([0, xmax]) 
ylim ([0, ymax])
xlabel ('Generations')
ylabel ('Fitness Scores') 
title ('Fitness Scores approaching 1') 
legend ('Maximum' , 'Average', 'Genetic Diversity')
