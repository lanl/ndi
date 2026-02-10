# Tell python where to find ndicl
import sys
import os
sys.path.append('/usr/projects/data/nuclear/ndi/2.2.0/x86_64-linux/bin')
import numpy as np
import ndicl

def write_data(keyword, data, fout):
    print(keyword)
    fout.write(keyword)
    fout.write("\n")
    if type(data) == type("string"):
        fout.write(" " + data)
    elif type(data) == type([]):
        for i in range(0, len(data)):
            fout.write(" " + str(data[i]))
            if (i+1) % 4 == 0 and i != len(data)-1:
                fout.write("\n")
    else:
        fout.write(" " + str(data))
    fout.write("\n")

def write_fission(n, fout):
    # Get all of the fission data

    haveprompt = False

    nu_pr = n.get_float_vec("NDI_NU_PR")
    if n.ierr == 0:
        write_data('nu_pr', nu_pr, fout)
        haveprompt = True

    havedelayed = False

    nu_del = n.get_float_vec("NDI_NU_DEL")
    if n.ierr == 0:
        write_data('nu_del', nu_del, fout)
        havedelayed = True

    if not (haveprompt and havedelayed):
        nu_tot = n.get_float_vec("NDI_NU_TOT")
        if n.ierr == 0:
            write_data('nu_tot', nu_tot, fout)

    haveprompt = False

    chi_mat_pr = n.get_float_vec("NDI_CHI_MAT_PR")
    if n.ierr == 0:
        write_data('chi_mat_pr', chi_mat_pr, fout)
        haveprompt = True
    elif n.ierr == 10:
        chi_vec_pr = n.get_float_vec("NDI_CHI_VEC_PR")
        write_data('chi_vec_pr', chi_vec_pr, fout)
        haveprompt = True

    havedelayed = False

    chi_mat_del = n.get_float_vec("NDI_CHI_MAT_DEL")
    if n.ierr == 0:
        write_data('chi_mat_del', chi_mat_del, fout)
        havedelayed = True
    elif n.ierr == 10:
        chi_vec_del = n.get_float_vec("NDI_CHI_VEC_DEL")
        write_data('chi_vec_del', chi_vec_del, fout)
        havedelayed = True

    if not (haveprompt and havedelayed):
        chi_mat_tot = n.get_float_vec("NDI_CHI_MAT_TOT")
        if n.ierr == 0:
            write_data('chi_mat_tot', chi_mat_tot, fout)
        elif n.ierr == 10:
            chi_vec_tot = n.get_float_vec("NDI_CHI_VEC_TOT")
            write_data('chi_vec_tot', chi_vec_tot, fout)

    fiss_q = n.get_float_vec("NDI_FISS_Q")
    if max(fiss_q) > 0.0:
        write_data('fiss_q', fiss_q, fout)


def write_scattering(n, fout):
    pn_order = n.get_int_val("NDI_PN_ORDER")
    write_data('pn_order', pn_order, fout)

    fout.write("pn_full\n")
    for i in range(0, pn_order):
        scatt = n.get_float_vec_n("NDI_PN_FULL",i)
        write_data('      ' + str(i), scatt, fout)
    
def write_reactions(n, fout):
    num_reac = n.get_int_val("NDI_NUM_REAC")
    write_data('num_reac', num_reac, fout)

    fout.write("sig_reac\n")

    for i in range(0, num_reac):
        reac_num = n.get_int_val_n("NDI_REAC_MT",i)
        q = n.get_float_val_n("NDI_REAC_Q",reac_num)
        reac_xs = n.get_float_vec_n("NDI_SIG_REAC",reac_num)
        write_data('    ' + str(reac_num) + '      ' + str(q), reac_xs, fout)
    
def write_rprod(name, n, fout):
    fout.write('      ' + name + '\n')
    suffix = ""
    if 'rprod_all' in name:
        suffix = "_ALL"
    elif 'rprod_few' in name:
        suffix = "_FEW"
    elif 'rprod_rmo' in name:
        suffix = "_RMO"
    else:
        print("Error, rprod not valid")

    reacs = n.get_int_vec("NDI_REAC_MT")

    for reac in reacs:
        num_prods = n.get_int_val_n("NDI_RPROD"+suffix,reac)
        ids = n.get_int_vec_n("NDI_RPROD_ID"+suffix,reac)
        mults = n.get_int_vec_n("NDI_RPROD_MLT"+suffix,reac)
        fout.write('    ' + str(reac) + '\n')
        fout.write('      ' + str(num_prods) + '\n')
        for i in range(0, len(ids)):
            fout.write('        ' + str(ids[i]) + ' ' + str(mults[i]) + '\n')
            
