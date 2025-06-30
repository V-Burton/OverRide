#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE *fp;
    char password_file[0x29];     // Buffer pour le contenu du fichier .pass (41 caractères max)
    char username[100];           // Input de l'utilisateur
    char password_input[100];     // Mot de passe tapé par l'utilisateur
    size_t read_size;
    int cmp_result;

    // Nettoyage mémoire (sûrement pour éviter des comportements indéfinis)
    memset(username, 0, sizeof(username));
    memset(password_input, 0, sizeof(password_input));
    memset(password_file, 0, sizeof(password_file));

    // Lecture du mot de passe stocké dans le fichier
    fp = fopen("/home/users/level03/.pass", "r");
    if (!fp) {
        fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
        exit(1);
    }

    // Lecture jusqu'à 0x29 (41) octets
    read_size = fread(password_file, 1, 0x29, fp);
    if (read_size != 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
        exit(1);
    }

    // Retire le retour à la ligne final
    password_file[strcspn(password_file, "\n")] = '\0';
    fclose(fp);

    // Affichage classique
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    // Demande du nom d'utilisateur
    printf("--[ Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    // Demande du mot de passe
    printf("--[ Password: ");
    fgets(password_input, sizeof(password_input), stdin);
    password_input[strcspn(password_input, "\n")] = '\0';

    puts("*****************************************");

    // Comparaison entre le vrai mot de passe et celui saisi
    cmp_result = strncmp(password_file, password_input, 0x29);

    if (cmp_result == 0) {
        printf("Greetings, %s!\n", username);
        system("/bin/sh");
        return 0;
    } else {
        // ⚠️ Vulnérabilité ici : printf(username) SANS format string → format string vulnerability
        printf(username);
        puts(" does not have access!");
        exit(1);
    }
}