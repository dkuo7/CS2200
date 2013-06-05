file = raw_input('file: ')
print file
f = open(file, 'rU')
g = open('hex.txt', 'w')
g.write('v2.0 raw' + '\n')
bin = []
for line in f:
	print line
	print 'middle'
	hexL = hex(int(line,2))[2:]
	formatted = (8-len(hexL)) * '0' + hexL
	g.write(formatted+'\n')
