function newMesh(filename, index, startnum)

% filename = 'man.smf'
[X2, X2vsize] = read_smf(filename);
[pc1, cenX, cenY, cenZ] = objPCA(filename);
vec1 = pc1(1,(1:3));
[pca, cen]= getClusterPCA(index);

vec2 = pca(1,(1:3));
H = transpose(vec1)*vec2;

[U, S, V] = svd(H);

R = V*transpose(U);

vec2 = vec2*R;

X2 = X2*R;
[numPoint, ~]= size(X2);
newCenX = sum(X2(:,1))/numPoint;
newCenY = sum(X2(:,2))/numPoint;
newCenZ = sum(X2(:,3))/numPoint;
% figure
% scatter3(X1(:,1), X1(:,2), X1(:,3));
X2(:,1) = X2(:,1)-newCenX+cenX;
X2(:,2) = X2(:,2)-newCenY+cenY;
X2(:,3) = X2(:,3)-newCenZ+cenZ;
% hold on
scatter3(X2(:,1), X2(:,2), X2(:,3));
% 
F = readFace(filename);
copyMesH(X2, F);
hold off
figure;
plot_smf('newMesh.smf')

end