% Simon's code

nCuts_no = length(unique(nCuts));
nTrees_no = length(unique(nTrees));



nCutsRS = reshape(nCuts,length(unique(nTrees)),length(unique(nCuts)))
nTreesRS = reshape(nTrees,length(unique(nTrees)),length(unique(nCuts)))
ROCintRS = reshape(ROCint,length(unique(nTrees)),length(unique(nCuts)))
surf(nCutsRS,nTreesRS,ROCintRS)