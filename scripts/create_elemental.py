# Tell python where to find ndicl
import sys
import os
from datetime import date
sys.path.append('/usr/projects/data/nuclear/ndi/2.2.4/x86_64-linux/bin')
import numpy as np
import ndicl

def write_data(keyword, data, fout):
    print(keyword)
    fout.write(keyword)
    fout.write("\n")
    if type(data) == type("string"):
        fout.write(" " + data)
    elif type(data) == type([float]) or type(data) == np.ndarray:
        if type(data) == np.ndarray:
            if len(data.shape) == 2:
                index = 0
                for i in range(0, data.shape[0]):
                    for j in range(0, data.shape[1]):
                        index = index + 1
                        fout.write(" " + str(data[i][j]))
                        if (index+1) % 4 == 0 and index != len(data)-1:
                            fout.write("\n")
            else:
                for i in range(0, len(data)):
                    fout.write(" " + str(data[i]))
                    if (i+1) % 4 == 0 and i != len(data)-1:
                        fout.write("\n")
        else:
            for i in range(0, len(data)):
                fout.write(" " + str(data[i]))
                if (i+1) % 4 == 0 and i != len(data)-1:
                    fout.write("\n")
    else:
        fout.write(" " + str(data))
    fout.write("\n")


def get_fission(n):
    # Get all of the fission data

    haveprompt = False

    nu_pr = n.get_float_vec("NDI_NU_PR")
    if n.err() == 0:
        haveprompt = True

    havedelayed = False

    nu_del = n.get_float_vec("NDI_NU_DEL")
    if n.err() == 0:
        havedelayed = True

    nu_tot = n.get_float_vec("NDI_NU_TOT")
#    if not (haveprompt and havedelayed):
#        nu_tot = n.get_float_vec("NDI_NU_TOT")

    haveprompt = False

    chi_mat_pr = n.get_float_vec("NDI_CHI_MAT_PR")
    if n.err() == 0:
        haveprompt = True
    elif n.err() == 10:
        chi_vec_pr = n.get_float_vec("NDI_CHI_VEC_PR")
        haveprompt = True

    havedelayed = False

    chi_mat_del = n.get_float_vec("NDI_CHI_MAT_DEL")
    if n.err() == 0:
        havedelayed = True
    elif n.err() == 10:
        chi_vec_del = n.get_float_vec("NDI_CHI_VEC_DEL")
        havedelayed = True

    chi_mat_tot = n.get_float_vec("NDI_CHI_MAT_TOT")
    if n.err() == 10:
       chi_vec_tot = n.get_float_vec("NDI_CHI_VEC_TOT")
#    if not (haveprompt and havedelayed):
#        chi_mat_tot = n.get_float_vec("NDI_CHI_MAT_TOT")
#        if n.err() == 10:
#            chi_vec_tot = n.get_float_vec("NDI_CHI_VEC_TOT")

    fiss_q = n.get_float_vec("NDI_FISS_Q")

    return nu_pr, nu_del, nu_tot, chi_mat_pr, chi_mat_del, chi_mat_tot, fiss_q


def get_scattering(n, ng):
    pn_order = n.get_int_val("NDI_PN_ORDER")
    num_grps = n.get_int_val("NDI_NUM_GRPS")

    scattering = np.zeros([pn_order,num_grps,num_grps])

    for i in range(0, pn_order):
        scatt = n.get_float_vec_n("NDI_PN_FULL",i)
        scattering[i] = np.array(scatt).reshape((ng,ng))

    return pn_order, scattering
    
def get_reactions(n):
    num_reac = n.get_int_val("NDI_NUM_REAC")
    num_grps = n.get_int_val("NDI_NUM_GRPS")
    reac_num = np.zeros(num_reac)
    q = np.zeros(num_reac)
    reac_xs = np.zeros([num_reac,num_grps])

    for i in range(0, num_reac):
        reac_num[i] = n.get_int_val_n("NDI_REAC_MT",i)
        q[i] = n.get_float_val_n("NDI_REAC_Q",reac_num[i])
        reac_xs[i] = n.get_float_vec_n("NDI_SIG_REAC",reac_num[i])

    return num_reac, reac_num, q, reac_xs
    
def get_secondaries(n,ng):
    num_sec_parts = n.get_int_val("NDI_NUM_SEC_PARTS")
