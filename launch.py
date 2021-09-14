import sys
from os import listdir
from os.path import isfile, join
import time
import os
import sys
import subprocess

def run(file, cmd):
  proc = subprocess.call(
    cmd,
    stderr = subprocess.STDOUT,
    shell = True
  )

targ_lvl = 1
try:
  targ_lvl = int(sys.argv[1]) if len(sys.argv) > 1 else 1
except:
  pass

os.system('./compile.sh')
input("Compiled source! Hit enter to execute.")

base_path = f'inp/level{targ_lvl}'
inp_files = [f for f in listdir(base_path) if isfile(join(base_path, f)) and f.endswith(".in")]
inp_files.sort()

for inp_file in inp_files:
  abs_path = join(base_path, inp_file)
  out_path = os.path.splitext(abs_path)[0] + '.out'
  cmd = f'cat {abs_path} | ./a.out {targ_lvl} > {out_path}'

  start = time.time()
  run(abs_path, cmd)
  end = time.time()
  print(f'C code for {out_path} took {end-start}s')
