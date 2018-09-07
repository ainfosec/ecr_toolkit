#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uapi/xen/privcmd.h>
#include "xen_ioctl.h"

int hypercall(uint16_t dom_no, unsigned long vmcs_field, unsigned long vmcs_bit, bool enable, bool rdtsc_alert) {

    static int _dom0_fd;
    int status;

    privcmd_hypercall hypercall = {
        42,                                                    // Hypercall number
        { dom_no, vmcs_field, vmcs_bit, enable, rdtsc_alert },
    };

    if ((_dom0_fd=open("/proc/xen/privcmd", O_RDWR | O_CLOEXEC)) == -1) {
        perror("Failed to open /proc/xen/privcmd\n");
        return -1;
    }
    if (ioctl(_dom0_fd, IOCTL_PRIVCMD_HYPERCALL, &hypercall) == -1)
        printf("ioctl returned -1: %s\n",
              strerror(errno));
    else {
        printf("ioctl returned: %s\n", strerror(errno));
    }
    close(_dom0_fd);
    return 0;
}

int main() {

    build_vmcs();

    FILE* fp;
    char line[250]; // Lines in config file shouldn't be longer than 50 chars, but if they are this number needs to be increased
    int num_lines = 0;

    /* Open file and get number of lines */
    fp = fopen("./config.txt", "r");
    if (fp == NULL) {
        printf("ERROR: Unable to open config.txt\n");
        return 1;
    }
    while ((fgets(line, sizeof line, fp)) != NULL) { num_lines++; }
    rewind(fp); // Reset file pointer

    /* Retrieve Domain Number */
    uint16_t dom_no = 0;
    bool rdtsc_alert = 0;
    char* temp;
    if ((fgets(line, sizeof line, fp)) != NULL) {
        temp = strtok(line, "\n"); // Strip newline character
        temp = strtok(line, " "); // Strip first part of string ("DOMAIN NUMBER")
        temp = strtok(NULL, " "); // Get second part of string (Actual number)
        dom_no = strtol(temp, NULL, 10);
    }
    if (dom_no == 0) {
        printf("ERROR: Couldn't get domain number\n");
        return 1;
    }
    if ((fgets(line, sizeof line, fp)) != NULL) {
        temp = strtok(line, "\n"); // Strip newline character
        temp = strtok(line, " "); // Strip first part of string ("RDTSC ALERT")
        temp = strtok(NULL, " "); // Get second part of string (Actual number)
        rdtsc_alert = strtol(temp, NULL, 10);
    }
    if (*temp != '0' && *temp != '1') {
        printf("ERROR: Couldn't get rdtsc alert value\n");
        return 1;
    }
    fgets(line, sizeof line, fp); // Gets rid of blank line between domain number and flags in config file
    num_lines -= 3; // Since the domain number and blank line are not a vmcs field, we don't include that in our array sizing below

    /* Retrieve flag names and values from config file */
    int flag_values[num_lines];
    char flag_names[num_lines][50];
    int i = 0;
    while ((fgets(line, sizeof line, fp)) != NULL) {
        temp = strtok(line, "\n"); // Strip newline character
        temp = strtok(line, " "); // Get first part of string (flag name)
        strcpy(flag_names[i], temp);
        temp = strtok(NULL, " "); // Get second part of string (enable bit)
        flag_values[i] = strtol(temp, NULL, 10);
        i++;
    }

    fclose(fp);
    fp = NULL;

    /* Perform hypercall for each VMCS execution field in config file */
    for (i = 0; i < num_lines; i++) {
//        printf("VMCS Field: %s\n", flag_names[i]);
//        printf("Enable? %i\n", flag_values[i]);
        int j = 0;
        bool prim_exec = false;
        bool sec_exec = false;
        /* Search for the VMCS field in the primary execution fields */
        for (j = 0; j < 16; j++) {
            if (!(strcmp(flag_names[i], primary_exec[j].name))) {
                prim_exec = true;
                break;
            }
        }
        /* If we didn't find the VMCS field in the primary fields, search the secondary fields */
        if (prim_exec == false) {
            for (j = 0; j < 10; j++) {
                if (!(strcmp(flag_names[i], secondary_exec[j].name))) {
                    sec_exec = true;
                    break;
                }
            }
        }

        /* If found, perform the appropriate hypercall */
        if (prim_exec == true)
            hypercall((uint16_t) dom_no, (unsigned long) CPU_BASED_VM_EXEC_CONTROL, (unsigned long) primary_exec[j].mask, (bool) flag_values[i], rdtsc_alert);
        else if (sec_exec == true)
            hypercall((uint16_t) dom_no, (unsigned long) SECONDARY_VM_EXEC_CONTROL, (unsigned long) secondary_exec[j].mask, (bool) flag_values[i], rdtsc_alert);
        else
            printf("%s not found\n", flag_names[i]);
        printf("\n");
    }

    return 0;
}
