
clear all; close all; clc; 
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

pl_file = '\DebugFiles\PL_Data\Plots\Sorted_Pos_lat47.464085lon19.154686h100_deltaT3600_el0_Tar_y2019_d019_136_Ref_CODG0190_brdc0190.txt';  %just for testing

filewPath = which(pl_file);
if exist(filewPath, 'file') ~= 2
   	error('%s file open error\n', filewPath);
end
[filepath,name,ext] = fileparts(filewPath);

A = importdata(filewPath);

pparam = setPlotParameters( filepath );

plData = A(:,1);
errorData = A(:,3);
numberOfSat = A(:,7);
strRequirementName = 'PL Horizontal (3 \sigma)';
AlertLimit_m = 5;

createStanfordDiagram(	abs(errorData) ...
                        ,plData ...
                        ,40 ...
                        ,numberOfSat ...
                        ,AlertLimit_m ...
                      	,strRequirementName ...
                        ,'Number of Used Sat' ...
                      	,strcat(filepath,'/Stanford_Diagram_','PL Horizontal') ...
                        ,pparam);
                    
plData = A(:,2);
errorData = A(:,4);
numberOfSat = A(:,7);
strRequirementName = 'PL Vertical (3 \sigma)';
AlertLimit_m = 7;

createStanfordDiagram(	abs(errorData) ...
                        ,plData ...
                        ,40 ...
                        ,numberOfSat ...
                        ,AlertLimit_m ...
                      	,strRequirementName ...
                        ,'Number of Used Sat' ...
                      	,strcat(filepath,'/Stanford_Diagram_','PL Vertical') ...
                        ,pparam);                    


