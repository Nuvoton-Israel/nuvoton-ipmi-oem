#!/usr/bin/env python3

# Example: python ./gpio_gen_header.py -i npcm8xx_pins.hpp -m 5

import os
import sys
import argparse

VERBOSE = False

def is_valid_pin(line):
    data = line.strip()
    # TODO: use re.match instead
    return data.startswith('NPCM')


class gpio_function:
    def __init__(self, name, reg, offset) -> None:
        # ex: iox1, MFSEL1,30
        self.name = name.strip()
        self.reg = reg.strip()
        self.offset = int(offset.strip())

    def __str__(self) -> str:
        return "[{}, {}, {}]".format(self.name, self.reg, self.offset)

class gpio:
    def __init__(self, line, max_fn) -> None:
        self.refValue = 0
        self.functions = []
        _data = line.split(",")
        self.line = line # just keep for debug
        self.pin = int(_data[0].strip())
        index = 1
        fn = 1
        while fn < max_fn and _data[index].strip().upper() != "NONE":
            gpio_fn = gpio_function(_data[index], _data[index+1], _data[index+2])
            self.functions.append(gpio_fn)
            # if function named "gpio", "gpi", or "gpo", we can directly check
            # the MFSEL bit is set
            if _data[index].strip().startswith("gpi") or \
               _data[index].strip().startswith("gpo"):
                if VERBOSE:
                    print("GPIO fn pin: {}, fn: {}".format(self.pin, fn))
                self.functions = [gpio_fn]
                self.refValue = 1
                break

            fn += 1
            index += 3


    def __str__(self) -> str:
        data = "pin: {},".format(self.pin)
        fns = []
        for fn in self.functions:
            fns.append(str(fn))
        data += ",".join(fns)
        return data

    def dump(self):
        data = "// gpio {}".format(self.pin) + "\n"
        if (len(self.functions) < 1):
            return data + "{DEF_GPIO, 0, 0, 0, NULL}"
        data += "{" + "{}, 1, {}, {}, ".format(self.refValue,
            self.functions[0].reg, self.functions[0].offset)
        if (len(self.functions) > 1):
            data += "&gpio_{}_1 ".format(self.pin) + "}"
        else:
            data += "NULL}"
        return data

    def fn_dump(self, start=1):
        if (len(self.functions) <= 1):
            return ""
        fns = []
        fns_len = len(self.functions)
        for i in range(start, fns_len):
            name = "struct gpio_lookup gpio_{}_{} =".format(self.pin, i)
            fn = self.functions[i]
            data = "0, 1, {}, {}, ".format(fn.reg, fn.offset)
            next = "NULL"
            if (i < fns_len - 1):
                next = "&gpio_{}_{}".format(self.pin, i+1)
            full_data = name + "\n{" + data + next + "};\n"
            fns.append(full_data)
        fns.reverse() # referenced data must define first
        return "\n".join(fns)

    # dump DEF_NONE type GPIO utility
    def NA_dump(pin):
        data = "// gpio {}".format(pin) + "\n"
        return data + "{DEF_NONE, 0, 0, 0, NULL}"


class gpio_table:
    GPIO_START = 0
    GPIO_END = 255
    def __init__(self) -> None:
        self.gpios = []
        self.fn_set = set()  # for debug dump

    # create gpio data and append to table
    def parse_line(self, line, max_fn):
        # get pin number
        _index = line.find('(') + 1
        line = line[_index:-2]
        pin = gpio(line, max_fn)
        self.gpios.append(pin)
        for fn in pin.functions:
            self.fn_set.add(fn.name)

    def _sort(self):
        self.gpios = sorted(self.gpios, key=lambda gpio: gpio.pin)

    # print gpio data for main table
    def dump_main_table(self):
        self._sort()
        table = []
        na_pins = []
        index = 0 # index of current gpio, we need dump all gpio with i
        for i in range(gpio_table.GPIO_START, gpio_table.GPIO_END + 1):
            if index < len(self.gpios) and self.gpios[index].pin == i:
                table.append(self.gpios[index].dump())
                index += 1
            else:
                table.append(gpio.NA_dump(i))
                na_pins.append(i)
        if VERBOSE:
            print("Not defined GPIOs: {}".format(", ".join(str(x) for x in na_pins)))
        return "struct gpio_lookup gpioLUT[GPIO_NUM] =\n{" + \
            ",\n".join(table) + "};"

    def dump_ref_table(self):
        table = []
        for pin in self.gpios:
            data = pin.fn_dump()
            if len(data) > 0:
                table.append(data)
        return "\n".join(table)


def parse_gpio(fin, max_fn):
    gpios = gpio_table()
    line = fin.readline()
    while line:
        if is_valid_pin(line):
            gpios.parse_line(line, max_fn)
        line = fin.readline()
    if VERBOSE:
        print("All functions: ")
        print(sorted(gpios.fn_set))
        print("")
    return gpios

def generate_header(input, output, max_fn):
    key = '<% data %>'
    with open(input) as fin:
        temp_fd = open("gpio_oem.tmp.hpp")
        template = temp_fd.readlines()
        temp_fd.close()
        gpios = parse_gpio(fin, max_fn)
        with open(output, 'w') as fout:
            for line in template:
                if line.strip() == key:
                    fout.write(gpios.dump_ref_table())
                    fout.write("\n\n")
                    fout.write(gpios.dump_main_table())
                else:
                    fout.write(line)

def main():
    parser = argparse.ArgumentParser(
        description="IPMI GPIO map code generator")
    parser.add_argument(
        '-i', '--input-file', dest='gpio_file', default="",
        help='input pin definition from pinctrl driver'
    )
    parser.add_argument(
        '-m', '--max-function', dest='max_fn', default=5,
        type=int, help='maximun gpio multiple functions'
    )
    parser.add_argument(
        '-o', '--output-file',  dest='output',
        default="gpio_oem_npcm.hpp", help="output file name"
    )
    parser.add_argument(
        '-v', '--verbose',  dest='verb', action="store_true",
        help="show verbose messages"
    )

    args = parser.parse_args()
    if (not (os.path.isfile(args.gpio_file))):
        sys.exit("Can not find input gpio file " + args.gpio_file)

    # dump arguments
    global VERBOSE
    if args.verb:
        print("input file     : {}".format(args.gpio_file))
        print("max functions  : {}".format(args.max_fn))
        print("output file    : {}".format(args.output))
        VERBOSE = args.verb

    generate_header(args.gpio_file, args.output, args.max_fn)



if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    main()