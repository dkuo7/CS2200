file = raw_input('file: ')
print file
f = open(file, 'rU')
g1 = open('1.txt', 'w')
g2 = open('2.txt', 'w')
g3 = open('3.txt', 'w')
g4 = open('4.txt', 'w')
bin = []
for line in f:
	if line[0] == 'v':
		g4.write(line)
		g3.write(line)
		g2.write(line)
		g1.write(line)
	else:
		g4.write(line[0:2] + '\n')
		g3.write(line[2:4] + '\n')
		g2.write(line[4:6] + '\n')
		g1.write(line[6:8] + '\n')
