from ndi_utils_lib import scrape_ndi_file, write_ndi_file, compute_element
from pathlib import Path

# Get path for two data files to read in, and the output file
h801_path = Path("/usr/projects/data/nuclear/ndi/data/sn/mt80/t00/1001.801nm")
h823_path = Path("/usr/projects/data/nuclear/ndi/data/sn/mt80/t22/1001.823nm")
output_path = Path.cwd() / "1001.test"

# "Scrape" data files to get all of their information.
h801_dict = scrape_ndi_file(h801_path)
h823_dict = scrape_ndi_file(h823_path)

# Replace t22's weights with t00
h823_dict['wgts'] = h801_dict['wgts']

# Print out new NDI data file
write_ndi_file(h823_dict, output_path)

# Use both dictionaries to compute a fictional "elemental"
output_path_elemental = Path.cwd() / "1001.elemental"
elemental_dict = compute_element([h801_dict, h823_dict], [0.9, 0.1])
write_ndi_file(elemental_dict,output_path_elemental)
