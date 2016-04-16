function copyMesH(X, F)

[Xsize, ~] = size(X);
[Fsize, ~] = size(F);
newX = 0;
newF = 0;

fid = fopen('newMesh.smf', 'r');
    if fid == -1
        % % append new data to file
        faID = fopen('newMesh.smf','a+t','n');
        
        formatSpec = '\n# %d %d';
        fprintf(faID,formatSpec,Xsize, Fsize);
        
        formatSpec = '\nv %s %s %s';
        for k = 1:Xsize
        fprintf(faID,formatSpec,X(k,1),X(k,2),X(k,3));
        end
        formatSpec = '\nf %d %d %d';
        for h = 1:Fsize
        fprintf(faID,formatSpec,F(h,1),F(h,2),F(h,3));
        end
        fclose(faID);
        return;
    end
    
fid = fopen('newMesh.smf', 'r'); 
        while (feof(fid) ~= 1)
            line = '';
            line = fgetl(fid);
            if length(line) > 0 & line(1) == '#'
              dummy = sscanf(line, '%c %d %d %d')
              info(1, 1:2) = dummy(2:3)
              newX = dummy(2);
              newF = dummy(3);
            end
        end
fclose(fid);

[oldF, oldV] = read_smf2('newMesh.smf');
[ovs, ~] = size(oldV);
[ofs, ~] = size(oldF);
% % append new data to file
faID = fopen('newMesh.smf','wt');
formatSpec = '# %d %d';
fprintf(faID,formatSpec,Xsize+ovs, Fsize+ofs);
fclose(faID);

faID = fopen('newMesh.smf','a+t','n');
formatSpec = '\nv %f %f %f';
for k = 1:ovs
fprintf(faID,formatSpec,oldV(k,1),oldV(k,2),oldV(k,3));
end
formatSpec = '\nf %d %d %d';
for h = 1:ofs
fprintf(faID,formatSpec,oldF(h,1),oldF(h,2),oldF(h,3));
end
fclose(faID);


% % append new data to file
faID = fopen('newMesh.smf','a+t','n');
formatSpec = '\nv %f %f %f';
for k = 1:Xsize
fprintf(faID,formatSpec,X(k,1),X(k,2),X(k,3));
end
formatSpec = '\nf %d %d %d';
for h = 1:Fsize
fprintf(faID,formatSpec,F(h,1)+ovs,F(h,2)+ovs,F(h,3)+ovs);
end
fclose(faID);

end