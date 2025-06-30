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
