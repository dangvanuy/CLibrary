#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct
{
	unsigned char tag_type;            // 8: audio, 9: video, 18: script data
	int data_size;          // size of the data
	int timestamp;
} tFlvHeader;


int gBufPos = 0;

unsigned char* gBuf;

void bitsSkip(int nums_of_bit)
{
	gBufPos += nums_of_bit;
	printf("gBufPos = %d\n", gBufPos);
}

int bitsGet( int num )
{
   int      bitsLeft = 0;
   int value    = 0;

   while( num <= 32 && num > 0  )
   {

      bitsLeft = 8 - ( gBufPos % 8 );
      if( ( bitsLeft < 8 ) && ( num < bitsLeft ) )
      {
         value    = ( gBuf[gBufPos / 8] >> ( bitsLeft - num ) ) & ( 1 << num );;
         gBufPos += num;
         num      = 0;
      }
      else if( bitsLeft < 8 )
      {
         value    = ( gBuf[gBufPos / 8] )  & ( ( 1 << bitsLeft ) - 1 );
         gBufPos += bitsLeft;
         num     -= bitsLeft;
      }
      else if( num >= 8 )
      {
         value   <<= 8;
         value    |= gBuf[gBufPos / 8];
         gBufPos  += 8;
         num      -= 8;
      }
      else
      {
         value <<= num;
         value |= gBuf[gBufPos / 8] >> ( 8 - num );
         gBufPos += num;
         num = 0;
      }
   }

   return value;
}

void headerGet( tFlvHeader * flv_th )
{
	printf("headerGet\n");
   flv_th->tag_type  = bitsGet( 8 );
   flv_th->data_size = bitsGet( 3 * 8 );
   flv_th->timestamp = bitsGet( 3 * 8 );
   printf("tag_type = %d, data_size = %d", flv_th->tag_type, flv_th->data_size);
   bitsSkip( 4 * 8 );
}

int gFd = -1;
int main(int agrc, char* argv[])
{
	 tFlvHeader flv_th = {0, 0, 0};
	gFd = open( argv[1], O_RDONLY );
	if (gFd < 0){
		printf("Can not opne file\n");
		return 0;
	}
	bitsSkip( 9 * 8 );

#if 1
	//while( true)
	{
		bitsSkip( 4 * 8 );
		headerGet(&flv_th );
		if( 8 == flv_th.tag_type )
		{
			//audioSend( &flv_th );
			printf("is AUDIO\n");
		}
		else if( 9 == flv_th.tag_type )
		{
			//videoSend( &flv_th );
			printf("is VIDEO\n");
		}
		else
		{
			bitsSkip( flv_th.data_size * 8 );
		}
	}


#endif
	close( gFd );
	return 0;
}

