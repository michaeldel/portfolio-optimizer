#!/bin/bash
matlab -nodisplay -nojvm -nosplash -nodesktop -r \
      "try, run('$1'), catch, exit(1), end, exit(0);"