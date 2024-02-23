#!/bin/bash

# config
OUTDIR=trees
mkdir $OUTDIR

new_sqlite=/d/grid15/gabyrod7/scigrid2/kskpi/kskppim/DSelector/mass_and_resolution_studies/ccdb_fix5/run_all/ccdb_NewAlignment_020224.sqlite

rm "ccdb_geometry.log"

# set up software
export GLUEX_TOP=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top
export BUILD_SCRIPTS=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top/build_scripts
source $BUILD_SCRIPTS/gluex_env_clean.sh

export GLUEX_TOP=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top
export BUILD_SCRIPTS=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top/build_scripts
export BMS_OSNAME=`$BUILD_SCRIPTS/osrelease.pl`
source $BUILD_SCRIPTS/gluex_env_version.sh version.xml 
export JANA_CALIB_URL=sqlite:///$new_sqlite

# make REST files
# edit make_rest_files.py first!
python make_rest_files.py $OUTDIR | tee -a "ccdb_geometry.log"
#mv *.hddm *.root $OUTDIR
mv *.hddm $OUTDIR


## reset software
export GLUEX_TOP=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top
export BUILD_SCRIPTS=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top/build_scripts
source $BUILD_SCRIPTS/gluex_env_clean.sh

export GLUEX_TOP=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top
export BUILD_SCRIPTS=/d/home/sdobbs/grid13/Software/gluex_install/gluex_top/build_scripts
export BMS_OSNAME=`$BUILD_SCRIPTS/osrelease.pl`
source $BUILD_SCRIPTS/gluex_env_version.sh version.xml 
export JANA_CALIB_URL=sqlite:////d/grid15/gabyrod7/scigrid2/kskpi/kskppim/DSelector/mass_and_resolution_studies/ccdb_fix5/run_all/ccdb_NewAlignment_020224.sqlite

#source ~/.bashrc
#source /d/home/gabyrod7/gluex_top/gluex_env_boot_gabriel.sh
#gxclean
#gxenv /d/grid15/gabyrod7/scigrid2/kskpi/kskppim/DSelector/mass_and_resolution_studies/Software/version_Dec2021.xml


# make ROOT files
python make_root_trees.py $OUTDIR | tee -a "make_root_trees.log"

