#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_SIZE 4096

int main()
{
    int fd = open("/dev/sensor_char",O_RDONLY);
    if(fd<0){ perror("open"); return 1; }

    char *addr = mmap(NULL,MAP_SIZE,PROT_READ,MAP_SHARED,fd,0);
    if(addr==MAP_FAILED){ perror("mmap"); return 1; }

    while(1)
    {
        printf("%s",addr);
        sleep(2);
    }

    munmap(addr,MAP_SIZE);
    close(fd);
    return 0;
}
