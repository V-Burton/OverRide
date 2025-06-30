# Walkthrough — Level00

## 🎯 Objectif

Exploiter le binaire `level00` pour obtenir un shell en tant que `level00`, puis lire le fichier `/home/users/level01/.pass`.

---

## 🧪 Étapes d’analyse avec GDB

### 1. Lancer le binaire avec GDB

```bash
gdb ./level00
```

---

### 2. Désassembler `main` pour identifier le `scanf`

```gdb
(gdb) disassemble main
```

Dans la sortie, on identifie :

```
0x080484de <+74>: call   0x80483d0 <__isoc99_scanf@plt>
0x080484e3 <+79>: mov    eax,DWORD PTR [esp+0x1c]
0x080484e7 <+83>: cmp    eax,0x149c
```

On place un point d'arrêt juste **après** le `scanf` :

```gdb
(gdb) break *0x080484e3
```

---

### 3. Exécuter le programme

```gdb
(gdb) run
```

Quand le prompt demande un mot de passe, entrer par exemple :

```
1234
```

---

### 4. Lire la valeur entrée

Le programme stocke notre entrée à l'adresse `esp + 0x1c`. Pour la lire :

```gdb
(gdb) p *(int*)($esp+0x1c)
```

Résultat :
```
$1 = 1234
```

Puis vérifier la comparaison effectuée :

```gdb
(gdb) si
(gdb) x/i $eip
```

On obtient :

```
0x080484e7 <main+83>: cmp eax,0x149c
```

`0x149c` = **5276** en décimal.

---

## ✅ Exploitation

Quitter GDB et lancer le programme normalement :

```bash
./level00
Password: 5276
Authenticated!
$ whoami
level01
$ cat /home/users/level01/.pass
```

---

## 📌 Résumé

| Élément        | Détail                        |
|----------------|-------------------------------|
| Vulnérabilité  | Comparaison sur valeur entrée |
| Attendu        | 0x149c (5276 en décimal)      |
| Exploit        | Entrer `5276` à l’exécution   |
| Résultat       | Shell + lecture du `.pass`    |