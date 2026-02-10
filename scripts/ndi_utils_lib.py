from pathlib import Path
import numpy as np
from collections import OrderedDict
from collections import defaultdict
import textwrap

def scrape_ndi_file(ndi_path):
    ndi_dict = OrderedDict()

    with ndi_path.open('r') as ndi_file:
        for line in ndi_file:
            # if the line starts with whitespace, it isn't a section name
            # so add it to the latest section (dict key)
            if line[0].isspace():
                current_key = list(ndi_dict)[-1]
                values = line.strip().split()

                # rprod section names weirdly start indented
                if 'rprod' in line:
                    ndi_dict[line.strip()] = []
                    continue

                # if it's an rprod, sig_reac, or pn_order section, we want to
                # preserve the structure of the section
                if 'rprod' in current_key:
                    rprod_values = [int(val) for val in values]
                    ndi_dict[current_key].append(rprod_values)
                elif 'sig_reac' == current_key:
                    # each reaction header starts with more than one space
                    # ex:             2          0.0
                    if line[1].isspace():
                        reac_key = int(values[0])
                        q_val = float(values[1])
                        ndi_dict['sig_reac'][reac_key] = [q_val, []]
                    else:
                        current_reac_key = list(ndi_dict['sig_reac'])[-1]
                        reac_array = ndi_dict['sig_reac'][current_reac_key][1]
                        reac_array.extend(values)
                elif 'full' in current_key:  # account for pn order sections
                    if line[1].isspace():
                        pn_order = int(values[0])
                        ndi_dict[current_key][pn_order] = []
                    else:
                        current_order = list(ndi_dict[current_key])[-1]
                        ndi_dict[current_key][current_order].extend(values)
                # these sections have only a single float value
                elif current_key in ['awr', 'at_wgt', 'temp', 'sig_0']:
                    ndi_dict[current_key] = float(values[0])
                # these sections have only a single int value
                elif (current_key in ['up_grps',
                                      'down_grps',
                                      'num_reac',
                                      'num_sec_parts',
                                      ]
                      or 'num_grps' in current_key
                      or 'order' in current_key):
                    ndi_dict[current_key] = int(values[0])
                # if there is only one whitespace to start line, assume array
                elif not line[1].isspace():
                    ndi_dict[current_key].extend(values)
                else:
                    ndi_dict[current_key] = line.strip()
            # line is a section name otherwise
            elif 'end' in line:  # stop scraping once data sections are done
                break
            else: # add a new section name to the dict
                if line.strip() == 'sig_reac':  # sig_reac is itself a dict
                    ndi_dict['sig_reac'] = defaultdict(lambda: [0,0])
                elif 'full' in line:
                    ndi_dict[line.strip()] = {}
                else:
                    ndi_dict[line.strip()] = []

    # use numpy arrays for arrays
    for key in ndi_dict:
        if 'rprod' in key:  # leave rprod sections as lists of lists
            continue
        elif 'sig_reac' == key:
            for reac_key in ndi_dict['sig_reac']:
                reac_array = ndi_dict[key][reac_key][1]
                reac_array = [float(value) for value in reac_array]
                ndi_dict[key][reac_key][1] = np.array(reac_array)
            continue
        elif 'full' in key:
            for pn_order in ndi_dict[key]:
                pn_array = ndi_dict[key][pn_order]
                pn_array = [float(value) for value in pn_array]
                ndi_dict[key][pn_order] = np.array(pn_array)
        elif isinstance(ndi_dict[key], list):
            ndi_dict[key] = [float(value) for value in ndi_dict[key]]
            ndi_dict[key] = np.array(ndi_dict[key])

    return ndi_dict


