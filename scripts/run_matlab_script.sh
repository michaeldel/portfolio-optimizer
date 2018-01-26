#!/bin/sh
matlab -nodisplay -nojvm -nosplash -nodesktop -wait -r \
      "try, run('$1'), catch, exit(1), end, exit(0);"