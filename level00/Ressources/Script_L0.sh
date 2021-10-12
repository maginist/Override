#!/bin/sh

PASS=level00
echo $sshpass
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level00@$OVERRIDE -p 4242 << EOF
(python -c "print '5276'"; echo 'cat /home/users/level01/.pass') | ./level00
./level00
EOF