def compute_element(ndi_dicts, abundances):
    # TODO: check if ndi_dicts are compatible (same groups, etc)
    # using a defaultdict with 0 takes advantage of numpy broadcasting when
    # adding weighted arrays to element_dict
    element_dict = defaultdict(lambda: 0)

    # here we effectively make an ordered set such that we get all possible
    # section names in order.
    # TODO: currently, if a later dict contains an unseen section name, it will
    # unproperly appear at the end. Fix this?
    keys = dict.fromkeys([key for ndi_dict in ndi_dicts for key in ndi_dict])
    for key in keys:
        if key == 'sig_reac':  # deal with nested reactions dict
            element_dict[key] = defaultdict(lambda: [0, 0])
            reactions = sorted({reaction for ndi_dict in ndi_dicts
                                for reaction in ndi_dict[key]})
            element_dict['num_reac'] = len(reactions)
            # print(reactions)
            for reaction in reactions:
                # print(reaction)
                for index, ndi_dict in enumerate(ndi_dicts):
                    q_val = abundances[index] * ndi_dict[key][reaction][0]
                    element_dict[key][reaction][0] += q_val
                    reac_array = abundances[index] * ndi_dict[key][reaction][1]
                    # print(element_dict[key][reaction][1])
                    # print(reac_array)
                    element_dict[key][reaction][1] += reac_array
            break
        if key == 'pn_full':
            element_dict[key] = defaultdict(lambda: 0)
            for order in range(element_dict['pn_order']):
                for index, ndi_dict in enumerate(ndi_dicts):
                    pn_array = abundances[index] * ndi_dict[key][order]
                    element_dict[key][order] += pn_array
        if key in ['temp',
                   'sig_0',
                   'num_grps',
                   'up_grps',
                   'down_grps',
                   'e_bounds',
                   'wgts',
                   'vel',
                   'pn_order',
                   'num_reac',
                   ]:
            element_dict[key] = ndi_dicts[0][key]
            continue

        for index, ndi_dict in enumerate(ndi_dicts):
            if key_value_exists_and_is_numeric(key, ndi_dict):
                # add weighted component from isotope to overall element
                element_dict[key] += abundances[index] * ndi_dict[key]

    return element_dict


def key_value_exists_and_is_numeric(key, ndi_dict):
    exists_and_is_numeric = False
    if key in ndi_dict:
        value = ndi_dict[key]
        if isinstance(value, float) or isinstance(value, np.ndarray):
            exists_and_is_numeric = True

    return exists_and_is_numeric


def write_ndi_file(ndi_dict, filepath):
    array_wrapper = textwrap.TextWrapper(initial_indent=' ',
                                         subsequent_indent='\n ',
                                         width=132,
                                         )
    with filepath.open('w') as write_file:
        for key in ndi_dict:
            if 'rprod' in key:
                write_file.write(f'{key.rjust(24)}\n')
                for line in ndi_dict[key]:
                    write_line = ""
                    if len(line) == 1:
                        write_line += str(line[0]).rjust(25)
                    if len(line) == 2:
                        write_line += str(line[0]).rjust(25)
                        write_line += str(line[1]).rjust(25)
                    write_line += '\n'
                    write_file.write(write_line)
            elif key == 'sig_reac':
                write_file.write(f'{key}\n')
                for reaction in ndi_dict[key]:
                    write_line = ""
                    write_line += str(reaction).rjust(24)
                    q_val = ndi_dict[key][reaction][0]
                    write_line += str(q_val).rjust(25)
                    write_line += '\n'
                    write_file.write(write_line)

                    reaction_array = ndi_dict[key][reaction][1]
                    reaction_array = [str(num) for num in reaction_array]
                    array_string = ' '.join(reaction_array)
                    write_file.writelines(array_wrapper.wrap(array_string))
                    write_file.write('\n')
            elif 'full' in key:
                write_file.write(f'{key}\n')
                for order in ndi_dict[key]:
                    write_line = str(order).rjust(24)
                    write_line += '\n'
                    write_file.write(write_line)

                    order_array = ndi_dict[key][order]
                    order_array = [str(num) for num in order_array]
                    array_string = ' '.join(order_array)
                    write_file.writelines(array_wrapper.wrap(array_string))
                    write_file.write('\n')
            elif isinstance(ndi_dict[key], np.ndarray):
                write_file.write(f'{key}\n')
                write_array = ndi_dict[key]
                write_array = [str(num) for num in write_array]
                array_string = ' '.join(write_array)
                write_file.writelines(array_wrapper.wrap(array_string))
                if array_string:
                    write_file.write('\n')
            else:
                write_file.write(f'{key}\n')
                write_line = str(ndi_dict[key]).rjust(24)
                write_line += '\n'
                write_file.write(write_line)
        write_file.write('end')

