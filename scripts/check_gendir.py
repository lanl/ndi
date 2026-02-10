#!/usr/bin/python

# Script to check if all entries in a gendir file are valid.

# Import
import sys
import os.path
from os import path

# This currently does not test for missing data in a gendir entry.
# I.e., it does not check if each entry has a "t="
# It also does not test whether or not the library is an actual library,
# it just checks if it's a string.

# Errors
# -1 = keyword (.e.g, z= or awr=) not valid
# -2 = repeated keyword
# -3 = invalid zaid
# -4 = invalid zaid extension (number), e.g. 800
# -5 = invalid zaid extension (characters), e.g. nm, ztn
# -6 = invalid library
# -7 = data file does not exist (bad path)
# -8 = File type /= asc or bin (ASCII or binary)
# -9 = number not a number
# -10 = number < 0
# -11 = number < 1
# -12 = number <= 0

# Test if something is a float (instead of a word or something. Works for integers too)
def is_float(string):
	try:
		x = float(string)
		return True
	except ValueError:
		return False

# Check if the z= word is a zaid (or reaction type).
# Note that this does NOT check if a non-zaid is valid.
# This includes TN reactions or radchem identifier.
# Either way, it will check if the zaid extension is valid
# (i.e., AAAbbb, e.g. 800nm or 389zrc)
def check_zaid(string, type):
	print("Check zaid " + string)
	error = 0
	strings = string.split(".")
	# If it isn't multigroup (either neutron or photon) or neutron dosimetry,
	# there is too wide of a range of acceptable values to test (without much
	# more work).
	if type == "mg" or type == "dos":
		if int(strings[0]) != 1 and int(strings[0]) < 1000:
			error = -3
	if len(strings) > 1:
		extension = strings[1]
		le = len(extension)
		# Note that for NDI this should be a AAAbb or AAAbbb format.
		# AAA is a three digit number, bb(b) is a character extension.
		# This is what we'll check for.
		if not extension[0:3].isdigit():
			error = -4
		postfix = extension[3:le]
		# If other types are added to NDI, this will need to be updated.
		if not (postfix == "nm" or postfix == "gm" or postfix == "ztn" or postfix == "ztr" or postfix == "zpd" or postfix == "zrc" or postfix == "ny" or postfix == "dx" or postfix == "nmb"):
			error = -5
	return error
# Just check if the library is a string
def check_library(string):
	error = 0
	print("Check library " + string)
	if len(string) == 0:
		error = -6
	return error
# Check if the file exists
def check_file(string):
	print("Check file " + string)
	error = 0
	# Expand any environment variables
	expanded_path = path.expandvars(string)
	if not path.exists(expanded_path):
		error = -7
	return error
# Check if the filetype is ASCII or binary
def check_filetype(string):
	print("Check file type " + string)
	error = 0
	if string != "asc" and string != "bin":
		error = -8
	return error
# Check the line number
def check_line(string):
	print("Check line number " + string)
	error = 0
	if not string.isdigit():
		error = -9
	elif int(string) < 0:
		error = -10
	return error
# Check the offset
def check_offset(string):
	print("Check offset " + string)
	error = 0
	if not string.isdigit():
		error = -9
	elif int(string) < 0:
		error = -10
	return error
# Check the number of groups
def check_groups(string):
	print("Check number of groups " + string)
	error = 0
	if not string.isdigit():
		error = -9
	elif int(string) < 1:
		error = -11
	return error
# Check the temperature
def check_temperature(string):
	print("Check temperature " + string)
	error = 0
	if not is_float(string):
		error = -9
	elif float(string) < 0:
		error = -10
	return error
# Check the background cross section sig0
def check_sig0(string):
	print("Check sig0 " + string)
	error = 0
	if not is_float(string):
		error = -9
	elif float(string) < 0:
		error = -10
	return error
# Check the atomic weight ratio
def check_awr(string):
	print("Check awr " + string)
	error = 0
	if not is_float(string):
		error = -9
	elif float(string) <= 0:
		error = -12
	return error
# Check the atomic weight
def check_aw(string):
	print("Check aw " + string)
	error = 0
	if not is_float(string):
		error = -9
	elif float(string) <= 0:
		error = -12
	return error

