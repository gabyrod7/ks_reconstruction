# K<sub>S</sub> reconstruction study scripts 

The scripts in this repository were used for K<sub>S</sub> reconstruction studies.

- ```run.sh``` is the main script for reconstructing the data from . It is will setup the GlueX software and run the ```make_rest_files.py``` and ```make_root_trees.py```. The user should change the ```new_sqlite``` variable to the desired file in order to reconstruct data with the desired geometry.
- ```make_rest_files.py``` will run hd_root over the desired .evio files to produce a .hddm and .root files. There is a ```hd_root_args``` variable that was used in the past to change the magnetic field in some way. Here are some flags that were used in the past:
	- hd_root_args = " -PBFIELD:PHI_BFIELD=1. "
	- hd_root_args = " -PBFIELD:X_SHIFT=1. "
	- hd_root_args = " -PBFIELD:Z_SHIFT=1. "
- ```make_root_trees.py``` will use the root files from the previous script to produce standard GlueX root trees for the reaction of interest 
- Once the root trees have been created use ```runDSelector.sh``` to run the DSelector over these trees. Set the ```trees_location``` variable in the ```runDSelector.sh``` script to the location of the generated trees. Several 2D histograms (pipi mass vs. some variable) will be saved in a file called ```hist.root```.
- After the DSelector finishes run the ```run_analysis.C``` script. This script will go through the 2D histograms in the ```hist.root``` file. For each 2D histogram it will make projections, fit each projection with a Gaussian and save a graph with the mean of the Gaussian for each projection.
