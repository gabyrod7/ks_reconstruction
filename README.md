# K<sub>S</sub> reconstruction study scripts 

The scripts in this repository were used for K<sub>S</sub> reconstruction studies.

- ```run.sh``` is the main script. It is will setup the GlueX software and run the ```make_rest_files.py``` and ```make_root_trees.py```. The user should change the ```new_sqlite``` variable to the desired file in order to reconstruct data with the desired geometry.
- ```make_rest_files.py``` will run hd_root over the desired .evio files to produce a .hddm and .root files.
- ```make_root_trees.py``` will use the root files from the previous script to produce standard GlueX root trees for the reaction of interest 
