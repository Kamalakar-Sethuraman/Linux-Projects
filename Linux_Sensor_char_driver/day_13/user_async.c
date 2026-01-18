#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

volatile sig_atomic_t data_available = 0;
int fd;

void sigio_handler(int signo){ data_available=1; }

int main()
{
    char buf[32]; int n;
    fd=open("/dev/sensor_char",O_RDONLY|O_NONBLOCK);
    if(fd<0){ perror("open"); return 1; }

    signal(SIGIO,sigio_handler);
    fcntl(fd,F_SETOWN,getpid());
    int flags=fcntl(fd,F_GETFL);
    fcntl(fd,F_SETFL,flags|O_ASYNC|O_NONBLOCK);

    while(1)
    {
        pause();
        if(data_available)
        {
            data_available=0;
            while((n=read(fd,buf,sizeof(buf)-1))>0)
            {
                buf[n]='\0';
                write(STDOUT_FILENO,buf,n);
            }
        }
    }
}
