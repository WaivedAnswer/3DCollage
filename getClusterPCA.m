function [pca, cen] = getClusterPCA(index)

fid = fopen('clusterPCA.txt', 'r');
    if fid == -1
        disp('ERROR: could not open file');
        V = 0; 
        return;
    end

    while (feof(fid) ~= 1)
        line = '';
        line = fgetl(fid);
        if length(line) > 0 & line(1) == 'C'
            dummy = sscanf(line, '%c %f %f %f %f %f %f %f %f %f %f %f %f %f');
            clustid = dummy(2);
            if(clustid == index)
            pca(1, :) = dummy(3:5, :);
            cen(1, :) = dummy(12:14, :);
            end
        end
    end


fclose(fid);
% scatter3(cen(1,1), cen(1,2), cen(1,3), 'r*');
end