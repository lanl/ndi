from ndi_utils_lib import *
from pathlib import Path
import os

temps = ['00','01','02','03','04','05','06','07','08','09',
         '10','11','12','13','14','15','16','17','18','19',
         '20','21','22']
temps1 = ['0','1','2','3','4','5','6','7','8','9',
         '10','11','12','13','14','15','16','17','18','19',
         '20','21','22']


for temp in range(0, len(temps)):
    t = temps[temp]
    t1 = temps1[temp]
    files = os.listdir("/usr/projects/data/nuclear/ndi/special/endf8_ss/t"+t)
    wfs0 = Path("./good_weights/001-H-001.ndi.t"+t1+"s0")
    wfs0_dict = scrape_ndi_file(wfs0)
    wfs1 = Path("./good_weights/001-H-001.ndi.t"+t1+"s1")
    wfs1_dict = scrape_ndi_file(wfs1)
    wfs2 = Path("./good_weights/001-H-001.ndi.t"+t1+"s2")
    wfs2_dict = scrape_ndi_file(wfs2)
    wfs3 = Path("./good_weights/001-H-001.ndi.t"+t1+"s3")
    wfs3_dict = scrape_ndi_file(wfs3)
    wfs4 = Path("./good_weights/001-H-001.ndi.t"+t1+"s4")
    wfs4_dict = scrape_ndi_file(wfs4)
    wfs5 = Path("./good_weights/001-H-001.ndi.t"+t1+"s5")
    wfs5_dict = scrape_ndi_file(wfs5)
    wfs6 = Path("./good_weights/001-H-001.ndi.t"+t1+"s6")
    wfs6_dict = scrape_ndi_file(wfs6)
    wfs7 = Path("./good_weights/001-H-001.ndi.t"+t1+"s7")
    wfs7_dict = scrape_ndi_file(wfs7)
    wfs8 = Path("./good_weights/001-H-001.ndi.t"+t1+"s8")
    wfs8_dict = scrape_ndi_file(wfs8)
    wfs9 = Path("./good_weights/001-H-001.ndi.t"+t1+"s9")
    wfs9_dict = scrape_ndi_file(wfs9)


    for f in files:
        input_file = Path("/usr/projects/data/nuclear/ndi/special/endf8_ss/t"+t+"/"+f)
        output_file = Path("./t"+t+"/"+f)
        if os.path.exists("./t"+t+"/"+f):
            print("Skipping " + f)
            continue
        try:
            input_dict = scrape_ndi_file(input_file)
            output_dict = scrape_ndi_file(input_file)
        except:
            continue

        if "s0" in f:
            output_dict['wgts'] = wfs0_dict['wgts']
        if "s1" in f:
            output_dict['wgts'] = wfs1_dict['wgts']
        if "s2" in f:
            output_dict['wgts'] = wfs2_dict['wgts']
        if "s3" in f:
            output_dict['wgts'] = wfs3_dict['wgts']
        if "s4" in f:
            output_dict['wgts'] = wfs4_dict['wgts']
        if "s5" in f:
            output_dict['wgts'] = wfs5_dict['wgts']
        if "s6" in f:
            output_dict['wgts'] = wfs6_dict['wgts']
        if "s7" in f:
            output_dict['wgts'] = wfs7_dict['wgts']
        if "s8" in f:
            output_dict['wgts'] = wfs8_dict['wgts']
        if "s9" in f:
            output_dict['wgts'] = wfs9_dict['wgts']

        print("Writing " + str(output_file))
        write_ndi_file(output_dict, output_file)
        
input_file = Path("/usr/projects/data/nuclear/ndi/special/endf8_ss/t00/001-H-001.ndi.t0s0")
output_file = Path("/usr/projects/data/nuclear/ndi/special/endf8_ss_fixed/t00/001-H-001.ndi.t0s0")

print(input_file)

input_dict = scrape_ndi_file(input_file)

write_ndi_file(input_dict, output_file)
