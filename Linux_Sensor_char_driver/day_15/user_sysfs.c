#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buf[32];
    int fd = open("/sys/class/sensor_class/sensor_char/value",O_RDONLY);
    if(fd<0){ perror("open"); return 1; }

    while(1)
    {
        lseek(fd,0,SEEK_SET);
        int n = read(fd,buf,sizeof(buf)-1);
        if(n>0)
        {
            buf[n]='\0';
            printf("Sensor value = %s",buf);
        }
        sleep(1);
    }

    close(fd);
    return 0;
}
