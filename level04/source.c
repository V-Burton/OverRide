#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <signal.h>

int main(void) {
    pid_t pid;
    int status;
    long syscall;
    char buffer[128] = {0};

    pid = fork();

    if (pid == 0) {
        prctl(PR_SET_DUMPABLE, 1);        // Autorise le dump mémoire (anti-debug)
        ptrace(PTRACE_TRACEME, 0, 0, 0);  // Signale que le parent peut tracer ce process, impossible de mettre du shellcode avec execve !
        
        puts("Give me some shellcode, k");
        gets(buffer);                      // ⚠️ Buffer overflow ici !
    } else {

        do {
            wait(&status); 

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                puts("child is exiting...");
                return 0;
            }

            syscall = ptrace(PTRACE_PEEKUSER, pid, 4 * 11, 0); // 11e registre = EAX = numéro syscall
        } while (syscall != 11);  // 11 = syscall execve

        puts("no exec() for you");  // Si execve détecté → tue le process
        kill(pid, 9);
    }

    return 0;
}