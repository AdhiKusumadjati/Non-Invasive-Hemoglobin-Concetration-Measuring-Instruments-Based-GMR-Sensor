d = xlsread('Pemetaan Magnetizer 1 Z.xlsx', 'z=0', 'D5:N14');

[X,Y] = meshgrid(0:.5:5,0:.5:4.5);
%X1 = [0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5];
%Y1 = [0; 0.5; 1; 1.5; 2; 2.5; 3; 3.5; 4; 4.5;];

Z = griddata(X1, Y1, d, X, Y, 'cubic');
mesh(X,Y,Z)%, hold on;
%plot3(X1,Y1,d,'.'), hold on;
%surf(X,Y,d);
[U,V,W] = surfnorm(X,Y,Z);

quiver3(X,Y,Z,U,V,W,0)

hold on
surf(X,Y,Z)
view(-35,45)
hold off

title('z = 0 cm')
xlabel('Y (cm)')
ylabel('X (cm)')
zlabel('By (mT)')

