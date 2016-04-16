function [pc, cenX, cenY, cenZ] = objPCA(filename)

V = read_smf(filename);
[numPoint, dim]= size(V);
scatter3(V(:,1), V(:,2), V(:,3), 'b.');
hold on

cenX = sum(V(:,1))/numPoint;
cenY = sum(V(:,2))/numPoint;
cenZ = sum(V(:,3))/numPoint;
[pc,score,latent,tsquare] = pca(V);

scatter3(cenX, cenY, cenZ, 'r*');
hold on
plot3([cenX pc(1,1)+cenX], [cenY pc(1,2)+cenY], [cenZ pc(1,3)+cenZ], 'r','LineWidth',6)
hold on
plot3([cenX pc(2,1)+cenX], [cenY pc(2,2)+cenY], [cenZ pc(2,3)+cenZ], 'g','LineWidth',6)
hold on
plot3([cenX pc(3,1)+cenX], [cenY pc(3,2)+cenY], [cenZ pc(3,3)+cenZ], 'b','LineWidth',6)


i=0;
writefilename = 'objPCA.txt';
fid = fopen(writefilename, 'r');
    if fid == -1
        fid = fopen(writefilename, 'wt');
        formatSpec = '0';
        fwrite(fid, formatSpec);
        fclose(fid);
        numObj =0;
    else
        fid = fopen(writefilename, 'r');
        tline = fgetl(fid);
        numObj = str2num(tline);
        
        while ischar(tline)
            i = i+1;
            tline = fgetl(fid);
            A{i} = tline;
        end
        fclose(fid);
    end
    
%store obj filename and its pc axis to txt file
%restore previous data to the file
fid = fopen('objPCA.txt', 'w');
numObj=numObj+1;
fprintf(fid,'%i', numObj);
for j = 1:i
    if (A{j} ~= -1)
        fprintf(fid,'\n%s', A{j});
    end
end
fclose(fid);

% append new data to file
fileID = fopen('objPCA.txt','a+t','n');
formatSpec = '\n%s %s %s %s %s %s %s %s %s %s';
fprintf(fileID,formatSpec,filename,num2str(pc(1,1)), num2str(pc(1,2)),num2str(pc(1,3)),num2str(pc(2,1)),num2str(pc(2,2)),num2str(pc(2,3)),num2str(pc(3,1)),num2str(pc(3,2)),num2str(pc(3,3)));

fclose(fileID);

end