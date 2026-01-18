#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    int fd = open("/dev/sensor_char", O_RDONLY);
    if(fd < 0){ perror("open"); return 1; }

    struct pollfd pfd = { .fd = fd, .events = POLLIN };
    char buf[32];

    while(1)
    {
        printf("Waiting for sensor....\n");
        poll(&pfd, 1, -1);
        int n = read(fd, buf, sizeof(buf)-1);
        if(n>0){ buf[n]='\0'; printf("Sensor value: %s", buf); }
    }
    close(fd);
}
