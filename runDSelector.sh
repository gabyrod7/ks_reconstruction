#!/bin/bash

trees_location='trees/tree*'

##### TChain PROOF####
root  -l -b << EOF
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("pimkpks__B4_M16_Tree");
chain->Add("$trees_location*");
DPROOFLiteManager::Process_Chain(chain,"DSelector_kskpi.C+", 16, "hist.root", "tree.root"); 
EOF

rm -r 0.*
