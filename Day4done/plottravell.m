clear all;
close all;
clc;

x_ts = load("Day3_qs/Q5311R012/x_ts.mat").ans;
u_ts = load("Day3_qs/Q5311R012/u_ts_with_bias.mat").ans;

plot(x_ts(1, :),x_ts(2,:))