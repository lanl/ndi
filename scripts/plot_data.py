import os
import sys
sys.path.append("/usr/projects/data/nuclear/ndi/2.2.1/x86_64-linux/bin")
import ndicl
import matplotlib.pyplot as plt

if __name__== '__main__':

# open_gendir instantiates NDI 
    g = ndicl.open_gendir("/usr/projects/data/nuclear/ndi/2.2.1/share/gendir.all")
# make a multigroup neutron handle
    d = g.make_handle("multigroup_neutron")

# Optional command to set a group collapse (currently will get the default
#   of 618 energy groups
#    d.set_option("NDI_COLLAPSE", "30_lanl")

# Choose the NDI library
    d.set_option("NDI_LIBRARY", "mt80")
# Select the isotope. Leaving off the extension (802nm) will result in
# the first isotope found with that ZA.
    d.set_isotope("92235.802nm")

# Get the energy group boundaries for this ZAID
    e_bounds = d.get_float_vec("NDI_E_BOUNDS")
# This is where you choose what data you want to plot
#  - get_float_vec("NDI_WGTS") will get the weight function
#  - get_float_vec("NDI_SIG_TOT") will get the total cross section
#    data = d.get_float_vec("NDI_WGTS")
# Alternately, use get_float_vec_n to get reaction XS or scattering
#  - get_float_vec_n("NDI_SIG_REAC", MT) requires a number in place of MT
#    that corresponds to the MT number for the requested cross section.
    data = d.get_float_vec_n("NDI_SIG_REAC",102)

# Scale by the energy bin width to avoid weird jumps in the data. Important
# for plotting weight functions
#    for i in range(0, len(data)):
#        data[i] = data[i]/(e_bounds[i]-e_bounds[i+1])

# Copy the final value for the step function plotting
    data.append(data[-1])

# Plot as a step function
    plt.step(e_bounds, data)

# Use log scales for plotting cross sections and weight functions
    plt.yscale("log")
    plt.xscale("log")
# Can change y-label appropriately
    plt.ylabel("Data")
# Everything NDI does is in MeV
    plt.xlabel("Energy (MeV)")
    plt.title("Weight function")
    plt.show()
