#!/usr/bin/env python3

# Example: 
#   python ./version_verify.py /tmp/data
#   echo "14 43 32 31 39 35 2e" | python ./version_verify.py -s
#   equals: echo "14 43 32 31 39 35 2e" | xxd -r -p
#   Or convert version string to ipmid format:
#   python ./version_verify.py -r C2195.0.BS.1A06.GN.3

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

def print_hex(ver, length_byte):
    # insert length byte if need
    if length_byte:
        ver = bytes([len(ver)]).decode('utf-8') + ver
    # convert string to hex format, and split each byte by space
    data = " ".join("{:02x}".format(ord(c)) for c in ver)
    # print new line for each 16 bytes to match format as ipmid
    data_bytes = data.split(" ")
    lines = []
    index = 0
    while (index < len(data_bytes)):
        data_16 = data_bytes[index:index+16]
        if (len(data_16) > 0):
            lines.append(" ".join(str(data_byte) for data_byte in data_16))
        index += 16

    return "\n".join(lines)

def main():
    parser = argparse.ArgumentParser(
        description="IPMID return version bytes data file")
    parser.add_argument(
        'file', default='', nargs='?',
        help='the IPMID oem version command return version byte data'
    )

    parser.add_argument(
        '-s', '--stdin', dest='stdin', action="store_true",
        help="read data from stdin instead of file"
    )
    parser.add_argument(
        '-r', '--revert', dest='revert', action="store_true",
        help="convert version string to bytes"
    )
    parser.add_argument(
        '-v', '--verbose', dest='verb', action="store_true",
        help="show verbose messages"
    )

    args = parser.parse_args()

    # dump arguments
    global VERBOSE
    byte_count = True
    if args.verb:
        print("input file     : {}".format(args.file))
        print("process length : {}".format(byte_count))
        VERBOSE = args.verb
    if args.stdin:
        ver = convert_byte_stdin(byte_count)
    elif args.revert:
        if (len(args.file) == 0):
            print("Input string should not be empty!!\n")
            # parser.print_help()
            return
        ver = print_hex(args.file, byte_count)
    else:
        if (not (os.path.isfile(args.file))):
            sys.exit("Can not find input file " + args.file)
        ver = convert_byte_file(args.file, byte_count)

    print(ver)


if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    main()