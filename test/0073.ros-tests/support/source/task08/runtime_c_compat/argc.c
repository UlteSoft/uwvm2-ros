/* Finite WASI argv control; no SDK/ROS resource is created. */
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
    if (argc == 2 && !strcmp(argv[1], "quit")) return 0;
    fprintf(stderr, "observed argc=%d\n", argc);
    return 22;
}
