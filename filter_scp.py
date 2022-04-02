#!/usr/bin/env python3
# Copyright 2010-2012 Microsoft Corporation
#                     Johns Hopkins University (author: Daniel Povey)

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
# WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
# MERCHANTABLITY OR NON-INFRINGEMENT.
# See the Apache 2 License for the specific language governing permissions and
# limitations under the License.

# This script takes a list of utterance-ids or any file whose first field
# of each line is an utterance-id, and filters an scp
# file (or any file whose "n-th" field is an utterance id), printing
# out only those lines whose "n-th" field is in id_list. The index of
# the "n-th" field is 1, by default, but can be changed by using
# the -f <n> switch

from collections import defaultdict
import sys
import re

# 指定 exclude 将没有出现在id_list中的行输出
# 否则将在id_list中的行输出

# f 默认是1 指in.scp第几列是uttr-id, 需要自己指定

exclude = 0
field = 1
shifted = 1

# 读取输入参数
argv = sys.argv[1:]

while shifted:
    shifted = 0
    if argv[0] == "--exclude":
        exclude = 1
        argv.pop(0)  # shift @ARGV
        shifted = 1
    if argv[0] == "-f":
        field = argv[1]
        argv.pop(0)
        argv.pop(0)
        shifted = 1

if len(argv) < 1 or len(argv) > 2:
    raise ValueError(
        "Usage: filter_scp.pl [--exclude] [-f <field-to-filter-on>] id_list [in.scp] > out.scp \n"
        "Prints only the input lines whose f'th field (default: first) is in 'id_list'.\n"
        "Note: only the first field of each line in id_list matters.  With --exclude, prints\n"
        "only the lines that were *not* in id_list.\n"
        "Caution: previously, the -f option was interpreted as a zero-based field index.\n"
        "If your older scripts (written before Oct 2014) stopped working and you used the\n"
        "-f option, add 1 to the argument.\n"
        "See also: utils/filter_scp.pl .\n")

idlist = argv.pop(0)
try:
    F = open(idlist, 'r', encoding='utf-8')
except FileNotFoundError:
    print("Could not open id-list file {}".format(idlist))
    sys.exit(1)

lines = F.readlines()
seen = defaultdict(int)
for line in lines:
    line = line.strip()
    A = re.split(r"\s+", line)
    if len(A) < 1:
        raise ValueError("Invalid id-list file line {}".format(line))
    seen[A[0]] = 1
F.close()

in_scp = open(argv[0], 'r', encoding='utf-8')
lines_in_scp = in_scp.readlines()
# print()
if field == 1:
    for line in lines_in_scp:
        if re.search(r"\s*(\s+)\s*", line) is None:
            print("Bad line {}, could not get first field.".format(line))
            sys.exit(1)
        uttr = re.findall(r"\s*(\S+)\s*", line)[0]

        if (~exclude and seen[uttr]) or (exclude and ~(seen[uttr])):
            print(uttr)

else:
    for line in lines_in_scp:
        line = line.strip()
        A = re.split(r"\s+", line)
        if len(A) <= 0:
            print("Invalid scp file line {}".format(line))
        if len(A) < field:
            print("Invalid scp file line {}".format(line))
        if (~exclude and seen[A[field - 1]]) or (exclude
                                                 and ~(seen[A[field - 1]])):
            print(A[field - 1])

in_scp.close()
