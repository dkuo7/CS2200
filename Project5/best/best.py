import argparse
parser = argparse.ArgumentParser(description="Find the best.")
parser.add_argument("file",help="Trace file")
parser.add_argument("start_C",type=int)
parser.add_argument("end_C",type=int)
"""
parser.add_argument("C",type=int)
parser.add_argument("B",type=int)
parser.add_argument("S",type=int)
parser.add_argument("WP",choices=["WBWA","WTWNA"])
C = args.C
B = args.B
S = args.S
WP = args.WP
"""
args = parser.parse_args()
tester = open('tester.sh','w')
def calc_total(C,B,S,WP):
    tag_size = 32 - C + S
    block_size = pow(2,B)*8 + tag_size + 1
    if WP == "WBWA":
        block_size += 1
    total = block_size * pow(2,C-B)
    total += pow(2,S) * S * pow(2,C-B-S)
    return (total / 8.0 / 1024) 

def make_cmd(C,B,S,WP):
    if WP == "WBWA":
        P = " -P"
    else:
        P = ""
    cmd = "echo -n \"%d %d %d %s -- \"  >> results.txt\n" % (C,B,S,WP)
    cmd += "../simulator -c %d -b %d -s %d%s < %s | tail -n 1  >> results.txt\n" % (C,B,S,P,args.file)
    return cmd

for WP in ["WBWA","WTWNA"]:
    for C in range(args.start_C,args.end_C + 1):
        for sm in range(0,C+1):
            for B in range(0,sm +1):
                S = sm - B
                total = calc_total(C,B,S,WP) 
                if total <= 48.0:
                    print("C:%d B:%d S:%d WP: %s" % (C,B,S,WP))
                    tester.write(make_cmd(C,B,S,WP))
 
tester.close()
