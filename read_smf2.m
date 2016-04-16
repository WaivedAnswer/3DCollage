function [F, X] = read_smf2(filename)

    fid = fopen(filename, 'r');
    if fid == -1
        disp('ERROR: could not open file');
        F = 0; 
        X = 0;
        return;
    end

    vnum = 1;
    fnum = 1;

    while (feof(fid) ~= 1)
        line = '';
        line = fgetl(fid);

        if length(line) > 0 & line(1) == 'v'
            dummy = sscanf(line, '%c %f %f %f');
            X(vnum, :) = dummy(2:4, :);
            vnum = vnum + 1;
        elseif length(line) > 0 & line(1) == 'f'
            dummy = sscanf(line, '%c %d %d %d');
            F(fnum,:)= dummy(2:4);
            fnum = fnum + 1;
        end

        % all other lines are ignored
    end

    fclose(fid);

end