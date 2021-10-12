#!/bin/sh

PASS='Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level03@$OVERRIDE -p 4242 << EOF
SHELL=/bin/sh
(python -c "print '322424827'"; echo 'cat /home/users/level04/.pass')| ./level03
EOF