#    sec_part_types = n.get_int_vec("NDI_SEC_PART_TYPES")
    sec_part_type = [None] * num_sec_parts
    sec_part = [None] * num_sec_parts
    num_grps_sec_part = [None] * num_sec_parts
    pn_order_sec_part = [None] * num_sec_parts
    kerma_sec_part = [None] * num_sec_parts
    ebounds_sec_part = [None] * num_sec_parts
    pn_prod_full_sec_part = [None] * num_sec_parts
    for i in range(0, num_sec_parts):
        sec_part_type[i] = n.get_string_val_n("NDI_SEC_PART_TYPES",i)
        n.set_option("NDI_CURR_PART",sec_part_type)
        sec_part[i] = n.check_option("NDI_CURR_PART")
        print("B1")
        print(sec_part)
        num_grps_sec_part[i] = n.get_int_val("NDI_NUM_GRPS_CURR_PART")
        pn_order_sec_part[i] = n.get_int_val("NDI_PN_ORDER_CURR_PART")
        kerma_sec_part[i] = n.get_float_vec("NDI_KERMA_CURR_PART")
        print(len(kerma_sec_part[i]))
        ebounds_sec_part[i] = n.get_float_vec("NDI_E_BOUNDS_CURR_PART")
        pn_prod_full_sec_part[i] = np.zeros([pn_order_sec_part[i],num_grps_sec_part[i]*ng])
        for s in range(0, pn_order_sec_part[i]):
            pn_prod_full_sec_part[i][s] = n.get_float_vec_n("NDI_PN_PROD_FULL_CURR_PART",s)

    print("A1")
    print(sec_part)
    return [num_sec_parts, sec_part_type, num_grps_sec_part, pn_order_sec_part, kerma_sec_part, ebounds_sec_part, pn_prod_full_sec_part]

