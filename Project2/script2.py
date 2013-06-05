file = raw_input('file: ')
print file
f = open(file, 'rU')
g = open('hexagain.txt', 'w')
bin = []
for line in f:
	print line
	print 'middle'
	newline = (9 - len(line)) * '0' + line
	g.write(newline)
