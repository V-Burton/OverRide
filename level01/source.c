extern char a_user_name[]; // déclaration de la variable globale contenant l'input

int verify_user_name(void) {
    puts("verifying username....\n");

    // Compare les 7 premiers caractères avec "dat_wil"
    if (strncmp(a_user_name, "dat_wil", 7) == 0) {
        return 0; // OK
    } else {
        return 1; // Mauvais nom d'utilisateur
    }
}

int verify_user_pass(char *input) {
    // Compare les 5 premiers caractères avec "admin"
    if (strncmp(input, "admin", 5) == 0) {
        return 0; // mot de passe correct
    } else {
        return 1; // mot de passe incorrect
    }
}


int main(void) {
    char local_54[64];  // buffer pour le mot de passe
    int valid = 0;      // variable de retour logique (0 = ok, 1 = erreur)
    
    // On nettoie le buffer en mettant tous les octets à zéro (équivalent à memset)
    memset(local_54, 0, sizeof(local_54));

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");

    // Lecture du nom d'utilisateur dans une variable globale `a_user_name`
    fgets(&a_user_name, 0x100, stdin);

    // Vérification du nom d’utilisateur
    valid = verify_user_name();

    if (valid == 0) {
        // Si le username est bon, on demande le mot de passe
        puts("Enter Password: ");
        fgets(local_54, 100, stdin);  // ⚠️ overflow possible ici !

        // Vérification du mot de passe
        valid = verify_user_pass(local_54);

        // bizarre mais instruction toujours vraie :
        if ((valid == 0) || (valid != 0)) {
            puts("nope, incorrect password...\n");
            return 1;
        } else {
            return 0;
        }
    } else {
        puts("nope, incorrect username...\n");
        return 1;
    }
}
