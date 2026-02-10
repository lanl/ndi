from datetime import date
import numpy as np

def write_data(fin, fout, num_elements):
    n = 0
    while n < num_elements:
        fout.write("   ")
        line = fin.readline()
        array = line.split()
        m = 0
        while m < len(array) and (m + n) < num_elements:
            fout.write(" %8.6e" % (float(array[m])))
            m = m + 1
        fout.write("\n")
        n = n + len(array)
    print n

def parse_input(fin, fout):

	lib_name = "dedx"
	date_src = "unknown"

	# Block 1

	# First line, charged particle
	line = fin.readline()
	proj_zaid = int(line)
	if proj_zaid < 1001:
		print "Error, invalid ZAID"
		return

	# Second line. Atomic Number, atomic weight, mass in (g)
	line = fin.readline()
	array = line.split()
	proj_z = array[0]
	at_wgt = array[1]

	# Third line. Number of energies, number of densities, number of temperatures
	line = fin.readline()
	array = line.split()
	num_e = array[0]
	num_den = array[1]
	num_temp = array[2]
	num_tot = float(array[0])*float(array[1])*float(array[2])

	# Fourth line. Reciprocal of log energy spacing factor, reciprocoal of log
	# density spacing factor, and reciprocal of log temperature spacing factor.
	# Unnecessary.
	line = fin.readline()

	# print header information
	fout.write("zaid\n    ")
	fout.write(str(proj_zaid)+"000cm\n")
	fout.write("info\n    ")
	fout.write("This data has been converted from ACE to NDI\n")
	fout.write("library_name\n    ")
	fout.write(lib_name + "\n")
	fout.write("date_source\n    ")
	fout.write(date_src+"\n")
	fout.write("date_processed\n    ")
	fout.write(str(date.today())+"\n")
	fout.write("awr\n    ")
	fout.write(str(float(at_wgt) / 1.0086649157855) + "\n") # convert to atomic weight ratio
	fout.write("at_wgt\n    ")
	fout.write(at_wgt + "\n")
	fout.write("num_grps\n    ")
	fout.write(num_e+"\n")
	fout.write("num_dens\n    ")
	fout.write(num_den+"\n")
	fout.write("num_temps\n    ")
	fout.write(num_temp+"\n")

	# Block 2. Energy, number density, and temperature grid (log)
	fout.write("energies\n")
	write_data(fin, fout, int(num_e))
	fout.write("densities\n")
	write_data(fin, fout, int(num_den))
	fout.write("temperatures\n")
	write_data(fin, fout, int(num_temp))

	# Block 3. Log of dE/dx values for free electrons
	fout.write("free_e_dedx\n")
	write_data(fin, fout, num_tot)

	# Block 4. Number of target ions
	line = fin.readline()
	num_targets = int(line)
	if ( int(num_targets) < 0 ):
		print "Invalid number of targets"
		return

	fout.write("num_targets\n    ")
	fout.write(str(num_targets) + "\n")


	fout.write("target_ion_dedx\n    ")

	for i in range(0, num_targets):
		# Line 1, target ZAID
		line = fin.readline()
		targ_zaid = int(line)
		if ( targ_zaid < 1000 ):
			print "Invalid target ZAID"
		fout.write("    " + str(targ_zaid) + "\n")
		# Line 2, Z-number, atomic weight, mass (ignored)
		line = fin.readline()
		# Data block
		write_data(fin, fout, num_tot)
	fout.write("end")

input_file = 'data'
output_file = 'ndi'

fin = open(input_file, 'r')
fout = open(output_file, 'w')

print "Write header information"

fout.write("dE/dx information translated from ACE to NDI\n")

parse_input(fin, fout)

fin.close()
fout.close()

