clc
% minAlt = 180;
% 
% for i = 1:1252
%     if(Altitude(i) < minAlt)
%         minAlt = Altitude(i);
%     end
% end
% minAlt;
% 
% for i = 1:1252
%     Altitude(i) = Altitude(i) - minAlt;
% end


% Pressure2 = Pressure;
% for i = 1:1252
%     Pressure2(i) = Pressure(i) - 3000;
% end
% 
% figure(1)
% hold on;
% plot(Altitude, Pressure, '*b')
% plot(Altitude, Pressure2, '*k')

A = [2 3 6 3];
B = [3 3 3 5];
C = cov(A,B)

sum = 0;
var = 0;
var2 = 0;
covar1 = 0;
covar2 = 0;
temp = 0;
temp2 = 0;
temp3 = 0;
temp4 = 0;
sum2 = 0;
sum3 = 0;
sum4 = 0;
avg2 = (3+3+3+5)/4;
avg = (2 + 3 + 6 + 3) / 4;
avg;
for i = 1:4
    temp = (A(i) - avg)^2;
    sum = sum + temp;
    temp2 = (B(i) - avg2)^2;
    sum2 = sum2 + temp2;
    
    temp3 = (A(i) - avg) * (B(i) - avg2);
    sum3 = sum3 + temp3
end

var = sum/3
var2 = sum2 / 3

covar1 = sum3/3