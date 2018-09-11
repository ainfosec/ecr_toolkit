#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include "mod_ioctl.h"

long strtoul_handler(char *val, int base) {
    long in_arg;
    char *endptr;

    in_arg = strtoul(val, &endptr, base);

    // Check for various possible errors
    if ( (errno == ERANGE && (in_arg == LONG_MAX || in_arg == LONG_MIN)) || (errno != 0 && in_arg == 0) ) {
        perror("Error encountered in strtoul_handler.");
        exit(EXIT_FAILURE);
    }
    if ( endptr == val ) {
        perror("No digits were found\n");
        exit(EXIT_FAILURE);
    }

    // If this is reached, strtroul() successfully parsed a number

    if ( *endptr != '\0' ) // Not necessarily an error
        printf("Further characters after number: %s\n", endptr);

    return in_arg;
}

int main(int argc, char **argv) {
    int fd, ret;
    timing_ioctl_struct arg_struct;

    if ( argc < 2 ) {
        puts("Usage: ./prog <option (0: nop, 1: run timing, 2: kmalloc)> [unused] [unused]");
        exit(EXIT_FAILURE);
    }
    fd = open("/dev/ECR", O_RDONLY);
    if ( fd == -1 ) {
        perror("Could not open the IOCTL /dev/ECR");
        exit(EXIT_FAILURE);
    }

    // This s not being used at the moment. It was partially used in the past to pass in an
    // address for a page walk.
    if ( argc > 2 ) {
        arg_struct.i = strtoul_handler(argv[2], 16);
    } else {
        arg_struct.i = 0;
    }
    if ( argc > 3 ) {
        arg_struct.j = strtoul_handler(argv[3], 16);
    } else {
        arg_struct.j = 0;
    }

    switch ( strtoul_handler(argv[1], 10) ) {
        case 0:
            ret = ioctl(fd, TIMING_IOCTL_NOP, &arg_struct);
            IOCTL_HANDLER;
        case 1:
            ret = ioctl(fd, TIMING_IOCTL_WALL, &arg_struct);
            IOCTL_HANDLER;
        case 2:
            ret = ioctl(fd, TIMING_IOCTL_THREAD, &arg_struct);
            IOCTL_HANDLER;
        case 3:
            ret = ioctl(fd, TIMING_IOCTL_CACHE, &arg_struct);
            IOCTL_HANDLER;
        case 4:
            ret = ioctl(fd, TIMING_IOCTL_MEMORY, &arg_struct);
            IOCTL_HANDLER;
        case 5:
            ret = ioctl(fd, TIMING_IOCTL_WALL_FL, &arg_struct);
            IOCTL_HANDLER;
        case 6:
            ret = ioctl(fd, TIMING_IOCTL_THREAD_FL, &arg_struct);
            IOCTL_HANDLER;
        case 7:
            ret = ioctl(fd, TIMING_IOCTL_KMALLOC, &arg_struct);
            IOCTL_HANDLER;
        default:
            fprintf(stderr, "Invalid option selection\n");
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
