int main(void) {
    int input;

    printf("***********************************\n");
    printf("*         -Level00 -              *\n");
    printf("***********************************\n");
    printf("Password: ");

    scanf("%d", &input);

    if (input == 5276) {
        printf("Authenticated!\n");
        system("/bin/sh");
    } else {
        printf("Invalid Password!\n");
    }
    return 0;
}