"""
import argparse
parser = argparse.ArgumentParser(description="Find the best.")
parser.add_argument("C",type=int)
parser.add_argument("B",type=int)
parser.add_argument("S",type=int)
parser.add_argument("WP",choices=["WBWA","WTWNA"])
args = parser.parse_args()
C = args.C
B = args.B
S = args.S
WP = args.WP
"""
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
    cmd = "./vm-simulator -c %d -b %d -s %d%s" % (C,B,S,P)
    return cmd

for WP in ["WBWA","WTWNA"]:
    for C in range(12,16):
        for sm in range(0,C+1):
            for B in range(0,sm +1):
                S = sm - B
                total = calc_total(C,B,S,WP) 
                if total <= 48.0:
                    print("C:%d B:%d S:%d WP: %s" % (C,B,S,WP))
                    print(make_cmd(C,B,S,WP))
