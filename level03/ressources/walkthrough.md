# Level03 - OverRide Walkthrough

## 🎯 Objectif

Trouver le bon **mot de passe numérique** pour obtenir un shell (`/bin/sh`) via une logique interne au binaire.

---

## 🔍 Analyse du programme

### `main`

```c
printf("Password:");
scanf("%d", &input);
test(input);                 
```

### `test`

```c
void test(int input) {
    int secret = 322424845;
    int diff = secret - input;

    if (diff <= 21) {
        decrypt(diff);
    } else {
        decrypt(rand());
    }
}
```

### `decrypt`

```c
char buffer[] = "Q}|u`sfg~sf{}|a3";

for (int i = 0; i < strlen(buffer); i++) {
    buffer[i] ^= diff;
}

if (strcmp(buffer, "Congratulations!") == 0) {
    system("/bin/sh");
}
```

---

## 🧠 Logique de résolution

On veut que :

```
XOR("Q}|u`sfg~sf{}|a3", diff) == "Congratulations!"
```

On teste donc les 22 valeurs possibles pour `diff` (`0` à `21`), on applique un XOR sur chaque caractère et on compare.

---

## ✅ Script de bruteforce

```c
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[] = "Q}|u`sfg~sf{}|a3";
    char result[20];

    for (int i = 0; i <= 21; i++) {
        strcpy(result, buffer);
        for (int j = 0; j < strlen(result); j++) {
            result[j] ^= i;
        }
        if (strcmp(result, "Congratulations!") == 0) {
            printf("Found! The password is: %d\n", 322424845 - i);
            return 0;
        }
    }

    printf("No match found.\n");
    return 1;
}
```

---

## 🧪 Compilation & Exécution

```bash
$ gcc prog.c -o prog
$ ./prog
Found! The password is: 322424827
```

---

## 🚀 Exploitation finale

```bash
$ ./level03
***********************************
*		level03		**
***********************************
Password: 322424827
Congratulations!
$ whoami
level04
```


