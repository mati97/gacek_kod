import sys
f = open(sys.argv[1])
tab = []
for line in f:
    line = line.split(',')
    if len(line) == 2:
        tab.append("0x" + line[1].strip())
out = "#include \"main.h\"\r\n// add this in your project: extern uint8_t pga_mem_map[91];\r\nuint8_t pga_mem_map[91] = { " + ", ".join(tab) + "};"
f.close()
f = open("../src/pga_mem_map.c", "w")
f.write(out)
f.close()