def elementalize(g, n, Z, abundance, ext, ng, ngg, zaids):

    # File name is elemental Z + 000 (for elemental) + extension
    filename = "new_data/"+str(Z)+"000."+ext
    if os.path.exists(filename):
        return
    print(filename)
    fout = open(filename,'w')

    awr = 0.0
    at_wgt = 0.0
    up_grps = 0
    down_grps = 0
    sig_tot = np.zeros(ng)
    kerma = np.zeros(ng)
    nu_pr = np.zeros(ng)
    nu_del = np.zeros(ng)
    nu_tot = np.zeros(ng)
    chi_mat_pr = np.zeros([ng,ng])
    chi_mat_del = np.zeros([ng,ng])
    chi_mat_tot = np.zeros([ng,ng])
    fiss_q = np.zeros(6)
    pn_order = 0
    scattering = np.zeros([10,ng,ng])
    num_reac = 0
    reac_num = np.zeros(1000)
    q = np.zeros(1000)
    reac_xs = np.zeros([1000,ng])
    sec_part_fail = True
    first_sec_part = True

    abunds = ''

    for zaid in zaids:
        dot = zaid.find(".")
        z = int(np.floor(int(zaid[0:dot])/1000))
        a = int(zaid[0:dot])-z*1000

        if int(Z) == int(z) and zaid[dot+1:] == ext:
            try:
                print(abundance[a])
            except:
                continue
            abound = abundance[a] / sum(abundance.values())
            abunds = abunds + str(zaid[0:dot]) + ": " + str(abound) + "; "
            print("A2 " + str(abound) + " " + str(abundance[a]) + " " + str(sum(abundance.values())))
            n.set_isotope(zaid)

            iso_awr = n.get_float_val("NDI_AWR")
            iso_at_wgt = n.get_float_val("NDI_AT_WGT")

            awr += iso_awr*abound
            at_wgt += iso_at_wgt*abound

            ug_iso = n.get_int_val("NDI_UP_GRPS")
            up_grps = max(up_grps, ug_iso)

            dg_iso = n.get_int_val("NDI_DOWN_GRPS")
            down_grps = max(down_grps, dg_iso)

            st = n.get_float_vec("NDI_SIG_TOT")
            kr = n.get_float_vec("NDI_KERMA")
            
            fissionable = 1==n.get_int_val("NDI_FISSIONABLE")

            if fissionable:
                [nu_pr_iso, nu_del_iso, nu_tot_iso, chi_mat_pr_iso, chi_mat_del_iso, chi_mat_tot_iso, fiss_q_iso] = get_fission(n)
            [pn_order_iso, scattering_iso] = get_scattering(n, ng)
            [num_reac_iso, reac_num_iso, q_iso, reac_xs_iso] = get_reactions(n)
            try:
                [num_sec_parts, sec_part_type, num_grps_sec_part, pn_order_sec_part, kerma_sec_part, ebounds_sec_part, pn_prod_full_sec_part] = get_secondaries(n, ng)
                has_sec_part = True
            except:
                has_sec_part = False

            pn_order = max(pn_order, pn_order_iso)

            for g in range(0, ng):
                sig_tot[g] += st[g]*abound
                kerma[g] += kr[g]*abound
                if fissionable:
                    nu_pr[g] += nu_pr_iso[g]*abound
                    nu_del[g] += nu_del_iso[g]*abound
                    nu_tot[g] += nu_tot_iso[g]*abound

                for r in range(0, num_reac_iso):
                    reac_num = int(reac_num_iso[r])
                    reac_xs[reac_num][g] += reac_xs_iso[r][g]*abound
                    if g == 0:
                        q[reac_num] += q_iso[r]*abound

                for k in range(0, ng):
                    index = g*ng+k
                    if fissionable:
                        chi_mat_pr[g,k] += chi_mat_pr_iso[index]*abound
                        chi_mat_del[g,k] += chi_mat_del_iso[index]*abound
                        chi_mat_tot[g,k] += chi_mat_tot_iso[index]*abound

                    for p in range(0, pn_order_iso):
                        scattering[p][g][k] += scattering_iso[p][g][k]*abound

            if fissionable:
                for i in range(0, 6):
                    fiss_q[i] += fiss_q_iso[i]*abound
   
            if fissionable:
                for g in range(0, ng):
                    sum1 = 0.0
                    sum2 = 0.0
                    sum3 = 0.0
                    for k in range(0, ng):
                        sum1 += chi_mat_pr[g,k]
                        sum2 += chi_mat_del[g,k]
                        sum3 += chi_mat_tot[g,k]

                    for k in range(0, ng):
                        chi_mat_pr[g,k] = chi_mat_pr[g,k] / sum1
                        chi_mat_del[g,k] = chi_mat_del[g,k] / sum2
                        chi_mat_tot[g,k] = chi_mat_tot[g,k] / sum3

            if first_sec_part and has_sec_part:
                first_sec_part = False
                sec_part_fail = False
                # Using 10 for maximum secondary particles
                max_sec_parts = 1
                all_sec_part_type = [None] * max_sec_parts
                all_num_grps_sec_part = [None] * max_sec_parts
                all_ebounds_sec_part = [None] * max_sec_parts
                all_pn_order_sec_part = [None] * max_sec_parts
                all_kerma_sec_part = [None] * max_sec_parts
                all_pn_prod_full_sec_part = [None] * max_sec_parts

                for i in range(0, num_sec_parts):
                    all_sec_part_type[i] = sec_part_type[i]
                    all_num_grps_sec_part[i] = num_grps_sec_part[i]
                    all_ebounds_sec_part[i] = np.zeros(num_grps_sec_part[i])
                    all_kerma_sec_part[i] = np.zeros(ng)
                    all_pn_order_sec_part[i] = pn_order_sec_part[i]
                    all_pn_prod_full_sec_part[i] = [None]*pn_order_sec_part[i]
                    for p in range(0, pn_order_sec_part[i]):
                        all_pn_prod_full_sec_part[i][p] = np.zeros(ng*num_grps_sec_part[i])
                    
            if not first_sec_part:
                for i in range(0, num_sec_parts):
                    if sec_part_type[i] != all_sec_part_type[i]:
                        print("ERROR! Secondary particle type %s not the same as particle type %s!" % (sec_part_type[i], all_sec_part_type[i]))
                    if num_grps_sec_part[i] != all_num_grps_sec_part[i]:
                        print("ERROR! Secondary particle %s number of groups not compatible between isotopes!" % (sec_part_type[i]))
                    if pn_order_sec_part[i] != all_pn_order_sec_part[i]:
                        print("ERROR! Secondary particle %s pn_order not compatible between isotopes!" % (sec_part_type[i]))
                    all_ebounds_sec_part[i] = ebounds_sec_part[i]
                    for g in range(0, ng):
                        all_kerma_sec_part[i][g] = all_kerma_sec_part[i][g] + kerma_sec_part[i][g]*abound
                    for p in range(0, all_pn_order_sec_part[i]):
                        all_pn_prod_full_sec_part[i][p] = all_pn_prod_full_sec_part[i][p] + pn_prod_full_sec_part[i][p]*abound

        else:
            continue

    for r in range(0, 1000):
        if np.any(reac_xs[r,:]) > 0.0:
            num_reac = num_reac + 1

    fout.write('ndi: multigroup version=1.1\n')

    write_data('zaid', str(Z)+"000."+ext, fout)    
    write_data('info', 'Elemental '+str(Z)+'. Abundances - '+abunds[:-2], fout)

    libname = n.get_string_val("NDI_LIBRARY_NAME")
    write_data('library_name', libname, fout)

    sourcedate = n.get_string_val("NDI_DATE_SOURCE")
    write_data('date_source', sourcedate, fout)

    processdate = n.get_string_val("NDI_DATE_PROCESSED")
    write_data('date_processed', date.today(), fout)

    write_data('awr', awr, fout)
    write_data('at_wgt', at_wgt, fout)

    temp = n.get_float_val("NDI_TEMP")
    write_data('temp', temp, fout)

    sig_0 = n.get_float_val("NDI_SIG_0")
    write_data('sig_0', sig_0, fout)

    num_grps = n.get_int_val("NDI_NUM_GRPS")
    write_data('num_grps', num_grps, fout)

    up_grps = min(num_grps-1, up_grps)
    write_data('up_grps', up_grps, fout)

    down_grps = min(num_grps-1,down_grps)
    write_data('down_grps', down_grps, fout)

    e_bounds = n.get_float_vec("NDI_E_BOUNDS")
    write_data('e_bounds', e_bounds, fout)

    wgts = n.get_float_vec("NDI_WGTS")
    write_data('wgts', wgts, fout)

    vel = n.get_float_vec("NDI_VEL")
    write_data('vel', vel, fout)

    write_data('sig_tot', sig_tot, fout)

    write_data('kerma', kerma, fout)

    fissionable = n.get_int_val("NDI_FISSIONABLE")

    if fissionable == 1:
        write_data('nu_pr', nu_pr, fout)
        write_data('nu_del', nu_del, fout)
        write_data('nu_tot', nu_tot, fout)
        write_data('chi_mat_pr', chi_mat_pr, fout)
        write_data('chi_mat_del', chi_mat_del, fout)
        write_data('chi_mat_tot', chi_mat_tot, fout)
        write_data('fiss_q', fiss_q, fout)

    write_data('pn_order', pn_order, fout)
    fout.write('pn_full\n')

    for i in range(0, pn_order):
        write_data('      ' + str(i), scattering[i], fout)
    
    if np.any(reac_xs != 0):
        write_data('num_reac', num_reac, fout)
        fout.write('sig_reac\n')

        for r in range(0, 1000):
            if np.any(reac_xs[r,:] != 0):
                print("Reaction " + str(r))
                write_data('    ' + str(r) + '    ' + str(q[r]), reac_xs[r,:], fout)

    if not sec_part_fail:
        write_data('num_sec_parts', num_sec_parts, fout)
        write_data('sec_part_types', all_sec_part_type, fout)
        for i in range(0, num_sec_parts):
            write_data('num_grps_'+str(all_sec_part_type[i]),all_num_grps_sec_part[i],fout)
            write_data('e_bounds_'+str(all_sec_part_type[i]),all_ebounds_sec_part[i],fout)
            write_data('kerma_'+str(all_sec_part_type[i]),all_kerma_sec_part[i],fout)
            write_data('pn_order_'+str(all_sec_part_type[i]),all_pn_order_sec_part[i],fout)
            fout.write('pn_prod_full_'+str(all_sec_part_type[i])+'\n')
            for pn in range(0, all_pn_order_sec_part[i]):
                write_data('      ' + str(pn), all_pn_prod_full_sec_part[i][pn],fout)

    fout.write("end")    

    print()
    print("done")

