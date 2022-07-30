#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/type.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
    int fd;
    char option;

    fd = open("/dev/quannh41_device", O_RDWR);
    if (fd < 0)
    {
        printf("Cannot open device file\n");
        return 0;
    }

    while(1)
    {
        printf("Enter the Option\n");
        printf("1 - Write\n");
        printf("1 - Open\n");
        printf("1 - Exit\n");

        scanf("%c", &option);

        switch(option)
        {
            case '1':
                printf("Enter the string to write to driver:");
                scanf("%s", write_buf);
                printf("\n");
                printf("Data writing...");
                write(fd, write_buf, strlen(write_buf) + 1);
                printf("Done\n");
                break;
            case '2':
                printf("Data reading ...");
                read(fd, read_buf, 1024);
                printf("Done\n");
                printf("Data = %s\n, read_buf");
                break;
            case '3':
                close(fd);
                exit(1);
                break;
            default:
                printf("Enter invalid option %c\n", option);
                break;
        }
    }
    close(fd);
    return 0;
}