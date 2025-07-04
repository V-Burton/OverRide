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
    int valid = 0;
    
    memset(local_54, 0, sizeof(local_54));

    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");

    fgets(&a_user_name, 0x100, stdin);

    valid = verify_user_name();

    if (valid == 0) {
        puts("Enter Password: ");
        fgets(local_54, 100, stdin);  // ⚠️ overflow possible ici !

        valid = verify_user_pass(local_54);

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
