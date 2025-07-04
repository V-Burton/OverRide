#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int store_number(int* storage);
int read_number(int* storage);
unsigned int get_unum(void);
void clear_stdin(void);

int main(int argc, char** argv, char** envp) {
    int storage[100];         
    int command_result = 0;    
    char command[20]; 
    
    memset(storage, 0, sizeof(storage));
    
    for (char** arg = argv; *arg != NULL; arg++) {
        int len = strlen(*arg);
        memset(*arg, 0, len);
    }
    
    for (char** env = envp; *env != NULL; env++) {
        int len = strlen(*env);
        memset(*env, 0, len);
    }
    
    puts("----------------------------------------------------\n"
         "  Welcome to wil's crappy number storage service!   \n"
         "----------------------------------------------------\n"
         " Commands:                                          \n"
         "    store - store a number into the data storage    \n"
         "    read  - read a number from the data storage     \n"
         "    quit  - exit the program                        \n"
         "----------------------------------------------------\n"
         "   wil has reserved some storage :>                 \n"
         "----------------------------------------------------\n");
    
    while (1) {
        printf("Input command: ");
        command_result = 1;
        
        fgets(command, sizeof(command), stdin);
        
        int len = strlen(command);
        if (len > 0 && command[len-1] == '\n') {
            command[len-1] = '\0';
        }
        
        if (strncmp(command, "store", 5) == 0) {
            command_result = store_number(storage);
        }
        else if (strncmp(command, "read", 4) == 0) {
            command_result = read_number(storage);
        }
        else if (strncmp(command, "quit", 4) == 0) {
            return 0;
        }
        
        if (command_result == 0) {
            printf(" Completed %s command successfully\n", command);
        } else {
            printf(" Failed to do %s command\n", command);
        }
        
        memset(command, 0, sizeof(command));
    }
    
    return 0;
}

ad_number(int* storage) {
    unsigned int index;
    
    printf(" Index: ");
    index = get_unum();
    
    printf(" Number at data[%u] is %u\n", index, storage[index]);
    
    return 0;
}


int store_number(int* storage) {
    unsigned int number;
    unsigned int index;
    
    printf(" Number: ");
    number = get_unum();
    
    printf(" Index: ");
    index = get_unum();
    
 
    if ((index % 3 == 0) || ((number >> 24) == 0xb7)) {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
    
    storage[index] = number;
    return 0;
}


unsigned int get_unum(void) {
    unsigned int number = 0;
    
    fflush(stdout);
    scanf("%u", &number);
    clear_stdin();
    
    return number;
}