# Specify the elementals to be created, their temperature indices, and sig0 indices
elemental_Zs = [1,12,14,17,18,19,20,22,23,24,26,28,29,31,40,42,47,48,49,54,6,63,64,72,74,77,82,92]
temperatures = range(0, 23)
sig0s = range(0, 1)

# Elemental abundances from NuDat 3.0 database
abundance = {
  1: {1: .999885, 2:.000115}, # Hydrogen
  12: {24: .7899, 25: .10, 26: .1101}, # Magnesium
  14: {28: .92223, 29: .04685, 30: .03092}, # Silicon
  17: {35: .7576, 37: .2424}, # Chlorine
  18: {36: .003336, 38: .000629, 40: .996035}, # Argon 1.00001
  19: {39: .932581, 40: .000117, 41: .067302}, # Potassium
  20: {40: .9694, 42: .00647, 43: .00135, 44: .0209, 46: .00004, 48: .00187}, # Calcium
  22: {46: .0825, 47: .0744, 48: .7372, 49: .0541, 50: .0518}, # Titanium
  23: {50: .0025, 51: .9975}, # Vanadium
  24: {50: .04345, 52: .83789, 53: .09501, 54: .02365}, # Chromium
  26: {54: .05845, 56: .91754, 57: .02119, 58: .00282}, # Iron
  28: {58: .6808, 60: .2622, 61: .0114, 62: .0363, 64: .0093}, # Nickel 1.00001
  29: {63: .6915, 65: .3085}, # Copper
  31: {69: .6011, 71: .3989}, # Gallium
  40: {90: .5145, 91: .1122, 92: .1715, 94: .1738, 96: .028}, # Zirconium
  42: {92: .1453, 94: .0915, 95: .1584, 96: .1667, 97: .096, 98: .2439, 100: .0982}, # Molybdenum 
  47: {107: .5184, 109: .4816}, # Silver
  48: {106: .0125, 108: .0089, 110: .1249, 111: .1280, 112: .2413, 113: .1222, 114: .2873, 116: .0749}, # Cadmium
  49: {113: .0429, 115: .9571}, # Indium
  54: {124: .001, 126: .0009, 128: .0191, 129: .264, 130: .0407, 131: .2123, 132: .2691, 134: .1044, 136: .0886}, # Xenon
  6: {12: .9893, 13: .0107}, # Carbon
  63: {151: .4781, 153: .5219}, # Europium
  64: {152: .002, 154: .0218, 155: .148, 156: .2047, 157: .1565, 158: .2484, 160: .2186}, # Gadolinium
  72: {174: .0016, 176: .0526, 177: .186, 178: .2728, 179: .1362, 180: .3508}, # Hafnium
  74: {180: .0012, 182: .265, 183: .1431, 184: .3064, 186: .2843}, # Tunsgten
  77: {191: .373, 193: .627}, # Iridium
  82: {204: 0.014, 206: .241, 207: .221, 208: .524}, # Lead 0.98614
  92: {234: 0.0001, 235: 0.0072, 238: 0.9927} # Uranium
} 