def write_sec_parts(n, ng, fout):
    num_sec_parts = n.get_int_val("NDI_NUM_SEC_PARTS")

    if num_sec_parts == 0:
        return

    write_data('num_sec_parts', num_sec_parts, fout)
    list_of_parts = []

    for i in range(0, num_sec_parts):
        part = n.get_string_val_n("NDI_SEC_PART_TYPES", i)
        list_of_parts.append(part)

    write_data('sec_part_types', list_of_parts, fout)

    for i in range(0, num_sec_parts):
        part = list_of_parts[i]

        n.set_option("NDI_CURR_PART", part)
        n.set_option("NDI_COLLAPSE",str(ng)+"_lanl")
        suffix = "_" + str(part)

        num_grps = n.get_int_val("NDI_NUM_GRPS_CURR_PART")
        write_data('num_grps'+suffix, num_grps, fout)

        ebounds = n.get_float_vec("NDI_E_BOUNDS_CURR_PART")
        write_data('e_bounds'+suffix, ebounds, fout)

        kerma = n.get_float_vec("NDI_KERMA_CURR_PART")
        write_data('kerma'+suffix, kerma, fout)

        pn_order = n.get_int_val("NDI_PN_ORDER_CURR_PART")
        write_data('pn_order'+suffix, pn_order, fout)

        fout.write('pn_prod_full'+suffix+"\n")
        for j in range(0, pn_order):
            pn_data = n.get_float_vec_n("NDI_PN_PROD_FULL_CURR_PART",j)
            print("AAAAA")
            print(len(pn_data))
            write_data('      '+str(j),pn_data,fout)

        part = n.get_int_val_n("NDI_SEC_PART_TYPE", i)


def convert(n, zaid_in, zaid_out, ng):

    filename = zaid_in
    fout = open(filename,'w')

    n.set_isotope(zaid_in)
    n.set_option("NDI_COLLAPSE", str(ng)+"_lanl")

    # Initial comment line
    fout.write("Converted " + zaid_in + " (618_lanl) to " + zaid_out + " (30_lanl)\n")

    # Basic data
    write_data('zaid', zaid_out, fout)
    try:
        info = n.get_string_val("NDI_INFO")
        write_data('info', 'Originally: ' + info, fout)
    except:
        # Sometimes the info line is too long for NDI to read in. In this case, ignore it.
        write_data('info', 'Info line too long', fout)

    libname = n.get_string_val("NDI_LIBRARY_NAME")
    write_data('library_name', libname, fout)

    sourcedate = n.get_string_val("NDI_DATE_SOURCE")
    write_data('date_source', sourcedate, fout)

    processdate = n.get_string_val("NDI_DATE_PROCESSED")
    write_data('date_processed', processdate, fout)

    awr = n.get_float_val("NDI_AWR")
    write_data('awr', awr, fout)

    at_wgt = n.get_float_val("NDI_AT_WGT")
    write_data('at_wgt', at_wgt, fout)

    temp = n.get_float_val("NDI_TEMP")
    write_data('temp', temp, fout)

    sig_0 = n.get_float_val("NDI_SIG_0")
    write_data('sig_0', sig_0, fout)

    num_grps = n.get_int_val("NDI_NUM_GRPS")
    write_data('num_grps', num_grps, fout)

    up_grps = n.get_int_val("NDI_UP_GRPS")
    up_grps = min(num_grps-1, up_grps)
    write_data('up_grps', up_grps, fout)

    down_grps = n.get_int_val("NDI_DOWN_GRPS")
    down_grps = min(num_grps-1,down_grps)
    write_data('down_grps', down_grps, fout)

    # Explicitly define the new group boundaries
    e_bounds = n.get_float_vec("NDI_E_BOUNDS")
    fout.write("e_bounds\n")
    fout.write('  1.70000000000000e+01  1.50000000000000e+01  1.35000000000000e+01  1.20000000000000e+01\n')
    fout.write('  1.00000000000000e+01  7.79000000000000e+00  6.07000000000000e+00  3.68000000000000e+00\n')
    fout.write('  2.86500000000000e+00  2.23200000000000e+00  1.73800000000000e+00  1.35300000000000e+00\n')
    fout.write('  8.23000000000000e-01  5.00000000000000e-01  3.03000000000000e-01  1.84000000000000e-01\n')
    fout.write('  6.76000000000000e-02  2.48000000000000e-02  9.12000000000000e-03  3.35000000000000e-03\n')
    fout.write('  1.23500000000000e-03  4.54000000000000e-04  1.67000000000000e-04  6.14000000000000e-05\n')
    fout.write('  2.26000000000000e-05  8.32000000000000e-06  3.06000000000000e-06  1.13000000000000e-06\n')
    fout.write('  4.14000000000000e-07  1.52000000000000e-07  1.39000000000000e-10\n')

    wgts = n.get_float_vec("NDI_WGTS")
    write_data('wgts', wgts, fout)

    vel = n.get_float_vec("NDI_VEL")
    write_data('vel', vel, fout)

    sig_tot = n.get_float_vec("NDI_SIG_TOT")
    write_data('sig_tot', sig_tot, fout)

    kerma = n.get_float_vec("NDI_KERMA")
    write_data('kerma', kerma, fout)

    fissionable = n.get_int_val("NDI_FISSIONABLE")

    if fissionable == 1:
        write_fission(n, fout)
    
    write_scattering(n, fout)
    write_reactions(n, fout)

    write_rprod('rprod_all', n, fout)
    write_rprod('rprod_few', n, fout)
    write_rprod('rprod_rmo', n, fout)

    write_sec_parts(n, ng, fout)

    fout.write("end")    

    print()
    print("done")

