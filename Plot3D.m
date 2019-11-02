%d = xlsread('Pemetaan Magnetizer 1 Z.xlsx', 'z=0', 'D5:N14');
d = xlsread('Pemetaan Magnetizer 2 X,Y,Z.xlsx', 'Y', 'D5:X14');

%[X,Y] = meshgrid(0:.5:5,0:.5:4.5);
%X1 = [0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5];
%Y1 = [0; 0.5; 1; 1.5; 2; 2.5; 3; 3.5; 4; 4.5;];

[X,Y] = meshgrid(0:.5:10,0:.5:4.5);
X1 = [0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 8.5 9 9.5 10];
Y1 = [0; 0.5; 1; 1.5; 2; 2.5; 3; 3.5; 4; 4.5;];

Z = griddata(X1, Y1, d, X, Y, 'cubic');
mesh(X,Y,Z)%, hold on;
%plot3(X1,Y1,d,'.'), hold on;
surf(X,Y,d);

%title('Grafik Pemetaan Fluks Magnet Arah Sumbu-Y')
xlabel('Y (cm)')
ylabel('X (cm)')
zlabel('By (mT)')
