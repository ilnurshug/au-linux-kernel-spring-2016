#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>

#include "../vsd_driver/vsd_ioctl.h"

void help() 
{
    printf("vsd_userspace size_get\n");    
    printf("vsd_userspace size_set SIZE_IN_BYTES\n");
}

int get_vsd_descriptor() 
{
    return open("/dev/vsd", O_RDONLY);
}

char is_number(const char* str) 
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        if (!isdigit(str[i])) return 0;
        
    return 1;
}

int size_get()
{
    vsd_ioctl_get_size_arg_t arg;
    int fd = get_vsd_descriptor();
    
    if (fd != -1 && ioctl(fd, VSD_IOCTL_GET_SIZE, &arg) == 0) 
    {
        printf("VSD size: %d\n", arg.size);
        return EXIT_SUCCESS;
    }
    
    printf("Error size_get\n");
    return EXIT_FAILURE;
}

int size_set(int size)
{
    if (size < 0) 
    {
        printf("Error size_set\n");
        return EXIT_FAILURE;   
    }
    
    vsd_ioctl_get_size_arg_t arg;
    arg.size = size;
    int fd = get_vsd_descriptor();
    
    if (fd != -1 && ioctl(fd, VSD_IOCTL_SET_SIZE, &arg) == 0) 
    {
        printf("VSD new size: %d\n", arg.size);
        return EXIT_SUCCESS;
    }
    
    printf("Error size_set\n");
    return EXIT_FAILURE;
}

int main(int argc, char **argv) 
{
    if (argc == 2 && strcmp("size_get", argv[1]) == 0) 
    {
        return size_get();
    }
    if (argc == 3 && strcmp("size_set", argv[1]) == 0 && is_number(argv[2]))
    {
        return size_set(atoi(argv[2]));
    }

    help();
    return EXIT_FAILURE;
}