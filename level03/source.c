
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void decrypt(int key) {
    char buffer[] = "Q}|u`sfg~sf{}|a3";
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] ^= key;
    }

    if (strcmp(buffer, "Congratulations!") == 0) {
        system("/bin/sh");
    } else {
        puts("Invalid Password");
    }
}

void test(int input) {
    int secret = 322424845;
    int diff = secret - input;

    if (diff <= 21) {
        decrypt(diff);
    } else {
        decrypt(rand());
    }
}

int main(void) {
    int password;
    srand(time(NULL));

    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &password);
    test(password);

    return 0;
}
