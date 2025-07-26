import re
import csv
import os
import argparse

parser = argparse.ArgumentParser(
    description="Generate folder path from model name and CPU type"
)
parser.add_argument(
    "--cpu", 
    required=True, 
    help="CPU identifier (e.g. i7, i9, arm64)"
)
parser.add_argument(
    "--modelname", 
    required=True, 
    help="Model name (e.g. llama3)"
)
args = parser.parse_args()
cpu = args.cpu
modelname = args.modelname
foldername = f"{modelname}/{modelname}_{cpu}/"
#print(foldername)  #e.g. llama3/llama3_i7/

filenames = os.listdir(foldername)
for filename in filenames:
    fullname = foldername + filename
    #print(fullname)
    outputfilename = fullname + ".csv"
    #print(outputfilename)
    block_re = re.compile(
        r"===\s*(?P<lib>\S+)\s+ThreadNum=(?P<threads>\d+)\s*===\s*"
        r"Took\s+(?P<took>[\d\.eE+-]+)\s+seconds\s+for\s+(?P<runs>\d+)\s+runs\.\s+"
        r"(?P<warmups>\d+)\s+warmups\s*"
        r"(?P<avg_time>[\d\.eE+-]+)\s+Avg\.\s+\((?P<avg_gflops>[\d\.]+)\s+GFLOPS\)\s*"
        r"(?P<med_time>[\d\.eE+-]+)\s+Med\.\s+\((?P<med_gflops>[\d\.]+)\s+GFLOPS\)\s*"
        r"(?P<min_time>[\d\.eE+-]+)\s+Min\.\s+\((?P<max_gflops>[\d\.]+)\s+GFLOPS\)\s*"
        r"(?P<dev>[\d\.eE+-]+)\s+Dev",
        re.MULTILINE
    )

    with open(fullname, 'r') as f:
        data = f.read()

    rows = []
    for m in block_re.finditer(data):
        med_gflops = int(round(float(m.group('med_gflops'))))
        avg_gflops = int(round(float(m.group('avg_gflops'))))
        max_gflops = int(round(float(m.group('max_gflops'))))
        rows.append({
            'CPU':      cpu,
            'ThreadNum':  m.group('threads'),
            'Med_GFLOPS':   med_gflops,
            'Avg_GFLOPS':   avg_gflops,
            'Max_GFLOPS':   max_gflops,

        })
    
    # write out CSV
    with open(outputfilename, 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)

    print(f"Wrote {len(rows)} rows to {outputfilename}")