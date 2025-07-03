void log_wrapper(FILE *file, char *prefix, char *message)
{
    char buffer[264];
    
    strcpy(buffer, prefix);
    
    int prefix_len = strlen(prefix);
    snprintf(buffer + prefix_len, sizeof(buffer) - prefix_len, "%s", message);
    
    char *newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }
    
    fprintf(file, "LOG: %s\n", buffer);
}

int main(int argc, char *argv[])
{
    FILE *log_file, *source_file;
    int dest_fd;
    char backup_path[104];
    int c;
    
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }
    
    log_file = fopen("./backups/.log", "w");
    if (!log_file) {
        printf("ERROR: Failed to open ./backups/.log\n");
        exit(1);
    }
    
    log_wrapper(log_file, "Starting back up: ", argv[1]);
    
    source_file = fopen(argv[1], "r");
    if (!source_file) {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }
    
    strcpy(backup_path, "./backups/");
    strncat(backup_path, argv[1], sizeof(backup_path) - strlen(backup_path) - 1);
    
    dest_fd = open(backup_path, O_WRONLY | O_CREAT | O_TRUNC, 0660);
    if (dest_fd < 0) {
        printf("ERROR: Failed to open %s\n", backup_path);
        exit(1);
    }
    
    while ((c = fgetc(source_file)) != EOF) {
        write(dest_fd, &c, 1);
    }
    
    log_wrapper(log_file, "Finished back up ", argv[1]);
    
    fclose(source_file);
    fclose(log_file);
    close(dest_fd);
    
    return 0;
}