# Select gendir with 618-group library
gendir = "./gendir.out"
# Input library
library = "lanl2022"
# Choose number of coarse groups. Currently must be a standard *_lanl group structure
numgroupscoarse = 30

# Setup ndicl for multigroup neutron
# Here we use a gendir handle to get a list of all of the ZAIDs, but a
# general multigroup_neutron handle for reading in the actual data
g = ndicl.open_gendir(gendir)
n = ndicl.ndicl(gendir)
n.set_option("NDI_LIB_TYPE", "multigroup_neutron")
n.set_option("NDI_LIBRARY", library)

# Get a list of all the ZAIDs in the gendir file.
zaids = g.get_string_vec("NDI_ZAID")

# These are the folders that the current data is in.
folders = ['t00','t01','t02','t03','t04','t05','t06','t07','t08','t09','t10','t11','t12','t13','t14','t15','t16','t17','t18','t19','t20','t21','t22']

# Create the output directories
if not os.path.exists("new_data"):
    os.system('mkdir new_data')
for temp in temperatures: 
    t = str(temp)       
    if temp < 10:       
        t = '0' + t     
    if not os.path.exists("new_data/t"+t):
        os.system('mkdir new_data/t'+t)

# Where the fine group data is:
fine_group_dir = '../new_data'

# These are the folders that the current data is in.
folders = os.listdir(fine_group_dir)

for folder in folders:
    # path1 is where the fine-group data is.
    path1 = fine_group_dir + '/' + +folder
    # path2 is where the collapsed data will go.
    path2 = folder
    # Get all isotopes at this particular temperature
    try:
        isos = os.listdir(path1)
    except:
        # If path1 is not a directory or the like, skip it
        continue

    for iso in isos:
        # Get the ZA
        m = iso.find('.')
        zaid = str(int(iso[0:m]))
        # Set the new full ZAID (with extension)
        zaid_in = zaid + '.3' + folder[1:] + 'nm'
        print("Fine group ZAID: " + zaid_in)
        zaid_out = zaid + '.3' + str(int(folder[1:])+30) + 'nm'

        if os.path.exists(folder + '/' + iso[0:m] + '.3' + str(int(folder[1:])+30) + 'nm'):
            continue

        convert(n, zaid_in, zaid_out, numgroupscoarse)
        os.system('mv ' + zaid_in + ' ' + folder + '/' + iso[0:m] + '.3' + str(int(folder[1:])+30) + 'nm')

print("DONE")
