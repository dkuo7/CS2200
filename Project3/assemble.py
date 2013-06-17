import sys,re
f = open(sys.argv[1], 'r')
n = open(sys.argv[2], 'w')
ops = {
	'add':'02',
	'addi':'42',
	'nand':'12',
	'ldadr':'C2',
	'sw':'48',
	'lw':'46',
	'beq':'21',
	'halt':'ff',
	'jalr':'33',
}
regs = {
	'$ra':'f',
	'$sp':'e',
	'$at':'d',
	'$zero':'0'
}
oppat = re.compile(r'\b(' + '|'.join(ops.keys()) + r')\b')
start = False
for line in f.readlines():
	if start:
		line = oppat.sub(lambda x: ops[x.group()], line)
		for key in regs:
			line = line.replace(key, regs[key])
		line = line.replace(',','')
		if not re.match(r'^\s*$', line) and line[:3] != 'POW':
			n.write(line[2:].lstrip())
	if line[:4] == 'main':
		start = True