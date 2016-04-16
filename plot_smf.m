function [F, X] = plot_smf(smf_file)

[F, X] = read_smf2(smf_file);


% scatter3(X(:,1), X(:,2), X(:,3));
trimesh(F, X(:,1), X(:,2), X(:,3), 'EdgeColor', [0.3 0.3 0.3], 'FaceColor', [0.8 0.8 0.8], 'FaceLighting', 'phong');