#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../vsd_driver/vsd_ioctl.h"
#include "vsd_device.h"

static int vsd_d;

int vsd_init() 
{
    vsd_d = open("/dev/vsd", O_RDWR);
    if (vsd_d == -1)
    	return EXIT_FAILURE;
    else
    	return EXIT_SUCCESS;
}

int vsd_deinit() 
{
    close(vsd_d);
    return EXIT_SUCCESS;
}

int vsd_get_size(size_t *out_size) 
{
    vsd_ioctl_get_size_arg_t size;
    
    if (ioctl(vsd_d, VSD_IOCTL_GET_SIZE, &size) == -1)
        return EXIT_FAILURE;

    *out_size = size.size;
    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size) 
{
    vsd_ioctl_set_size_arg_t new_size;
    new_size.size = size;
    
    if (ioctl(vsd_d, VSD_IOCTL_SET_SIZE, &new_size) == -1) 
        return EXIT_FAILURE;
    else
    	return EXIT_SUCCESS;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size) 
{
    return (lseek(vsd_d, offset, SEEK_SET) < 0) ? -1 : read(vsd_d, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size) 
{
    return (lseek(vsd_d, offset, SEEK_SET) < 0) ? -1 : write(vsd_d, src, size);
}

void* vsd_mmap(size_t offset) 
{
    size_t l;
    vsd_get_size(&l);
    return mmap(0, l - offset, PROT_READ | PROT_WRITE, MAP_SHARED, vsd_d, offset);
}

int vsd_munmap(void* addr, size_t offset) 
{
    size_t l;
    vsd_get_size(&l);
    return munmap(addr, l - offset);
}
