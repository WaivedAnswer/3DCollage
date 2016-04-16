function [F, fnum] = readFace(filename)


    fid = fopen(filename, 'r');
    if fid == -1
        disp('ERROR: could not open file');
        F = 0; 
        return;
    end

    fnum = 1;

    while (feof(fid) ~= 1)
        line = '';
        line = fgetl(fid);
        if length(line) > 0 & line(1) == 'f'
            dummy = sscanf(line, '%c %d %d %d');
            F(fnum,:)= dummy(2:4);
            fnum = fnum + 1;
        end
    end

    fclose(fid);


end