# %%%%%%%%%%%%%%%%%% MAIN CODE STARTS HERE %%%%%%%%%%%%%%%%%%%%%

# Get gendir file name from script arguments

if len(sys.argv) > 2:
	print ("Error, too many command line arguments")
	exit()
if len(sys.argv) == 1:
	print ("No gendir file path entered.")
	gendir = raw_input("Enter gendir path: ")
else:
	strings = sys.argv
	gendir = strings[1]
print ("Gendir path: " + gendir)

# Open error output file
output_file = open("errors.out", 'w')

# Open gendir file
gendir_file = open(gendir, 'r')

# Read first line
line = gendir_file.readline()

while line:
	# Ignore return character
	line_string = line[0:len(line)-1]
	# Check for section
	if line_string == "multigroup_neutron" or \
	   line_string == "multigroup_photon" or \
	   line_string == "tndata" or \
	   line_string == "tn_reactions" or \
	   line_string == "neutron_depletion" or \
	   line_string == "radchem_chain" or \
	   line_string == "dosimetry_neutron":
		zaid = ""
		data_type = ""
		# Set data type if necessary
		if line_string == "multigroup_neutron" or \
		   line_string == "multigroup_photon":
			data_type = "mg"
		elif line_string == "dosimetry_neutron":
			data_type = "dos"
		# We want to make sure there are not any repeated entries
		# in a single gendir block.
		#
		# zaid, date, library, file, file type, line number, offset
		# number of groups, temperature, sig0, atomic weight,
		# atomic weight ratio
		fz = False
		fd = False
		fl = False
		ff = False
		fft = False
		fln = False
		fo = False
		fng = False
		ft = False
		fs0 = False
		faw = False
		fawr = False
		# Go line by line
		while line and line_string != "end":
			line = gendir_file.readline()
			# Split line into entries
			strings = line.split()
			for string in strings:
				ls = len(string)
				# Check by each XX=
				if string[0:2] == "z=":
					zaid = string[2:ls]
					error = check_zaid(string[2:ls], data_type)
					if fz == True:
						error = -2
					fz = True
				elif string[0:2] == "d=":
					# Okay. Who cares?
					if fd == True:
						error = -2
					fd = True
					continue
				elif string[0:2] == "l=":
					error = check_library(string[2:ls])
					if fl == True:
						error = -2
					fl = True
				elif string[0:2] == "f=":
					error = check_file(string[2:ls])
					if ff == True:
						error = -2
					ff = True
				elif string[0:3] == "ft=":
					error = check_filetype(string[3:ls])
					if fft == True:
						error = -2
					fft = True
				elif string[0:3] == "ln=":
					error = check_line(string[3:ls])
					if fln == True:
						error = -2
					fln = True
				elif string[0:2] == "o=":
					error = check_offset(string[2:ls])
					if fo == True:
						error = -2
					fo = True
				elif string[0:3] == "ng=":
					error = check_groups(string[3:ls])
					if fng == True:
						error = -2
					fng = True
				elif string[0:2] == "t=":
					error = check_temperature(string[2:ls])
					if ft == True:
						error = -2
					ft = True
				elif string[0:3] == "s0=":
					error = check_sig0(string[3:ls])
					if fs0 == True:
						error = -2
					fs0 = True
				elif string[0:4] == "awr=":
					error = check_awr(string[4:ls])
					if fawr == True:
						error = -2
					fawr = True
				elif string[0:3] == "aw=":
					error = check_aw(string[3:ls])
					if faw == True:
						error = -2
					faw = True
				# at the end of an entry, reset all present flags
				elif string[0:3] == "end":
					fz = False
					fd = False
					fl = False
					ff = False
					fft = False
					fln = False
					fo = False
					fng = False
					ft = False
					fs0 = False
					faw = False
					fawr = False
					break
				# keyword not found
				else:
					error = -1
				# if an error is returned, print where it happens and
				# for what zaid
				if error != 0:
					print ("Error " + str(error) + " for zaid " + zaid + " and entry " + string)
					output_file.write("Error " + str(error) + " for zaid " + zaid + " and entry " + string + "\n")
#					exit()
			line_string = line[0:len(line)-1]
	line = gendir_file.readline()

# Close
gendir_file.close()

