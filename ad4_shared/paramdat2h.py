#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
    print("Usage: python paramdat2h.py AD4_parameters.dat AD4.1_bound.dat", file=sys.stderr)
    sys.exit(1)

def format_param_file(filename, array_name):
    print(f'const char *{array_name}[MAX_LINES] = {{')
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith('#'):
                print(f'"{line}\\n", ')
    print('};')

format_param_file(sys.argv[1], 'param_string_4_0')
format_param_file(sys.argv[2], 'param_string_4_1')
print('// EOF')
