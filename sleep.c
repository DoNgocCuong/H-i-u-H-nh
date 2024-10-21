#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // If the user forgets to pass an argument, print an error message to stderr.
        fprintf(2, "Usage: sleep <ticks>\n");
        exit(1);
    }

    // Convert the argument to an integer.
    int ticks = atoi(argv[1]);
    if (ticks < 0) {
        fprintf(2, "Error: sleep time must be a positive integer.\n");
        exit(1);
    }

    // Call the sleep system call.
    sleep(ticks);

    // Exit the program.
    exit(0);
}