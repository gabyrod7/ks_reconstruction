import os,sys
import glob

if len(sys.argv) < 2:
    print "need to pass input directory!" 
dirname = sys.argv[1]

hd_root_args = "  "

#for fname in glob.glob('/d/grid17/sdobbs/gluex_data/ks_skim/0306??'): # test on small batch
for fname in glob.glob('/d/grid17/sdobbs/gluex_data/ks_skim/0?????'):
	print fname
	run = int(fname[-5:])
	
	cmd = "hd_root --nthreads=8  -PPLUGINS=danarest %s -o hd_root_%06d.root %s/*.evio"%(hd_root_args, run, fname)	
	print cmd
	os.system(cmd)
	
	cmd = "mv dana_rest.hddm dana_rest_%06d.hddm"%(run)
	print cmd
	os.system(cmd)
	
	cmd = "mv hd_root_%06d.root %s/hd_root_rest_%06d.root"%(run,dirname,run)
	print cmd
	os.system(cmd)
