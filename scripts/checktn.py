#! /usr/bin/python

# Tell python where to find ndicl
import sys
#sys.path.append('/usr/projects/data/nuclear/ndi/2.1.4/x86_64-linux/bin')
sys.path.append('/usr/projects/data/nuclear/special/tgsaller/ndicl/python')

import ndicl

def trunc(list):
	"""return string containing either list values
	if less than five of them or first five values
	and elipsis"""
	l = len(list)
	if l <= 5:
		return ("%s "*l) % tuple(list)
	else:
		return ("%s "*5 + "...") % tuple(list[:5])

#g = ndicl.open_gendir("/yellow/usr/projects/data/nuclear/ndi/devel/lanl04cj/gendir.out")
g = ndicl.open_gendir("/usr/projects/data/nuclear/ndi/2.2.0alpha/share/gendir.all")

g.set_option_gendir("NDI_LIB_TYPE_DEFAULT", "tn")
g.set_option_gendir("NDI_LIBRARY_DEFAULT", "lanl01")
tn_list = g.get_string_vec_x("NDI_ZAID", "")

print("HEY %s" % tn_list)

d = g.make_handle("tn")

for za in tn_list:
	print("input zaid: %s" % za)
	d.set_reaction(za)
	zaid = d.get_string_val("NDI_ZAID")
	print("zaid\n\t%s" % zaid)
	library_name = d.check_option("NDI_LIBRARY")
	print("library_name\n\t%s" % library_name)
	date_source = d.get_string_val("NDI_DATE_SOURCE")
	print("date_source\n\t%s" % date_source)
	date_processed =  d.get_string_val("NDI_DATE_PROCESSED")
	print("date_processed\n\t%s" % date_processed)
	info = d.get_string_val("NDI_INFO")
	print("info\n\t%s" % info)
	num_temps = d.get_size("NDI_TEMPS")
	num_temps2 = d.get_int_val("NDI_NUM_TEMPS")
	print("num_temps (size)\n\t%s" % num_temps)
	print("num_temps (int)\n\t%s" % num_temps2)
	temps = d.get_float_vec("NDI_TEMPS") # (MeV)
	print("temps\n\t%s" % trunc(temps))
	einbars = d.get_float_vec("NDI_EINBAR")
	print("einbars\n\t%s" % trunc(einbars)) # (MeV)
	print("int_einbars")
	print("\t%s"% d.get_size("NDI_EINBAR_INTERP_REG"))
	r = d.get_int_vec("NDI_EINBAR_INTERP_REG")
	t = d.get_int_vec("NDI_EINBAR_INTERP_TYPE")
	for x in zip(r, t):
		print("\t%s %s" % x)
	sigvbars = d.get_float_vec("NDI_SIGVBARS")
	print("sigvbars\n\t%s" % trunc(sigvbars)) # (cm^3/s)
	print("int_sigvbars")
	print("\t%s"% d.get_size("NDI_SIGVBAR_INTERP_REG"))
	r = d.get_int_vec("NDI_SIGVBAR_INTERP_REG")
	t = d.get_int_vec("NDI_SIGVBAR_INTERP_TYPE")
	for x in zip(r, t):
		print("\t%s %s" % x)

	print("reactants")
	n = d.get_int_val("NDI_NUM_REACTANTS")
	print("\t%s" % n)
	r = d.get_int_vec("NDI_REACTANTS_ID")
	m = d.get_int_vec("NDI_REACTANTS_MLT")
	print("\t")
	for x in zip(r, m):
		print("%s %s " % x)
	print()
	print("rprods")
	n = d.get_int_val("NDI_NUM_RPRODS")
	print("\t%s" % n)
	r = d.get_int_vec("NDI_RPRODS_ID")
	m = d.get_int_vec("NDI_RPRODS_MLT")
	print("\t")
	for x in zip(r, m):
		print("%s %s " % x)
	print()

	print("reac_q")
	reac_q = d.get_float_val("NDI_REAC_Q")
	print("\t%s" % reac_q)

	print("num_sec_parts")
	num_sec_parts = d.get_int_val("NDI_NUM_SEC_PARTS")
	print("\t%s" % num_sec_parts)

	for x in range(0, num_sec_parts):
		print("sec_part number %d " % x)
		print("sec_part_type")
		sec_part_type = d.get_string_val_n("NDI_SEC_PART_TYPES",x)
		print("\t%s" % sec_part_type)

		d.set_option("NDI_CURR_PART",sec_part_type)
		d.set_option("NDI_TEMP", temps[0])
		d.set_option("NDI_COLLAPSE", "30_lanl")

		print("e_dist_temps")
		edist_temps = d.get_float_vec("NDI_EDIST_TEMPS")
		print("\t%s" % trunc(edist_temps))

		print("e_dist_interp_reg")
		edist_interp_reg = d.get_int_vec("NDI_EDIST_INTERP_REG")
		print("\t%s" % trunc(edist_interp_reg))

		print("e_dist_interp_type")
		edist_interp_type = d.get_int_vec("NDI_EDIST_INTERP_TYPE")
		print("\t%s" % trunc(edist_interp_type))

		print("e_dist_cont_e")
		edist_cont_e = d.get_float_vec("NDI_EDIST_CONT_E")
		print("\t%s" % trunc(edist_cont_e))

		print("e_dist_cont_interp_reg")
		edist_cont_interp_reg = d.get_int_vec("NDI_EDIST_CONT_INTERP_REG")
		print("\t%s" % trunc(edist_cont_interp_reg))

		print("e_dist_cont_p")
		edist_cont_p = d.get_float_vec("NDI_EDIST_CONT_P")
		print("\t%s" % trunc(edist_cont_p))

		print("e_dist")
		edist = d.get_float_vec("NDI_EDIST")
		print("\t%s" % trunc(edist))

		print("e_outbar")
		eoutbar = d.get_float_val("NDI_EOUTBAR")
		print("\t%s" % eoutbar)


	# Need "sec_part_types", "e_edist_1", etc.i

	print("end")
