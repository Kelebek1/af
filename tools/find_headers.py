#!/usr/bin/env python3

import sys
import re
from pathlib import Path

def get_incldue_syms(path):
    include_syms = {}
    for file in path.iterdir():
        if Path.is_dir(file):
            include_syms |= get_incldue_syms(file)
        else:
            with open(file, "r") as f:
                fd = f.read()
            funcs = re.findall("(^(// )?[a-zA-Z0-9*_]+ .*\(.*\));$", fd, re.MULTILINE)
            for func in funcs:
                func = func[0]
                if "//" in func:
                    func = func.split("// ",1)[1]
                if "typedef" in func:
                    continue
                f = func.split("(", 1)[0].split(" ",1)[1]
                include_syms[f] = str(file)
    return include_syms

sym_addrs = {}
with open("linker_scripts/jp/symbol_addrs_code.txt", "r") as f:
    fd = f.read().splitlines()
    for line in fd:
        if " = 0x" in line:
            name, addr = line.split(" = ",1)
            addr = addr.split(";",1)[0]
            sym_addrs[int(addr, 0)] = name

include_syms = get_incldue_syms(Path("include/"))
with open(sys.argv[1], "r") as f:
    fd = f.read()

pattern = "jal +([^ ].*)$"
syms_used = re.findall(pattern, fd, re.MULTILINE)
needed = set()
not_found = set()
for sym in syms_used:
    if sym in include_syms:
        needed.add(f"#include \"{include_syms[sym]}\"")
    else:
        not_found.add(sym)

for need in sorted(list(needed)):
    print(need)

if len(not_found) > 0:
    print()
    print(f"Header not found, needs to be created for funcs:")
    for need in sorted(list(not_found)):
        print(f"\t{need}")
