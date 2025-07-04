# Level06

Pour ce niveau, il fallait décompiler le programme avec Ghidra et analyser la fonction `auth`.

Le programme demande un login, détecte qu'il s'agit d'un nouveau compte, puis demande un "serial" (mot de passe). Si la fonction `auth` valide le serial, un shell est lancé, sinon le programme se termine.

La fonction `auth` applique plusieurs vérifications :
- Le login doit faire au moins 6 caractères.
- Une protection anti-debug (avec `ptrace`) est présente, mais la logique de calcul du serial est suffisamment explicite pour être comprise sans debugger.
- Le hash utilisé pour vérifier le serial dépend du 3ème caractère du login, puis est modifié dans une boucle en fonction de chaque caractère du login (en s'assurant qu'aucun caractère n'est inférieur à l'espace ASCII).
- À chaque itération, le hash est mis à jour avec une opération de XOR et un modulo 1337.

En reproduisant ce calcul dans un script, on peut générer le serial attendu pour n'importe quel login.

```c
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <username>\n", argv[0]);
        return 1;
    }
    char username[32];
    strncpy(username, argv[1], 32);
    int hash = (username[3] ^ 4919) + 6221293;

    for (int i = 0; i < strlen(username); i++){
        if (username[i] < 32){
            printf("Invalid username\n");
            return 1;
        }
        hash = hash + (username[i] ^ hash) % 1337;
    }
    printf("password found:%d\n", hash);
}
```

## Notes
Ce mécanisme ressemble à un système de génération de licence, souvent utilisé dans l'authentification de logiciels.
