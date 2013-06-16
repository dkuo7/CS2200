import sys
f = open(sys.argv[1], 'r')
n = open(sys.argv[2], 'w')
for line in f.readlines():
	if line[:3] == 'Bin':
		binary = ''.join(line[4:].split())
		if binary != '':
			if len(binary) != 32:
				raise Exception("Binary value not 32 bits: " + binary)
			hx = hex(int(binary,2))[2:]
		else:
			hx = ''
	if line[:3] == 'Hex':
		line = line[:4] + ' ' + (8 - len(hx)) * '0' + hx + '\n'
	n.write(line)
			
#binary = ''.join(sys.argv[1].split())
#print(hex(int(binary,2)))
