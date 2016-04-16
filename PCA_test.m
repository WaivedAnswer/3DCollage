clear all
clc
filename = 'bigFish.smf';
filename2 = 'man.smf';



X1 = read_smf('bigFish.smf');
X2 = read_smf('man.smf');

[pc1, cenX, cenY, cenZ] = objPCA(filename);
hold on
[pc2, cenX2, cenY2, cenZ2] = objPCA(filename2);%target
hold on

% scatter3(X2(:,1)-cenX2+cenX, X2(:,2)-cenY2+cenY, X2(:,3)-cenZ2+cenZ);



vec1 = pc1(1,(1:3));
vec2 = pc2(1,(1:3));

H = transpose(vec1)*vec2;

[U, S, V] = svd(H);

R = V*transpose(U);

% vec2 = vec2*R;

% plot3([0 vec1(1,1)], [0 vec1(1,2)], [0 vec1(1,3)], 'k','LineWidth',6)
% hold on

X2 = X2*R;
[numPoint, ~]= size(X2);
newCenX = sum(X2(:,1))/numPoint;
newCenY = sum(X2(:,2))/numPoint;
newCenZ = sum(X2(:,3))/numPoint;
figure
scatter3(X1(:,1), X1(:,2), X1(:,3));
hold on
scatter3(X2(:,1)-newCenX+cenX, X2(:,2)-newCenY+cenY, X2(:,3)-newCenZ+cenZ);

