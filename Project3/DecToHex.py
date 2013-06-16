import sys
sign = sys.argv[1][0]
if sign == '-':
	se = '1'
	start = 3
else:
	se = '0'
	start = 2
binary = bin(int(sys.argv[1]))[start:]
print((16 - len(binary)) * se + binary)
