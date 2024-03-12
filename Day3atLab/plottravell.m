clear all;
close all;
clc;

% Datafiles Directory
datafiles_directory_name ="activeplots\";

% read all files and store them in struct
data_struct= struct;
cd(datafiles_directory_name);
data_directories= dir;
for k=3:length(dir)
    run_struct=struct();
    struct_name =data_directories(k).name;
    cd(data_directories(k).name)
    run_directory =dir;
    for j=3:length(run_directory)
        run_struct.(run_directory(j).name(1:end-4)) = load(run_directory(j).name);
    end
    data_struct.(struct_name)=run_struct;
    cd("..");
end
cd("..");


%x_ts = load("Day3_qs/Q5311R012/x_ts.mat").ans;
%u_ts = load("Day3_qs/Q5311R012/u_ts_with_bias.mat").ans;
%t = x_ts(1, :);
%u = u_ts(2,:);
%x1 = x_ts(2,:);
%x2 = x_ts(3,:);
%x3 = x_ts(4,:);
%x4 = x_ts(5,:);
%plot(x_ts(1, :),x_ts(2,:))

%% Plotting
fn = fieldnames(data_struct);
%x_ts = x_vector;
figure(2)
sgtitle("LQ controller output")
subplot(511)
hold("on")
%length(data_struct)
for k= 1:length(fn)
    u = data_struct.(fn{k}).u_ts_with_bias.ans(2,:);
    t = data_struct.(fn{k}).u_ts_with_bias.ans(1,:);
    stairs(t,u)
end
grid
hold("off")
ylabel('u')
xlim([5,t(end)+1])
subplot(512)
hold("on")
legendlist=[];
for k= 1:length(fn)
    t = data_struct.(fn{k}).loop_variables.y_loop_variables(1, :);
    x1 = data_struct.(fn{k}).loop_variables.y_loop_variables(2, :);
    plot(t,x1, "DisplayName",fn{k})
end
t = data_struct.(fn{k}).x_ts.ans(1, :);
x1 = data_struct.(fn{k}).x_ts.ans(2, :);
plot(t,x1, "m--", "DisplayName","$x^*$" )
grid
lgd = legend;
lgd.Interpreter = "Latex";
legend show
hold("off")
ylabel('lambda')
xlim([5,t(end)+1])
subplot(513)
hold("on")
for k= 1:length(fn)
    t = data_struct.(fn{k}).loop_variables.y_loop_variables(1, :);
    x2 = data_struct.(fn{k}).loop_variables.y_loop_variables(3, :);
    plot(t,x2, "DisplayName",fn{k})
    
end
t = data_struct.(fn{k}).x_ts.ans(1, :);
x2 = data_struct.(fn{k}).x_ts.ans(3, :);
plot(t,x2, "m--", "DisplayName","$x^*$" )
grid
lgd = legend;
lgd.Interpreter = "Latex";
legend show
hold("off")

ylabel('r')
xlim([5,t(end)+1])
subplot(514)
hold("on")
for k= 1:length(fn)
    t = data_struct.(fn{k}).loop_variables.y_loop_variables(1, :);
    x3 = data_struct.(fn{k}).loop_variables.y_loop_variables(4, :);
    plot(t,x3, "DisplayName",fn{k})
    
end
t = data_struct.(fn{k}).x_ts.ans(1, :);
x3 = data_struct.(fn{k}).x_ts.ans(4, :);
plot(t,x3, "m--", "DisplayName","$x^*$" )
grid
lgd = legend;
lgd.Interpreter = "Latex";
legend show
hold("off")
ylabel('p')
xlim([5,t(end)+1])
subplot(515)
hold("on")
for k= 1:length(fn)
    t = data_struct.(fn{k}).loop_variables.y_loop_variables(1, :);
    x4 = data_struct.(fn{k}).loop_variables.y_loop_variables(5, :);
    plot(t,x4, "DisplayName",fn{k})
    
end
t = data_struct.(fn{k}).x_ts.ans(1, :);
x4 = data_struct.(fn{k}).x_ts.ans(5, :);
plot(t,x4, "m--", "DisplayName","$x^*$" )
grid
lgd = legend;
lgd.Interpreter = "Latex";
legend show
hold("off")
xlabel('tid (s)'),ylabel('pdot')
xlim([5,t(end)+1])