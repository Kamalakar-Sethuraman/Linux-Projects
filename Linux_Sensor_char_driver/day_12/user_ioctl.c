#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_SET_VALUE _IOW('s', 1, int)
#define IOCTL_GET_VALUE _IOR('s', 2, int)

int main()
{
    int fd = open("/dev/sensor_char", O_RDWR);
    if(fd < 0){ perror("open"); return 1; }

    int value = 42;
    ioctl(fd, IOCTL_SET_VALUE, &value);
    printf("Value set to %d via ioctl\n", value);

    int read_val = 0;
    ioctl(fd, IOCTL_GET_VALUE, &read_val);
    printf("Value read via ioctl: %d\n", read_val);

    close(fd);
    return 0;
}
