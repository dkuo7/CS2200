import argparse

parser = argparse.ArgumentParser()

parser.add_argument("file")

args = parser.parse_args()

res = open(args.file,'r')

min = float("inf")
minline = ""

for line in res.readlines():
    num = line.split(":")[-1]
    if float(num) < min:
        min = float(num)
        minline = line

print(min)
print(minline)
res.close()
