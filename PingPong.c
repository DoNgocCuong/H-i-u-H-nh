#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pipe1[2]; // Pipe for parent-to-child communication
    int pipe2[2]; // Pipe for child-to-parent communication
    char buf[1];  // Buffer to hold the byte

    // Create the pipes
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        fprintf(2, "pipe error\n");
        exit(1);
    }

    // Fork the process
    int pid = fork();
    if (pid < 0) {
             fprintf(2, "fork error\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Close unused ends of the pipes
        close(pipe1[1]);
        close(pipe2[0]);

        // Read from pipe1 (parent to child)
        read(pipe1[0], buf, 1);
        printf("%d: received ping\n", getpid());

        // Write to pipe2 (child to parent)
        write(pipe2[1], buf, 1);

        // Close the pipes
        close(pipe1[0]);
        close(pipe2[1]);

        // Exit the child process
        exit(0);
    } else {
        // Parent process
        // Close unused ends of the pipes
        close(pipe1[0]);
        close(pipe2[1]);

        // Write to pipe1 (parent to child)
        buf[0] = 'x'; // Sending an arbitrary byte
        write(pipe1[1], buf, 1);

        // Read from pipe2 (child to parent)
        read(pipe2[0], buf, 1);
        printf("%d: received pong\n", getpid());

        // Close the pipes
        close(pipe1[1]);
        close(pipe2[0]);

        // Wait for the child process to complete
        wait(0);

        // Exit the parent process
        exit(0);
    }
}
