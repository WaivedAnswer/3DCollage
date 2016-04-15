function copyMesH(X, filename)

[Xsize, ~] = size(X);
i=0;
readfilename = filename;
writefilename = 'newMesh.smf';
% fid = fopen(writefilename, 'r');
%     if fid == -1
%         fid = fopen(writefilename, 'wt');
%         formatSpec = '%f';
%         fwrite(fid, formatSpec, Xsize);
%         fclose(fid);
%         numObj =Xsize;
%     else
%         fid = fopen(writefilename, 'r');
%         tline = fgetl(fid);
%         numObj = str2num(tline);
%         
%         while ischar(tline)
%             i = i+1;
%             tline = fgetl(fid);
%             A{i} = tline;
%         end
%         fclose(fid);
%     end
    
%store obj filename and its pc axis to txt file
%restore previous data to the file
% fid = fopen('objPCA.txt', 'w');
% numObj=numObj+Xsize;
% fprintf(fid,'%i', numObj);
% for j = 1:i
%     if (A{j} ~= -1)
%         fprintf(fid,'\n%s', A{j});
%     end
% end
% fclose(fid);

% append new data to file
fileID = fopen('newMesh.smf','a+t','n');
formatSpec = '#%d';
fprintf(fileID,formatSpec,Xsize);

formatSpec = '\nv %s %s %s';
for k = 1:Xsize
fprintf(fileID,formatSpec,X(k,1),X(k,2),X(k,2));
end
fclose(fileID);

fid = fopen(writefilename, 'r');
%     if fid == -1
%         fid = fopen(writefilename, 'wt');
%         formatSpec = '%f';
%         fwrite(fid, formatSpec, Xsize);
%         fclose(fid);
%         numObj =Xsize;
%     else
%         fid = fopen(writefilename, 'r');
%         tline = fgetl(fid);
%         numObj = str2num(tline);
%         
%         while ischar(tline)
%             i = i+1;
%             tline = fgetl(fid);
%             A{i} = tline;
%         end
%         fclose(fid);
%     end
    
%store obj filename and its pc axis to txt file
%restore previous data to the file
fid = fopen('objPCA.txt', 'w');
numObj=numObj+Xsize;
fprintf(fid,'%i', numObj);
for j = 1:i
    if (A{j} ~= -1)
        fprintf(fid,'\n%s', A{j});
    end
end
fclose(fid);

end