# Specify gendir and library for creating elementals
gendir = "/usr/projects/data/nuclear/ndi/data/gendir/1.1/gendir.lanl2022"
library = "lanl2022"

# Setup ndicl for multigroup neutronn = ndicl.open_gendir(gendir)
g = ndicl.open_gendir(gendir)
g.set_option("NDI_LIB_TYPE", "multigroup_neutron")
g.set_option("NDI_LIBRARY", library)
n = g.make_handle("multigroup_neutron")

# Get a list of ZAIDs in this library
zaids = g.get_string_vec("NDI_ZAID")

# Set the number of energy groups, 618 for neutrons and 103 for gammas
ng = 618
ngg = 103

# Create the output directories
if not os.path.exists("new_data"):
    os.system('mkdir new_data')
for temp in temperatures:
    t = str(temp)
    if temp < 10:
        t = '0' + t
    if not os.path.exists("new_data/t"+t):
        os.system('mkdir new_data/t'+t)

# Cycle through each elemental, each temperature, and each sig0 (currently 1)
for Z in elemental_Zs:
    print ("Z: " + str(Z))
    for temp in temperatures:
        for sig0 in sig0s:
            # The extension is library-dependent and should be updated.
            # For LANL2022, the extension starts with 300nm
            extension = "3%.2dnm" % (temp)
            elementalize(g, n, Z, abundance[Z], extension, ng, ngg, zaids)

print("DONE")
