% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Flåten

clear all;
close all;
clc;

%% Initialization and model definition
init; % Change this to the init file corresponding to your helicopter

% Discrete time system model. x = [lambda r p p_dot]'
delta_t	= 0.25; % sampling time
A1 =eye(6)+ delta_t* [
    0,1,0,0,0,0;
    0,0,-K_2, 0,0,0;
    0,0,0,1,0,0;
    0,0,-K_1*K_pp, -K_1*K_pd,0,0;
    0,0,0,0,0,1;
    0,0,0,0,-K_3*K_ep, -K_3*K_ed
    ];
B1 = delta_t* [0,0;0,0;0,0;K_1*K_pp,0;0,0;0,K_3*K_ep];

% Number of states and inputs
mx = size(A1,2); % Number of states (number of columns in A)
mu = size(B1,2); % Number of inputs(number of columns in B)
mx


% Initial values
x1_0 = pi;                               % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x5_0 = 0; %??????????????? riktig initial value
x6_0 = 0;
x0 = [x1_0 x2_0 x3_0 x4_0 x5_0 x6_0]';           % Initial values

% Time horizon and initialization
N  = 100;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization
z0(1:mx) = x0;
% Bounds
ul 	    = [-pi/6; -inf];                   % Lower bound on control
uu 	    = [pi/6;inf] ;                   % Upper bound on control


%uel= alpha_*exp(-beta_*(lambda)) %Lower bound elevation
%ueu = %Upper bound elevation
xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul(1);                           % Lower bound on state x3
xu(3)   = uu(1);                           % Upper bound on state x3

% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul,uu); % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vlb(N*mx+M*mu-1)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu-1)  = 0;                    % We want the last input to be zero


% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);
Q1(1,1) = 2;                            % Weight on state x1
Q1(2,2) = 0;                            % Weight on state x2
Q1(3,3) = 0;                            % Weight on state x3
Q1(4,4) = 0;                            % Weight on state x4
Q1(5,5) = 0;                            % Weight on state x5
Q1(6,6) = 0;                            % Weight on state x6
P1 = [0.12, 0;0,0.12];                                % Weight on input
Q = gen_q(Q1,P1,N,M);                                  % Generate Q, hint: gen_q
c = zeros(N*mx+M*mu,1);                                  % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A1,B1,N,mx,mu);             % Generate A, hint: gen_aeq
beq = zeros(size(Aeq, 1),1);
beq(1:mx) = A1*x0; % Generate b
size(Aeq);


%% Day 3 calculate K
Q_dlqr = diag([10,3,1,1, 10, 5]);
R_dlqr = diag([0.12,0.12]);
[K,P,eigenvalues] = dlqr(A1,B1,Q_dlqr,R_dlqr,[]);
K=K';

%% Solve QP problem with linear model
f = @(z) 0.5*z'*Q*z;
tic
options =optimoptions('fmincon','Algorithm','sqp','MaxFunEvals',400000);
%[z,lambda] = quadprog(Q,c,[],[],Aeq,beq,vlb,vub,x0); % hint: quadprog. Type 'doc quadprog' for more info 
nonlcon = @e_constraint;
%[z, lambda, exitflag,output] = fmincon(f,z0,[],[],Aeq,beq,vlb,vub,nonlcon, options);
[z, lambda, exitflag,output] = fmincon(f,z0,[],[],Aeq,beq,vlb,vub,@(z) e_constraint(z), options);
t1=toc;
output

% Calculate objective value
phi1 = 0.0;
PhiOut = zeros(N*mx+M*mu,1);
for i=1:N*mx+M*mu
  phi1=phi1+Q(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

%% Extract control inputs and states
%size(z)
u  = [z(N*mx+1:N*mx+M*mu);z(N*mx+M*mu)]; % Control input from solution
%size(u)
u1 = [u(1:mu:M*mu)];
u2 = [u(2:mu:M*mu)];
u1 = [u1; u1(end)];
u2 = [u2; u2(end)];
%size(u1)
%size(u2)

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);z(5:mx:N*mx)];              % State x4 from solution
x6 = [x0(6);z(6:mx:N*mx)];              % State x4 from solution

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

%u   = [zero_padding; u; zero_padding];
u1   = [zero_padding; u1; zero_padding];
u2   = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

%size(u1)180=pi

t = 0:delta_t:delta_t*(length(u1)-1);
%size(t)
u = [
    u1,u2
    ];
x_vector = [
    x1,x2,x3,x4,x5,x6
];
u2;
%size(u)
%size(x_vector)



%% Plotting

u_ts = timeseries(u,t);
x_ts = timeseries(x_vector, t);
%x_ts = x_vector;
figure(2)
subplot(711)
stairs(t,u),grid
ylabel('u')
subplot(712)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(713)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(714)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(715)
plot(t,x4,'m',t,x4','mo'),grid
xlabel('tid (s)'),ylabel('pdot')
subplot(716)
plot(t,x5,'m',t,x5','mo'),grid
xlabel('tid (s)'),ylabel('e')
subplot(717)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('tid (s)'),ylabel('edot')

function [c,ceq] = e_constraint(z)
    alpha_ = 0.2;
    beta_ = 20;
    lambda_t_ = 2*pi/3;
    
    N=100;
    c = zeros(N,1);
    mx = 6;
    for i=1:N
        lambda_k = z(1+(i-1)*mx);
        e_k= z(5+(i-1)*mx);
        c(i) = alpha_*exp(-beta_*(lambda_k - lambda_t_)^2)-e_k;
    end
    ceq = [];
end


