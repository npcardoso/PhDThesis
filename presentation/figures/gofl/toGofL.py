#!/usr/bin/env python

import sys
import re


def createGogLLatex(l):
    l = l.strip()
    l = l.split('!')[0]
    l = l.split('$')

    pattern = re.compile(r"([0-9]*)([bo])")

    lines = []
    for line in l:
        elems = []
        for count, symbol in pattern.findall(line):
            if count == '':
                count = 1
            else:
                count = int(count)

            if symbol == 'o':
                symbol = '1'
            else:
                symbol = '0'

            elems += count * [symbol]
        lines.append(elems)

    max_size = 0
    for line in lines:
        max_size = max(max_size, len(line))

    ret = "{\n"
    for line in lines:
        line += (max_size - len(line)) * ['0']
        ret += "{" + ','.join(line) + "},\n"
    ret += "}"

    return ret



tmp = ''
for line in sys.stdin:
    tmp += line.strip()
    if line.find('!') >= 0:
        print(createGogLLatex(tmp))
        tmp = ''
