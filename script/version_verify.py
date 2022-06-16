#!/usr/bin/env python3

# Example: 
#   python ./version_verify.py /tmp/data
#   echo "14 43 32 31 39 35 2e" | python ./version_verify.py -s

import os
import sys
import argparse

def convert_byte(data, length_byte):
    ver = bytearray()
    for line in data:
        ver += bytearray.fromhex(line)
    if (length_byte):
        return ver[1:].decode('utf-8')
    return ver.decode('utf-8')

def convert_byte_stdin(length_byte):
    return convert_byte(sys.stdin, length_byte)

def convert_byte_file(file, length_byte):
    with open(file) as f:
        data = f.readlines()
        return convert_byte(data, length_byte)


def main():
    parser = argparse.ArgumentParser(
        description="IPMID return version bytes data file")
    parser.add_argument(
        'file', default='', nargs='?',
        help='the IPMID oem version command return version byte data'
    )

    parser.add_argument(
        '-s', '--stdin',  dest='stdin', action="store_true",
        help="read data from stdin instead of file"
    )
    parser.add_argument(
        '-v', '--verbose',  dest='verb', action="store_true",
        help="show verbose messages"
    )

    args = parser.parse_args()

    # dump arguments
    global VERBOSE
    if args.verb:
        print("input file     : {}".format(args.file))
        VERBOSE = args.verb
    if args.stdin:
        ver = convert_byte_stdin(True)
    else:
        if (not (os.path.isfile(args.file))):
            sys.exit("Can not find input file " + args.file)
        ver = convert_byte_file(args.file, True)

    print(ver)


if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    main()