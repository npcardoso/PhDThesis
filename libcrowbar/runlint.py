#!/usr/bin/env python
import os
import os.path
from subprocess import *

extensions = ['.cpp','.h']
lint_path = 'configs/cpplint.py'

for dirpath, dirnames, filenames in os.walk('.'):
  for filename in [f for f in filenames if True in map(f.endswith, extensions)]:
    r = call(['python', lint_path, os.path.join(dirpath, filename)])
    if r != 0:
      exit(1)
