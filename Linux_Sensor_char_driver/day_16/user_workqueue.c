#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    char buf[32]; int n;
    int fd=open("/dev/sensor_char",O_RDONLY);
    if(fd<0){ perror("open"); return 1; }

    while(1)
    {
        printf("User: waiting for sensor data...\n");
        n = read(fd,buf,sizeof(buf)-1);
        if(n>0)
        {
            buf[n]='\0';
            printf("User: received sensor value = %s",buf);
        }
    }

    close(fd);
    return 0;
}
