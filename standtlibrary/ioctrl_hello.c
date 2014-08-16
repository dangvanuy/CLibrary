#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define FILTER_INIT     1
#define FILTER_START    2
#define FILTER_CLEAN    3
#define FILTER_STOP     4
#define FILTER_GETBCNT  5
#define FILTER_GETPCNT  6
#define FILTER_GETPOFS  7
#define FILTER_GETSBCNT 8


// Multi-stream related ioctl codes
typedef struct {
    int  number;  // Number of filtered stream
    char ip[32];  // IP of filtered stream
    int  port;    // Port of filtered stream
} IpFilter_Stream;
#define FILTER_STRESET  10 
#define FILTER_STGETCNT 11                             
#define FILTER_STGET    12 
#define FILTER_STADD    13 
#define FILTER_STNAMERM 14 
#define FILTER_STNUMBRM 15 
#define FILTER_QUERY    16 

static int ioctl_Open(char * device)
{
   int fd = -1;
 
   fd = open(device, O_RDONLY);

   return fd;
}

static void ioctl_Close(int fd)
{
    if(fd)
    {
       close(fd);
    }
}

static int ioctl_Init(int fd)
{
    int rc = -1;

    if(fd < 0 )
    {
        printf("Error, mc filter is not in run\n");
        return rc;
    }

    if( (rc = ioctl( fd, FILTER_INIT, 0 )) != 0 )
    {
        printf("Error, FILTER_INIT failed\n");
    }

    return rc;
}

static int ioctl_Start(int fd)
{
    int rc = -1;

    if(fd < 0 )
    {
        printf("Error, mc filter is not in run\n");
        return rc;
    }

    if( (rc = ioctl( fd, FILTER_START, 0 )) != 0 )
    {
        printf("Error, FILTER_START failed\n");
    }

    return rc;
}

static int ioctl_Stop(int fd)
{
    int rc = -1;

    if(fd < 0 )
    {
        printf("Error, mc filter is not in run\n");
        return rc;
    }

    if( (rc = ioctl(fd, FILTER_STOP, 0 )) != 0 )
    {
        printf("Error, FILTER_STOP failed\n");
    }

    return rc;
}

static int ioctl_Clean(int fd)
{
    int rc = -1;

    if(fd < 0 )
    {
        printf("Error, mc filter is not in run\n");
        return rc;
    }

    if( (rc = ioctl(fd, FILTER_CLEAN, 0 )) != 0 )
    {
        printf("Error, FILTER_CLEAN failed\n");
    }

    return rc;
}

static int ioctl_RemoveStream(int fd, IpFilter_Stream *filter)
{
    IpFilter_Stream stream;
    int rc = 0;

    strcpy(&stream.ip[0], filter->ip);
    stream.port = filter->port;

    if( (rc = ioctl(fd, FILTER_STNAMERM, &stream )) != 0 )
    {
        printf("Error, FILTER_STNAMERM failed\n");
    }

    return rc;
}

static int ioctl_AddStream(int fd, IpFilter_Stream* filter)
{
   IpFilter_Stream stream;

    int rc = 0;

    strcpy(&stream.ip[0], filter->ip);
    stream.port = filter->port;

    if( (rc = ioctl(fd, FILTER_STADD, &stream )) != 0 )
    {
        printf("Error, FILTER_STADD failed\n");
    }

    return rc;
}

static int ioctl_Query(int fd, IpFilter_Stream* filter)
{
    IpFilter_Stream stream;

    int rc = 0;

    strcpy(&stream.ip[0], filter->ip);
    stream.port = filter->port;

    stream.number = -1;

    if( (rc = ioctl(fd, FILTER_QUERY, &stream )) != 0 )
    {
        printf("Error, FILTER_QUERY failed rc(%d)\n", rc);
        return rc;
    }

    return stream.number;
}

static int ioctl_GetCnt(int fd)
{
    int rc = 0;
    int cnt = 0;

    if( (rc= ioctl(fd, FILTER_STGETCNT, &cnt)) != 0 )
    {
        printf("Error, FILTER_STGETCNT failed\n");
        return rc;
    }

    return cnt;
}

static int ioctl_ReadData( int fd, char* buf, size_t size )
{
   struct timeval timeout;
   fd_set         rset;
   unsigned int readCnt = 0;
   int            rc = -1;

   FD_ZERO( &rset );
   FD_SET( fd, &rset );

   timeout.tv_sec = 5;
   timeout.tv_usec = 0;

   rc = select( fd + 1, &rset, NULL, NULL, &timeout );

   if( rc < 0 )
   {
      printf("Error, Filter select error rc(%d)\n", rc);
      return rc;
   }

   if( rc == 0 )
   {
      printf("Select time-out, read it anyway\n");
      readCnt = read(fd, buf, size );
      if(readCnt == 0 )
      {
         printf("rc = 0, No data to read\n");
      }else {
		printf("Data were readed: %s\n", buf);
	  }
   }
   else
   {
      if( FD_ISSET(fd, &rset ) ) {
         readCnt = read(fd, buf, size );
         if(readCnt == 0 )
         {
            printf("No data to read\n");
         }else {
			printf("Data were readed: %s\n", buf);		  
		 }
      }
      else
      {
         printf("No data to read\n");
      }
   }

   return readCnt;
}
//mknod /dev/myDevice c 125 0
#define MY_DEVICE "/dev/myDevice"

int main(int argc, char *argv[])
{
	int myDeviceFd;
	myDeviceFd = ioctl_Open(MY_DEVICE);
	if( myDeviceFd < 0 ) {
		printf("Can not open your device: %s\n", MY_DEVICE);
	}
	else{
		printf("Open your device %s successfully\n", MY_DEVICE);
	}
	if (myDeviceFd >= 0){
		ioctl_Init(myDeviceFd);
		ioctl_Start(myDeviceFd);
		
		char * buff;
		ioctl_ReadData(myDeviceFd, buff, 1);
	}
	div_t ans = div(8, 3);
	printf("Thuong %d, so du: %d\n", ans.quot, ans.rem);
}
//gcc -o ioctrl_hello ioctrl_hello.c


