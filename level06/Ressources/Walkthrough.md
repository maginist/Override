# LEVEL 06

On trouve un binaire level06 à la racine du répo, contenant une fonction **main** et **auth**.

En traduisant le code (**source.c**), on s'aperçoit que l'on doit insérer un **login** dans un premier **fgets** et insérer un **serial** dans un second **fgets** avant d'appeler la fonction auth avec ces deux paramètres en arguments. Si la fonction auth retourne 0, le programme appele **system("/bin/sh")**. Nous allons donc nous intéresser à la fonction auth.

La première condition indique que notre login doit avoir une longueure supérieur à 5. `cmpl   $0x5,-0xc(%ebp)`.
Dans cette condition, nous pouvons voir un appel à la fonction *ptrace*, qui sert, dans ce cas, de sécurité pour le programme. La suite du code dans la condition calcule un hash de notre login et l'assigne dans une variable. Il y a une condition dans le calcul du hash, celui-ci requière des charactères ascii suppérieur à 30 `cmp    $0x1f,%al`.

À la fin de la fonction, une autre condition compare notre variable **hash** avec notre **serial** pris en argument. Si cette comparaison est validée, auth retourne 0, sinon il retourne 1.

En analysant le code ASM du hash, on peut réaliser un script python qui nous permet d'obtenir un hash pour un login spécifique :

```python
import sys
str = sys.argv[1]
print("ARG: ",str)
tmp = (ord(str[3]) ^ 4919) + 6221293
for c in str:
	if ord(c) <= 31:
		exit
	A =  ord(c) ^ int(tmp)
	B = A * 0x88233b2b >> 32
	C = A - B
	D = C >> 1
	E = D + B 
	F = E >> 10
	G = F * 1337
	H = A - G
	tmp += H
print("hash: ", tmp)
```

Par exemple, en insérant "victory" pour le **login**, on obtien notre **serial** :

```python
python .\level06\Ressources\hash_breaker.py victory
ARG:  victory
hash:  6233784
```

Par la suite, il nous suffit de lancer le programme avec nos deux arguments :

```bash
level06@OverRide:~$ > ./level06
***********************************
*               level06           * 
***********************************
-> Enter Login: > victory
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: > 6233784
Authenticated!
$ > cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

#### Success !
