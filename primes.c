#include "kernel/types.h"
#include "user/user.h"

// Indicate that sieve doesn't return
void sieve(int left_pipe[2]) __attribute__((noreturn));

void sieve(int left_pipe[2]) {
    close(left_pipe[1]); // Close the write end of the left pipe

    int prime;
    if (read(left_pipe[0], &prime, sizeof(prime)) == 0) {
        // No more data to read, exit
        close(left_pipe[0]);
        exit(0);
    }

    // Print the prime number
    printf("prime %d\n", prime);

    // Create a new pipe for the next process
    int right_pipe[2];
    if (pipe(right_pipe) < 0) {
        fprintf(2, "pipe error\n");
        exit(1);
    }

    // Fork a new process for the next prime filter
    if (fork() == 0) {
        // Child process
        close(left_pipe[0]); // Close the read end of the left pipe
        sieve(right_pipe);   // Continue the sieve with the right pipe
    } else {
        // Parent process
        close(right_pipe[0]); // Close the read end of the right pipe

        int num;
        // Read numbers from the left pipe and filter them
        while (read(left_pipe[0], &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(right_pipe[1], &num, sizeof(num));
            }
        }

        // Close the pipes after done
        close(left_pipe[0]);
        close(right_pipe[1]);
        // Wait for the child process to finish
        wait(0);
        exit(0);
    }
}

int main() {
    int initial_pipe[2];

    // Create the first pipe
    if (pipe(initial_pipe) < 0) {
        fprintf(2, "pipe error\n");
        exit(1);
    }

    // Fork the first sieve process
    if (fork() == 0) {
        // Child process
        sieve(initial_pipe);
    } else {
        // Parent process: generate numbers 2 to 280
        close(initial_pipe[0]); // Close the read end of the pipe

        for (int i = 2; i <= 280; i++) {
            write(initial_pipe[1], &i, sizeof(i));
        }

        // Close the write end of the pipe
        close(initial_pipe[1]);

        // Wait for the first sieve process to finish
        wait(0);
        exit(0);
    }
}
