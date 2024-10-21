#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char *cmd[MAXARG];
    int n, i;

    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    for (i = 1; i < argc; i++) {
        cmd[i - 1] = argv[i];
    }

    while ((n = read(0, buf, sizeof(buf) - 1)) > 0) {
        if (n == 0) {
            break;
        }
        buf[n] = 0;

        char *p = buf;
        char *end = buf + n;
        while (p < end) {
            char *q = p;
            while (q < end && *q != '\n') {
                q++;
            }
            *q = 0;

            cmd[argc - 1] = p;
            cmd[argc] = 0;

            if (fork() == 0) {
                exec(cmd[0], cmd);
                fprintf(2, "exec %s failed\n", cmd[0]);
                exit(1);
            } else {
                wait(0);
            }

            p = q + 1;
        }
    }

    exit(0);
}
