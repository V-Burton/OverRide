#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
0x1337 = 4919
0x5eeded = 6221293
0x539 = 1337
*/

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