# LEVEL 03

On trouve un binaire level03 à la racine du répo, contenant une fonction **main**, **test** et **decrypt**.

En traduisant le code (**source.c**), on s'appercoit que le main utilise un **scanf** pour récupérer un int sur l'entrée standard.

```gdb
   0x080488b6 <+92>:    lea    0x1c(%esp),%edx  #breakpoint
   0x080488ba <+96>:    mov    %edx,0x4(%esp)
   0x080488be <+100>:   mov    %eax,(%esp)
   0x080488c1 <+103>:   call   0x8048530 <__isoc99_scanf@plt>
   0x080488c6 <+108>:   mov    0x1c(%esp),%eax
   0x080488ca <+112>:   movl   $0x1337d00d,0x4(%esp)
   0x080488d2 <+120>:   mov    %eax,(%esp)
   0x080488d5 <+123>:   call   0x8048747 <test>

(gdb) > x/s $eax
0x8048a85:       "%d"

(gdb) > p 0x1337d00d
$6 = 322424845
```

 Il y a par la suite la fonction **test** qui prend en argument ce dernier et la valeur 322424845. Dans cette fonction, on soustrait le second int au premier, et on compare le résultat avec 21, et si celui ci est inférieur ou égal a 21, l'int sera envoyé dans la fonction **decrypt**.

```gdb
   0x08048755 <+14>:    sub    %eax,%ecx
   0x08048757 <+16>:    mov    %ecx,%eax
   0x08048759 <+18>:    mov    %eax,-0xc(%ebp)
   0x0804875c <+21>:    cmpl   $0x15,-0xc(%ebp)
   0x08048760 <+25>:    ja     0x804884a <test+259>
   0x08048766 <+31>:    mov    -0xc(%ebp),%eax
   0x08048769 <+34>:    shl    $0x2,%eax
   0x0804876c <+37>:    add    $0x80489f0,%eax
   0x08048771 <+42>:    mov    (%eax),%eax
   0x08048773 <+44>:    jmp    *%eax
   0x08048775 <+46>:    mov    -0xc(%ebp),%eax
   0x08048778 <+49>:    mov    %eax,(%esp)
   0x0804877b <+52>:    call   0x8048660 <decrypt>

Breakpoint 2, 0x08048755 in test ()
(gdb) > p $eax
$7 = 4242
(gdb) > p $ecx
$8 = 322424845
(gdb) > p 0x15
$10 = 21
```

La fonction **decrypt** quant à elle, va chercher à obtenir la string "Congratulations!", après avoir appliqué un mask sur le password.

```gdb
   0x080486e8 <+136>:   cmp    -0x24(%ebp),%eax
   0x080486eb <+139>:   jb     0x80486c7 <decrypt+103>
   0x080486ed <+141>:   lea    -0x1d(%ebp),%eax
   0x080486f0 <+144>:   mov    %eax,%edx
   0x080486f2 <+146>:   mov    $0x80489c3,%eax
   0x080486f7 <+151>:   mov    $0x11,%ecx
   0x080486fc <+156>:   mov    %edx,%esi
   0x080486fe <+158>:   mov    %eax,%edi
   0x08048700 <+160>:   repz cmpsb %es:(%edi),%ds:(%esi)

Breakpoint 3, 0x08048700 in decrypt ()
(gdb) > x/s $edi
0x80489c3:       "Congratulations!"
(gdb) > x/s $esi
0xffffd69b:      "fJKBWDQPIDQLJKV\004"
```

On veut donc trouver une donnée adéquate qui nous permettra d'obtenir la string attendue par le programme, qui nous donnera accès au **/bin/sh**.

```gdb
   0x08048715 <+181>:   movl   $0x80489d4,(%esp)
   0x0804871c <+188>:   call   0x80484e0 <system@plt>

(gdb) > x/s 0x80489d4
0x80489d4:       "/bin/sh"
```

On détermine la string qui va modifier le password avec un xor en regardant les variables chargées dans ebp avec n'importe quel outil de conversion :

```gdb
   0x08048673 <+19>:    movl   $0x757c7d51,-0x1d(%ebp)
   0x0804867a <+26>:    movl   $0x67667360,-0x19(%ebp)
   0x08048681 <+33>:    movl   $0x7b66737e,-0x15(%ebp)
   0x08048688 <+40>:    movl   $0x33617c7d,-0x11(%ebp)

   	// int var1 = 0x757c7d51; u|}Q
	// int var2 = 0x67667360; gfs`
	// int var3 = 0x7b66737e; {fs~
	// int var4 = 0x33617c7d; 3a|}
	char str[17] = "Q}|u`sfg~sf{}|a3";
```

On cherche donc le X de Q XOR X = C, donc on peut faire Q XOR C = X.
En faisant cette opération, nous trouvons 18 et nous pouvons donc l'appliquer a toute la chaine de caractère:

> 322424845 - 18 = 322424827

```shell
level03@OverRide:~$ ./level03 322424827
***********************************
*               level03         **
***********************************
Password:322424827
$ cd ../level04 && cat .pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```