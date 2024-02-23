import os,sys
import glob

if len(sys.argv) < 2:
    print "need to pass input directory!" 
dirname = sys.argv[1]

for fname in glob.glob("%s/dana_rest_*.hddm"%dirname):
    #print fname
    tokens = fname.split('/')
    #print tokens[-1]
    run = int(tokens[-1][11:16])
    #print run
    #break

    cmd = "hd_root --nthreads=12 --config=analysis.config -o hd_root_%06d.root %s"%(run,fname)
    print cmd
    os.system(cmd)
    #break

    cmd = "mv tree_pimkpks__B4_M16.root %s/tree_pimkpks__B4_M16_%06d.root"%(dirname,run)
    print cmd
    os.system(cmd)

    cmd = "mv hd_root_%06d.root %s/hd_root_%06d.root"%(run,dirname,run)
    print cmd
    os.system(cmd)
