%
% This function reads in an SMF file and stores the geometry
% and connectivity information
%
% Input: name (full path) of the smf file as a string
% Output: Point coordinates list X (n x 3) and face list 
% F (f x 3) indexing into the point list.
%
% ----------------------------------------------------------
% Richard Zhang (c) 2016
%
%
% Modified by Cathy Yiu 8th April,2016
%

function [V, vnum] = read_smf(filename)

    fid = fopen(filename, 'r');
    if fid == -1
        disp('ERROR: could not open file');
        V = 0; 
        return;
    end

    vnum = 1;

    while (feof(fid) ~= 1)
        line = '';
        line = fgetl(fid);

        if length(line) > 0 & line(1) == 'v'
            dummy = sscanf(line, '%c %f %f %f');
            V(vnum, :) = dummy(2:4, :);
            vnum = vnum + 1;
        end
    end

    fclose(fid);
