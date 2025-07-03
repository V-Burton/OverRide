

struct s_message {
    char message[140];
    char username[40];
    int len_message;
};


void secret_backdoor(void) {
    char buffer[128];
    fgets(buffer, sizeof(buffer), stdin);
    system(buffer);
}


void set_username(struct s_message *msg) {
    char input[128];
    int i;

    puts(">: Enter your username");
    printf(">>: ");
    fgets(input, sizeof(input), stdin);

    for (i = 0; i <= 40 && input[i] != '\0'; i++) {
        msg->username[i] = input[i];  // ⚠️ Overflow ici à i == 40
    }

    printf(">: Welcome, %s", msg->username);
}


void set_msg(struct s_message *msg) {
    char input[1024];

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(input, sizeof(input), stdin);

    strncpy(msg->message, input, msg->len_message);
}



void handle_msg(void) {
    struct s_message msg;

    msg.len_message = 0x8c;  //140
    set_username(&msg);
    set_msg(&msg);

    puts(">: Msg sent!");
}



int main(void) {
    puts("--------------------------------------------");
    puts("|   ~Welcome to l33t-m$n ~    v1337        |");
    puts("--------------------------------------------");

    handle_msg();
    return 0;
}
