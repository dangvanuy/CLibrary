
/* 
 * Sample IR user level program 
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <sys/ioctl.h>

#include "include/ir.h"

/* ioctl commands for user level applications*/
#define IR_IOC_MAGIC           'I'
#define IR_IOCSETREPEATKEYS	_IO(IR_IOC_MAGIC, 0)
#define IR_IOCGETREPEATKEYS	_IO(IR_IOC_MAGIC, 1)
#define IR_IOCSETWAITPERIOD	_IO(IR_IOC_MAGIC, 2)
#define IR_IOCGETWAITPERIOD	_IO(IR_IOC_MAGIC, 3)

/* IR device node and file handle */
static const char *dev = "/dev/ir";

/* Get the repeatation keys: only applicable for NEC remote */
int ir_get_repetition_keys(unsigned int *num_keys, unsigned long *keys)
{
	int fno;
	unsigned long buf[256];
	int res;

	if ((fno = open(dev, O_RDONLY|O_NONBLOCK)) < 0) {
		printf("error opening %s\n", dev);
		return(-1);
	}

	res = ioctl(fno, IR_IOCGETREPEATKEYS, (unsigned long)buf);
	if (res == 0)
	{
		*num_keys = buf[0];	/* # of kets and followed bt the keys */
		memcpy(keys, &buf[1], sizeof(unsigned long) * buf[0]);
	}
	
	close(fno);
	
	return res;
}

/* Set the repeatation keys: only applicable for NEC remote */
int ir_set_repetition_keys(unsigned int num_keys, unsigned long *keys)
{
	int fno;
	unsigned long buf[256];
	int res;

	if ((fno = open(dev, O_RDONLY|O_NONBLOCK)) < 0) {
		printf("error opening %s\n", dev);
		return(-1);
	}

	buf[0] = num_keys;	/* # of kets and followed bt the keys */
	memcpy(&buf[1], keys, sizeof(unsigned long) * buf[0]);
	res = ioctl(fno, IR_IOCSETREPEATKEYS, (unsigned long)buf);
	/*if(ioctl(fno, IR_IOCSETWAITPERIOD, (unsigned long)400) )
	{
		printf("can't set IR_IOCSETWAITPERIOD >>>>>>>>>>>>\n");
	}*/
	close(fno);
	
	return res;
}
