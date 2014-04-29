
/*********************************************************************
 Copyright (C) 2001-2009
 Sigma Designs, Inc. 
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 2 as
 published by the Free Software Foundation.
 *********************************************************************/

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>

#ifdef CONFIG_TANGO2
#include <asm/tango2/tango2_gbus.h>
#include <asm/tango2/hardware.h>
#include <asm/tango2/ir.h>
#elif defined(CONFIG_TANGO3)
#include <asm/tango3/tango3_gbus.h>
#include <asm/tango3/hardware.h>
#include <asm/tango3/ir.h>
#endif

/* For all SMP8xxx, it's always there */
#define WITH_RC5_CONTROL
#define WITH_RC6_CONTROL

#define USE_POWER_CHIP
#define USE_POWER_LED

#define KEY_RED
//#define DEBUG_IR
//#define ALL_KEYS_REPEATABLE
#define DEBUG_RED_KEY
#define SYS_gpio_base		SYS_gpio_dir

/* NEC Control */
#define IR_NEC_CTRL             (REG_BASE_system_block + SYS_gpio_base + 0x18)
#define IR_NEC_CAPTURE_DATA    	(REG_BASE_system_block + SYS_gpio_base + 0x1c)

#ifdef WITH_RC5_CONTROL
/* RC5 Control */
#define IR_RC5_CTRL             (REG_BASE_system_block + SYS_gpio_base + 0x20)
#define IR_RC5_DECODE_CLK_DIV  	(REG_BASE_system_block + SYS_gpio_base + 0x24)
#define IR_RC5_DECODER_DATA    	(REG_BASE_system_block + SYS_gpio_base + 0x28)
#define IR_RC5_INT_STATUS    	(REG_BASE_system_block + SYS_gpio_base + 0x2c)
#endif /* WITH_RC5_CONTROL */

#ifdef WITH_RC6_CONTROL
/* RC6 Control */
#define RC6_DWORDS		5	/* 5 dwords = 20 bytes */
#define IR_RC6_CTRL             (REG_BASE_system_block + SYS_gpio_base + 0xe0)
#define IR_RC6_T_CTRL           (REG_BASE_system_block + SYS_gpio_base + 0xe4)
#define IR_RC6_DATA_OUT0        (REG_BASE_system_block + SYS_gpio_base + 0xe8)
#define IR_RC6_DATA_OUT1        (REG_BASE_system_block + SYS_gpio_base + 0xec)
#define IR_RC6_DATA_OUT2        (REG_BASE_system_block + SYS_gpio_base + 0xf0)
#define IR_RC6_DATA_OUT3        (REG_BASE_system_block + SYS_gpio_base + 0xf4)
#define IR_RC6_DATA_OUT4        (REG_BASE_system_block + SYS_gpio_base + 0xf8)
#endif

/* The buffer size defines the size of circular buffer to keep the IR data */
#ifdef WITH_RC6_CONTROL
#define BUF_SIZE		6 	/* Minimum 20 bytes */
#else
#define BUF_SIZE		2
#endif

/* Wait period, to avoid bouncing? */
#define WAIT_PERIOD		100

/* Max. size of key table */
#define MAX_KEYS		32

/* The major device number and name */
#define IR_DEV_MAJOR		0
#define IR_DEV_NAME		"ir"

#define DRIVER_VERSION         "1.19"

MODULE_DESCRIPTION("TANGOX ir remote driver\n");
MODULE_AUTHOR("TANGOX standalone team");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

#ifdef WITH_RC5_CONTROL
static int rc5_clk_div = 48006;	/* 48006 = 1.778ms, 36018 = 1.334ms, */
				/* 59994 = 2.222ms */
module_param(rc5_clk_div, int, 0);
#endif /* WITH_RC5_CONTROL */

static int wp_var; /* Variable wait period */
static int wait_jiffies = ((((WAIT_PERIOD * HZ) / 1000) == 0) ? 1 : ((WAIT_PERIOD * HZ) / 1000));

static int wait_period = WAIT_PERIOD;
module_param(wait_period, int, 0);

static int buffer_size = BUF_SIZE;
module_param(buffer_size, int, 0);

static int max_keys = MAX_KEYS;
module_param(max_keys, int,0);

static int major_num = IR_DEV_MAJOR;
module_param(major_num, int, 0);

static int repeat_sends_zero = 0;
module_param(repeat_sends_zero, int, 0);

/* Wait queue, may be used if block mode is on */
static DECLARE_WAIT_QUEUE_HEAD(ir_wq);

/* Private data structure */
struct ir_private {
	unsigned long *buffer;		/* Circular buffer */
	unsigned long *key_table;	/* Table for repetition keys */
	unsigned p_idx;			/* Index of producer */
	unsigned c_idx; 		/* Index of consumer */
	unsigned ref_cnt;		/* Reference count */
	spinlock_t lock;		/* Spin lock */
	unsigned char b_mode;		/* Blocking mode or not */
	unsigned long last_jiffies;	/* Timestamp for last reception */
	unsigned int num_keys;		/* Number of keys in the table */
};

/* Some prototypes */
static int ir_open(struct inode *, struct file *);
static int ir_release(struct inode *, struct file *);
static int ir_read(struct file *, char *, size_t, loff_t *);
static long ir_unlocked_ioctl(struct file *, unsigned int, unsigned long);
static unsigned int ir_poll(struct file *, struct poll_table_struct *);

/* Global data */
static struct ir_private ir_priv;
static char *ir_devname = IR_DEV_NAME;
static int ir_irq = LOG2_CPU_INFRARED_INT + IRQ_CONTROLLER_IRQ_BASE;
static struct cdev cdev;
static struct class *ir_class;

static struct file_operations ir_fops = {
	open: ir_open,
	read: ir_read,
	unlocked_ioctl: ir_unlocked_ioctl,
	poll: ir_poll,
	release: ir_release,
	owner: THIS_MODULE,
};

#ifdef USE_POWER_CHIP
/*--------------Utility Functions -----------------*/
struct gbus;
struct gbus *pgBus = (void *) KSEG1ADDR(REG_BASE_cpu_block + CPU_remap);

#ifndef assert
#define assert(exp) ((exp)?1: (printk(KERN_INFO "==================> GOOD LUCK\n")))
#endif

#define DEFAULT_SCL_HI_TIMEOUT		25
#define OUT 1
#define IN 0
#define EMI2C_SEND_NACK			0x10000
#define EMI2C_SEND_NACK_IF_MISMATCH	0x20000
#define EMI2C_SEND_NACK_COMPARE_DATA	0x000FF
#define EMI2C_RD_SUB_STOPONRESTART	0x100000
#define EMI2C_RD_SUB_NONACKLASTBYTE	0x200000


//#define I2C_I2C_ADDR			0x72
#define I2C_WRITE			0xFE
#define I2C_READ			0x01
//#define I2C_ADDRESS			0x100				/* assume using end of page */
//#define NUM_TEST_BYTES			4
#define PAGE_BLOCK_0			0
#define PAGE_BLOCK_BYTES		256
#define START_SETUP_TIME		5				/* all are in microseconds */
#define START_HOLD_TIME			4
#define STOP_SETUP_TIME			5
#define STOP_HOLD_TIME			START_SETUP_TIME
#define SCL_HIGH_TIME			4
#define SCL_LOW_TIME			(10-SCL_HIGH_TIME)		/* assume 100 KHz clock frequency */
#define SCL_LOW_TIME_DIVIDEBY2		(SCL_LOW_TIME >> 1)  		/* don't divide by 2 */
#define WRITE_CYCLE_TIME		15000				/* cycle time is 15 msec max */

#define DIKS_POWER_SD	0xb54acb04	//Power Key for SD Remote Control	// ZTE : 0xbf40ff00
#define DIKS_POWER_HW	0x23dcff00    //Power Key for HUAWEI Remote Control
#define DIKS_POWER_SCTV	0xf50a0202    //Power Key for SCTV Remote Control
#define DIKS_RED_SD		0xa05fcb04    // Red key SIGMA Remote
#define DIKS_RED_SCTV		0xbf400202    // Red key SCTV Remote

void write_file(char *filename, char *data) 
{ 
	struct file *file;
	mm_segment_t oldfs;
	int ret;
	loff_t pos = 0;
	oldfs = get_fs();
	set_fs(get_ds());
	file = filp_open(filename, O_RDWR|O_CREAT|O_APPEND|O_LARGEFILE, 0777);

	ret = vfs_write(file, data, strlen(data), &pos);

	set_fs(oldfs);
	vfs_fsync(file, 0, 0);
	filp_close(file, NULL);
}
int flag =0;
void force_updater()
{
	char filename[100] = "/etc/update_fw.txt";
	char data[100] = "1";
	printk("write file  flag = %d\n", flag);
	if(flag ==0){
		printk("write file /etc/update_fw.txt success \n");
		write_file(filename, data);
		flag = 1;
	}
	if(flag ==1) printk("...");
}

static void em_udelay(unsigned long us)
{
	unsigned long t0, t1, d;
	unsigned long TotalDelay = us + 0;

        // implementation safe for delays smaller than 0xFFFFFFFF/27us = 159 sec
        // if SYS_xtal_in_cnt doesn't work - hang here !!
	t0 = gbus_read_uint32( pgBus, REG_BASE_system_block + SYS_xtal_in_cnt);
        do {
                t1 = gbus_read_uint32( pgBus, REG_BASE_system_block + SYS_xtal_in_cnt);
                if (t1 >= t0) d = t1 - t0;
                else d = 0xFFFFFFFF - t0 + t1 + 1;
        } while ( d < (27 * TotalDelay) );
}

/*---------------Hardware Control------------------*/
/* Clock Control */
static void em_sclio(unsigned int out)
{
	if(out)
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_dir, 0x00010001 << 0);
	else
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_dir, 0x00010000 << 0);
}

static void em_sclset(unsigned int high)
{
        if(high)
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010001 << 0);
	else
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010000 << 0);
}

static void em_sclget(unsigned int *pData)
{

	assert(pData);

	*pData = (gbus_read_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data) & (1 << 0))?1:0;
}

/* a device can hold the scl time for a wait state */
/* an i2c bus is supposed to have external pullups */
static int em_scltryhi(void)
{

	unsigned long count = 0;
	unsigned int data = 0;
	unsigned long timeout = DEFAULT_SCL_HI_TIMEOUT;



	if( 0x80 != 0 )
		timeout = 0x80;

        em_sclset(1); 	/* if it is in, we are floating hi/we have control
				 * and if it is out, set it to 1 */
	em_sclio(OUT);

	em_sclio(IN); /* float hi using external pullups */
	em_sclget(&data);
	while (data == 0)
	{
		if(count++ > timeout)
			return -1;
		/*sclset(1);*//* OUT is already 1*/
		em_sclio(OUT);

		em_udelay(1);
		em_sclio(IN);

		em_sclget(&data);
	}

	/* sclset(1);*//* OUT is already 1*/
	em_sclio(OUT);

	return 1;
}


/* Data Control */
static void em_sdaio(unsigned int out)
{


	if(out)
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_dir, 0x00010001 << 1);
	else
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_dir, 0x00010000 << 1);
}

static void em_sdaset(unsigned int high)
{


	if(high)
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010001 << 1);
	else
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010000 << 1);
}

static void em_sdaget(unsigned int *pData)
{

	assert(pData);

	*pData = (gbus_read_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data) & (1 << 1))?1:0;
}


/*-----------------I2C Calls----------------------*/
/* in between start and stop, we tristate
 * in between read/write/ack, we OUT D =0, C =0 */
int emi2c_start(void)
{
	unsigned int sda;
	//unsigned int scl;



	em_sdaset(1);
	em_sdaio(OUT);
	em_sdaget(&sda);

	if( sda == 0 )
		return -1;	/* sda is being held low */

	if( em_scltryhi() != 1 ) /* sets scl OUT */
		return -1;

	/* Then Start */
	em_sdaset(0);
	em_udelay(4); /*tHD;STA*/
	em_sclset(0);
	em_udelay(5); /*tLOW*/

	return 1;
}


int emi2c_stop(void)
{


	em_sdaset(0);
	em_sdaio(OUT);

	if( em_scltryhi() != 1 )
		return -1;

	em_udelay(4); /*tSU;STO*/
	em_sdaset(1);
	em_udelay(5); /*tBUF*/

	/* at this point, both outputs are 1, tristate for easy
	 * read or for other devices to grab bus */
	em_sdaio(IN);
	em_sclio(IN);
	return 1;
}


int emi2c_sendbyte(unsigned int uData, unsigned int *pNack)
{
	printk(KERN_INFO "emi2c_sendbyte uData = 0x%x\n", uData);
	unsigned int uMask=0x80;
	unsigned int i;
	unsigned int b;


	assert(pNack);

	em_sclget(&b);
	if( b != 0 )
		return -1;
	/*sclio(OUT);*//*should already be in output mode */
	em_sdaio(OUT);

	for( i = 0; i < 8; i++ )
	{
		if (uData & uMask)
			em_sdaset(1);
		else
			em_sdaset(0);
		uMask = uMask >> 1; /* get ready for next bit */

		em_udelay(1); /* tSU;DAT */

		/* clock */
		if( em_scltryhi() != 1 )
		{
			em_sdaset(0);
			return -1;
		}
		em_udelay(4); /* tHIGH */
		em_sclset(0);
		em_udelay(5); /* tHD;DAT */
	}

	/* Ack/Nack */
	em_sdaio(IN);
	em_udelay(1); /* let sda float for slave to drive */
	if( em_scltryhi() != 1 )
	{
		em_sdaset(0);
		em_sdaio(OUT);
		return -1;
	}
	em_udelay(4); /* tHIGH */
	em_sdaget(pNack); /* get the ack/Nack */
	em_sclset(0);

	em_sdaio(OUT);
	em_sdaset(0); /* hold the bus */

	em_udelay(5); /* tLOW */

	return 1;
}


int emi2c_readbyte(unsigned int* pData, unsigned long nackConfiguration)
{
	unsigned int b = 0;
	unsigned int uData = 0;
	unsigned int i = 0;


	assert(pData);

	// make sure we aren't using stray flags
	assert( (nackConfiguration & (~(EMI2C_SEND_NACK | EMI2C_SEND_NACK_IF_MISMATCH | EMI2C_SEND_NACK_COMPARE_DATA))) == 0 );


	em_sclget(&b); /* see if the slave is holding SCL */
	if( b != 0 )
		return -1;
	b = 0;

	em_sdaio(IN);
	em_udelay(1); /* slave now drives */
	for ( i = 0; i < 8; i++)
	{
		if( em_scltryhi() != 1 )
		{
			em_sdaset(0); /* hold the bus */
			em_sdaio(OUT);
			return -1;
		}
		em_udelay(4); /* tHIGH and tSU;DAT */
		uData = uData << 1;
		em_sdaget(&b); /* get data while HIGH, spec says data must be stable */
		uData |= b;
		em_sclset(0);
		em_udelay(5); /* tHD;DAT not necessary?  doesn't matter*/
	}

	if( nackConfiguration & EMI2C_SEND_NACK )
		em_sdaset(1); /* sda = 1 for nack */
	else
	{
		if( nackConfiguration & EMI2C_SEND_NACK_IF_MISMATCH )
		{
			if( uData != (nackConfiguration & EMI2C_SEND_NACK_COMPARE_DATA))
				em_sdaset(1); /* sda = 1 for nack */
			else
				em_sdaset(0); /* 0 for ack */
		}
		else
			em_sdaset(0); /* 0 for ack */
	}
	em_sdaio(OUT);

	em_udelay(1); /* tSU;DAT */

	if( em_scltryhi() != 1 )
	{
		em_sdaset(0);
		return -1;
	}
	em_udelay(4); /* tHIGH */
	em_sclset(0);
	em_udelay(5); /* tLOW ,diff from tHD;DAT but same */

	*pData = uData;
	return 1;
}

/*-----------------High Level I2C Calls----------------------*/
int emi2c_write_subaddress(
				unsigned int uSlaveAddress,
				unsigned int uSubAddress,
				unsigned int *pData,
				unsigned long uByteCount)
{
	int er;
	unsigned long uCount;
	unsigned int nack;

	unsigned int sa;

	assert(pData != 0);

	////printk(KERN_INFO "\n\t \033[0m Write:: Addr = 0x%x -- pData = 0x%x \033[0m ",uSubAddress,*pData);
	sa = uSlaveAddress & 0xFE;
	if( uSlaveAddress & 0x01 )
		printk(KERN_INFO "emi2c_write_subaddress - WARNING, original Slave address LSB=1, not typical write address, changed with &0xFE\n");

	if( (er = emi2c_start()) != 1)
	{
		//DPRINT(("emi2c_write_subaddress - emi2c_start() error %d\n", er));
		goto emi2c_write_subaddress_error;
	}
	/* slave address W, need & 0xFE*/
	if ( (er = emi2c_sendbyte(sa, &nack)) != 1 )
	{
		printk(KERN_INFO "emi2c_write_subaddress - Slave Address %x error %d\n", sa, er );
		goto emi2c_write_subaddress_error;
	}
	if( nack != 0 )
	{
		printk(KERN_INFO "emi2c_write_subaddress - Slave Address %x NACK %d\n", sa, nack);
		goto emi2c_write_subaddress_error;
	}


	if ( (er = emi2c_sendbyte(uSubAddress, &nack)) != 1 )
	{
		printk(KERN_INFO "emi2c_write_subaddress - Sub Address %x error %d\n", uSubAddress, er);
		goto emi2c_write_subaddress_error;
	}
	if( nack != 0 )
        {
			printk(KERN_INFO "emi2c_write_subaddress - Sub Address %x NACK %d\n", uSubAddress, nack);
                goto emi2c_write_subaddress_error;
        }


	for( uCount = 0; uCount < uByteCount; uCount++ )
	{
		//printk(KERN_INFO "\n\033[0m wr 0x%x \033[0m", pData[uCount]);
		if( (er = emi2c_sendbyte(pData[uCount], &nack)) != 1 )
		{
			//DPRINT(("emi2c_write_subaddress - Slave %x Sub %x Data %x Count %ld error %d\n", sa, uSubAddress, pData[uCount], uCount, er));
			goto emi2c_write_subaddress_error;
		}
		if( nack != 0 )
        	{
                	//DPRINT(("emi2c_write_subaddress - Slave %x Sub %x Data %x Count %ld NACK %d\n", sa, uSubAddress, pData[uCount], uCount, nack));
                	goto emi2c_write_subaddress_error;
        	}
	}

	if( (er = emi2c_stop()) != 1 )
	{
		//DPRINT(("emi2c_write_subaddress - emi2c_stop() error %d\n", er));
		return -1;
	}
	return 1;

emi2c_write_subaddress_error:
	if( (er = emi2c_stop()) != 1 )
	{
		//DPRINT(("emi2c_write_subaddress - emi2c_stop() error %d\n", er));
	}
	return -1;
}



int emi2c_read_subaddress(
				unsigned int uSlaveAddress,
				unsigned int uSubAddress,
				unsigned int *pData,
				unsigned long *pByteCount,
				unsigned long uFlags)
{
	int er;
	unsigned long uCount = 0;
	unsigned int nack;

	unsigned int saW;
	unsigned int saR;

	assert(pData != 0);
	assert(pByteCount != 0 );
	assert(*pByteCount != 0);

	// no stray flags.
	assert( (uFlags & (~(EMI2C_RD_SUB_STOPONRESTART |
	                     EMI2C_RD_SUB_NONACKLASTBYTE
	                    ))
	        ) == 0);

	saW = uSlaveAddress & 0xFE;
	saR = uSlaveAddress | 0x01;

	if( (er = emi2c_start()) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - emi2c_start() error %d\n", er));
		goto emi2c_read_subaddress_error;
	}
	if ( (er = emi2c_sendbyte(saW, &nack)) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - Slave Address %x error %d\n", saW, er ));
		goto emi2c_read_subaddress_error;
	}
	if( nack != 0 )
	{
		//DPRINT(("emi2c_read_subaddress - Slave Address %x NACK %d\n", saW, nack));
		goto emi2c_read_subaddress_error;
	}

	if ( (er = emi2c_sendbyte(uSubAddress, &nack)) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - Sub Address %x error %d\n", uSubAddress, er));
		goto emi2c_read_subaddress_error;
	}
	if( nack != 0 )
        {
                //DPRINT(("emi2c_read_subaddress - Sub Address %x NACK %d\n", uSubAddress, nack));
                goto emi2c_read_subaddress_error;
        }

	if( uFlags & EMI2C_RD_SUB_STOPONRESTART )
	{
		/* Sr Condition */
		if( (er = emi2c_stop()) != 1 )
		{
			//DPRINT(("emi2c_read_subaddress - emi2c_stop()SR error %d\n", er));
			goto emi2c_read_subaddress_error;
		}
	}

	if( (er = emi2c_start()) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - emi2c_start()SR error %d\n", er));
		goto emi2c_read_subaddress_error;
	}
	if ( (er = emi2c_sendbyte(saR, &nack)) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - Slave Address %x error %d\n", saR, er ));
		goto emi2c_read_subaddress_error;
	}
	if( nack != 0 )
	{
		//DPRINT(("emi2c_read_subaddress - Slave Address %x NACK %d\n", saR, nack));
		goto emi2c_read_subaddress_error;
	}


	for( uCount = 0; uCount < (*pByteCount); uCount++ )
	{
		unsigned long f = 0;

		// nack last byte, that is part of i2c spec
		if( ((uCount+1) == (*pByteCount)) )
		{
			if( uFlags & EMI2C_RD_SUB_NONACKLASTBYTE )
				f = 0;
			else
				f = EMI2C_SEND_NACK;
		}

		if( (er = emi2c_readbyte(pData+uCount, f)) != 1 )
		{
			// Return the byteCount on Error
			*pByteCount = uCount;

			printk(KERN_INFO "emi2c_read_subaddress - Slave %x Sub %x Data %x Count %ld error %d\n", saR, uSubAddress, pData[uCount], uCount, er);
			goto emi2c_read_subaddress_error;
		}
		printk(KERN_INFO "\n\033[0m rd 0x%x \033[0m", pData[uCount]);
	}
	// Return the byteCount after we've read all bytes.
	*pByteCount = uCount;


	if( (er = emi2c_stop()) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - emi2c_stop()SR error %d\n", er));
		return -1;
	}
	return 1;

emi2c_read_subaddress_error:
	if( (er = emi2c_stop()) != 1 )
	{
		//DPRINT(("emi2c_read_subaddress - emi2c_stop() error %d\n", er));
	}
	return -1;
}


/****************************************************************************
 Issue a START command to the EEPROM
 ***************************************************************************/
void i2c_start(void)
{
	em_sclset(1);
	em_sdaset(1);
	em_sdaio( OUT);
	em_udelay( START_SETUP_TIME );
	em_sdaset(0);
	em_udelay( START_HOLD_TIME );
	em_sclset(0);

	return;
}

/****************************************************************************
 Issue a STOP command to the EEPROM
 ***************************************************************************/
void i2c_stop(void)
{
	em_sclset(0);
	em_sdaio( OUT);
	em_sdaset(0);
	em_udelay( STOP_SETUP_TIME );
	em_sclset(1);
	em_udelay( STOP_SETUP_TIME );
	em_sdaset(1);
	em_udelay( WRITE_CYCLE_TIME );

	return;
}

/****************************************************************************
 Initializes the EEPROM
 ***************************************************************************/
void i2c_init(void)
{
	unsigned int i=0;
	em_sclset(1);
	em_sclio( OUT);

   	/* This ugly sequence is to make sure that the EEPROM*/
   	/* is forced back to know default state. */
	/* Issue 7 clk pulses, then i2c_stop. */

	for (i=0;i<7;i++){
		em_udelay( SCL_HIGH_TIME);
		em_sclset(0);
		em_udelay( SCL_LOW_TIME );
		em_sclset(1);
	}

	i2c_stop();

	em_sdaio( IN);

	return;
}

/****************************************************************************
 Writes a bit out to the EEPROM
 ***************************************************************************/
void i2c_writebit(unsigned int bit )
{
	em_udelay(SCL_LOW_TIME_DIVIDEBY2);
	em_sdaio( OUT);
	em_sdaset( bit );
	em_udelay(SCL_LOW_TIME_DIVIDEBY2);
	em_sclset( 1 );
	em_udelay(SCL_HIGH_TIME );
	em_sclset( 0 );

	return;
}

/****************************************************************************
 Reads a bit from the EEPROM
 ***************************************************************************/
unsigned int i2c_readbit(void)
{
	unsigned int bit;

	em_sdaio( IN);
	em_udelay(SCL_LOW_TIME );
	em_sclset( 1 );
	em_udelay( SCL_HIGH_TIME );
	em_sdaget( &bit);
	em_sclset( 0 );

 	return bit;
}
/****************************************************************************
 Issue an ACK to the EEPROM
 ***************************************************************************/
void i2c_ack(void)
{
	i2c_writebit( 0 );

	return;
}

/****************************************************************************
 Waits for an ACK from the EEPROM
 ***************************************************************************/
unsigned int i2c_waitack(void)
{
	unsigned int status;

	if ( !i2c_readbit() )
		status = true;
	else
		status = false;

	return status;
}

/****************************************************************************
 Writes a byte out to the EEPROM
 ***************************************************************************/
void i2c_writebyte(unsigned int uData )
{
	unsigned int bitcount;

	for ( bitcount = 8; bitcount > 0; bitcount-- )
	{
		/* Data is written MSB first */
		if ( uData & 0x80 )
			i2c_writebit( 1 );
		else
			i2c_writebit( 0 );
		uData <<= 1;
	}

	return;
}


/****************************************************************************
 Reads a byte from the EEPROM
 ***************************************************************************/
unsigned int i2c_readbyte(void)
{
	unsigned int uData;
	unsigned int bitcount;

	uData = 0;

	for ( bitcount = 8; bitcount > 0; bitcount-- )
	{
		/* Data is read MSB first */
		uData <<= 1;

		/* Read a bit and transfer it into the byte  */
		uData |= i2c_readbit();
	}

	return uData;
}


/****************************************************************************
 Reads data from the EEPROM and stores it in RAM
 ***************************************************************************/
unsigned int i2c_read(unsigned int subaddr, unsigned int epromaddr, unsigned int* memaddr, unsigned int nbbytes )
{
	unsigned int status = false;
	i2c_init();

	i2c_start();

	/* Dummy write cycle */
	i2c_writebyte( subaddr & I2C_WRITE );

	if ( i2c_waitack() )
	{
		i2c_writebyte( epromaddr );

		if ( i2c_waitack() )
		{
			i2c_start();

			/* Reading this time */
			i2c_writebyte( subaddr | I2C_READ );

			if ( i2c_waitack() )
			{
				while ( nbbytes )
				{
					/* Read a byte from the EEPROM and save it */
					*memaddr = i2c_readbyte();
					//printk(KERN_INFO "\n\033[0m i2c_read 0x%x \033[0m", memaddr[0]);
					if ( 1 != nbbytes )
						i2c_ack();

					++memaddr;
					--nbbytes;
				}

				i2c_stop();
				status = true;
			}
		}
	}

	return status;
}

/****************************************************************************
 Writes data to the EEPROM from RAM
 ***************************************************************************/
unsigned int i2c_write(unsigned int subaddr,unsigned int epromaddr, unsigned int* memaddr, unsigned int nbbytes )
{
	unsigned int status=false;
	unsigned char addrcur=epromaddr;
restart:
	i2c_init();
	i2c_start();
	//printk(KERN_INFO "subaddr = 0x%x\n", subaddr);
	i2c_writebyte( subaddr & I2C_WRITE );
	if ( i2c_waitack() )
	{
		//printk(KERN_INFO "addrcur = 0x%x\n", addrcur);
		i2c_writebyte( addrcur );
		if ( i2c_waitack() )
		{
			while ( nbbytes )
			{
				//printk(KERN_INFO "\n\033[0m i2c_write 0x%x \033[0m", memaddr[0]);
				i2c_writebyte( *memaddr );

				if ( i2c_waitack() ==  false )
				{
					printk(KERN_INFO "i2c_waitack() ==  false\n");
					nbbytes=0;	/* force loop exit with status remaining false */
				}
				else
				{
					++memaddr;
					--nbbytes;
					status = true;
					++addrcur;
					if(((addrcur %16)==0)&&(nbbytes!=0))
					{
							i2c_stop();
							goto restart;
					}

				}
			}
			i2c_stop();
		}
	}

	return status;
}

void powerOff()
{
	printk(KERN_DEBUG ">>>> PowerOff\n");
	unsigned int ucWriteByte[10];
	ucWriteByte[0] = 0x32; //0x32
	i2c_write(0x68, 0x10, ucWriteByte, 1);

	ucWriteByte[0] = 0xfc;
	i2c_write(0x68, 0x08, ucWriteByte, 1);
	ucWriteByte[0] = 0x80;
	i2c_write(0x68, 0x11, ucWriteByte, 1);

	ucWriteByte[0] = 0xb0;
	i2c_write(0x68, 0x20, ucWriteByte, 1);

	ucWriteByte[0] = 0x81;
	i2c_write(0x68, 0x21, ucWriteByte, 1);

	ucWriteByte[0] = 0xa0;
	i2c_write(0x68, 0x30, ucWriteByte, 1);

	
#if 1
	// 0xf50a0202  - SCTV Power key
	printk(KERN_DEBUG ">>>> POWER ON - SCTV remote control \n");
	ucWriteByte[0] = 0xf5;
	i2c_write(0x68, 0x32, ucWriteByte, 1);
	ucWriteByte[0] = 0x0a;
	i2c_write(0x68, 0x33, ucWriteByte, 1);
	ucWriteByte[0] = 0x02;
	i2c_write(0x68, 0x34, ucWriteByte, 1);
	ucWriteByte[0] = 0x02;
	i2c_write(0x68, 0x35, ucWriteByte, 1);
#else
	// 0x23dcff00 - Huawei Power Key
	ucWriteByte[0] = 0x23;
	i2c_write(0x68, 0x32, ucWriteByte, 1);
	ucWriteByte[0] = 0xdc;
	i2c_write(0x68, 0x33, ucWriteByte, 1);
	ucWriteByte[0] = 0xff;
	i2c_write(0x68, 0x34, ucWriteByte, 1);
	ucWriteByte[0] = 0x00;
	i2c_write(0x68, 0x35, ucWriteByte, 1);
#endif
		
	// 0xbf40ff00 - ZTE Power Key
	// 0xb54acb04 - Sigma Designs
	printk(KERN_DEBUG ">>>> POWER ON - SIGMA DESIGNS remote control \n");
	ucWriteByte[0] = 0xb5;
	i2c_write(0x68, 0x38, ucWriteByte, 1);
	ucWriteByte[0] = 0x4a;
	i2c_write(0x68, 0x39, ucWriteByte, 1);
	ucWriteByte[0] = 0xcb;
	i2c_write(0x68, 0x3a, ucWriteByte, 1);
	ucWriteByte[0] = 0x04;
	i2c_write(0x68, 0x3b, ucWriteByte, 1);

	ucWriteByte[0] = 0x81;
	i2c_write(0x68, 0x06, ucWriteByte, 1);

	//sleep(1);

}

#ifdef USE_POWER_LED
enum GPIO_type{
	GPIO_0 = 0,GPIO_1,GPIO_2,GPIO_3,GPIO_4,GPIO_5,GPIO_6,GPIO_7,
	GPIO_8,GPIO_9,GPIO_10,GPIO_11,GPIO_12,GPIO_13,GPIO_14,GPIO_15,
	GPIO_16,GPIO_17,GPIO_18,GPIO_19,GPIO_20,GPIO_21,GPIO_22,GPIO_23,
	GPIO_24,GPIO_25,GPIO_26,GPIO_27,GPIO_28,GPIO_29,GPIO_30,GPIO_31
};

static void ReadGpio(long* pData)
{
	*pData = gbus_read_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data);
}

static void Write_Gpio(enum GPIO_type pin, int ishigh){
	if(ishigh)
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010001 << pin);
	else
		gbus_write_uint32(pgBus, REG_BASE_system_block + SYS_gpio_data, 0x00010000 << pin);
}

static void TurnOnLED()
{
	//Set Led red
	Write_Gpio(GPIO_14, 0);
	Write_Gpio(GPIO_15, 1);
}

static void TurnOffLED()
{
	//Set Led green
	Write_Gpio(GPIO_15, 0);
	Write_Gpio(GPIO_14, 1);
}


static void SetAutoLED()
{
	long GpioData = 0xffff;
	int valGPIO_15 = 0, valGPIO_14 = 0;
	
	ReadGpio(&GpioData);
	valGPIO_15 = (GpioData & (1 << GPIO_15))?1:0;
	valGPIO_14 = (GpioData & (1 << GPIO_14))?1:0;

#ifdef DEBUG_IR
	printk("\n \033[32m SetAutoLED:: GpioData = 0x%lx\033[0m",GpioData);
	printk("\n \033[32m valGPIO_14 = %d - valGPIO_15 = %d\033[0m", valGPIO_14, valGPIO_15);
#endif

	if (valGPIO_14 == 0 && valGPIO_15 == 1){

	#ifdef DEBUG_IR
		printk("\n \033[32m LED IS BEING TURNED ON (GREEN) \033[0m");

	#endif
		TurnOffLED();
	}
	if (valGPIO_15 == 0 && valGPIO_14 == 1){
	#ifdef DEBUG_IR
		printk("\n \033[32m LED IS BEING TURNED OFF (RED) \033[0m");
	#endif
		TurnOnLED();
	}

#ifdef DEBUG_IR
	ReadGpio(&GpioData);
	printk("\n \033[32m SetAutoLED end:: GpioData = 0x%lx\033[0m",GpioData);
#endif


}
#endif //USE_POWER_LED

#endif // USE_POWER_CHIP

/* Check to see if we can find the key in the repetition key table */
static int ir_findkey(struct ir_private *priv, unsigned long key)
{
#ifdef ALL_KEYS_REPEATABLE
	return(key);
#else
	register unsigned int i;
	unsigned long *ptr = priv->key_table;

	for (i = 0; i < priv->num_keys; i++, ptr++)
		if (key == *ptr)
			return(key);
	return(0);
#endif
}

/* Produce data */
static void ir_produce(struct ir_private *priv, unsigned long status)
{
	
	static int power_key_count = 0;
	static int red_key_count = 0;
	static unsigned long power_key_lastcounted = 0;
	static unsigned long old_key = 0;
	static unsigned long save_key = 0;
	unsigned long data = 0;
	unsigned pidx;
	int repeat_key = 0;
#ifdef WITH_RC6_CONTROL
	static unsigned long save_rc6_key[RC6_DWORDS];	/* Only used for RC6 */
#endif

	spin_lock(&priv->lock);

#ifdef WITH_RC6_CONTROL
	if ((status & 0x80000000) != 0) {	// RC6 Data in IRQ
		unsigned long dx[RC6_DWORDS];
		dx[0] = gbus_read_reg32(IR_RC6_DATA_OUT0);
		dx[1] = gbus_read_reg32(IR_RC6_DATA_OUT1);
		dx[2] = gbus_read_reg32(IR_RC6_DATA_OUT2);
		dx[3] = gbus_read_reg32(IR_RC6_DATA_OUT3);
		dx[4] = gbus_read_reg32(IR_RC6_DATA_OUT4);
#ifdef DEBUG_IR
		printk(KERN_DEBUG "D0-4: 0x%08lx 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", dx[0], 
				dx[1], dx[2], dx[3], dx[4]);
#endif
		if ((dx[0] & 0x1f) != 0x1c) { 
#ifdef DEBUG_IR
			printk(KERN_DEBUG "Not acceptable RC6 code 0x%08lx\n", dx[0]);
#endif
			goto out;	/* Not valid */
		} else if (time_after(priv->last_jiffies + wait_jiffies, jiffies) && 
			(memcmp(&save_rc6_key, &dx, sizeof(unsigned long) * RC6_DWORDS) == 0)) {
			/* Throw away this key if this is the same key and came too
		   	   fast */
#ifdef DEBUG_IR
			printk(KERN_DEBUG "%s: same data\n", ir_devname);
#endif
			save_key = 0;
			goto out;
		} 

		/* Save the key */
		memcpy(&save_rc6_key, &dx, sizeof(unsigned long) * RC6_DWORDS);

		priv->last_jiffies = jiffies;
		pidx = priv->p_idx;	/* Save the old index before proceeding */

		/* Save it to buffer */
		if (((priv->p_idx + 1) % buffer_size) == priv->c_idx) {
			/* Adjust consumer index since buffer is full */
			/* Keep the latest one and drop the oldest one */
			priv->c_idx = (priv->c_idx + 1) % buffer_size;
			printk(KERN_WARNING "%s: buffer full\n", ir_devname);
		} else if (((priv->p_idx + 2) % buffer_size) == priv->c_idx) {
			/* Adjust consumer index since buffer is full */
			/* Keep the latest one and drop the oldest ones */
			priv->c_idx = (priv->c_idx + 2) % buffer_size;
			printk(KERN_WARNING "%s: buffer full\n", ir_devname);
		} else if (((priv->p_idx + 3) % buffer_size) == priv->c_idx) {
			/* Adjust consumer index since buffer is full */
			/* Keep the latest one and drop the oldest ones */
			priv->c_idx = (priv->c_idx + 3) % buffer_size;
			printk(KERN_WARNING "%s: buffer full\n", ir_devname);
		} else if (((priv->p_idx + 4) % buffer_size) == priv->c_idx) {
			/* Adjust consumer index since buffer is full */
			/* Keep the latest one and drop the oldest ones */
			priv->c_idx = (priv->c_idx + 4) % buffer_size;
			printk(KERN_WARNING "%s: buffer full\n", ir_devname);
		} else if (((priv->p_idx + 5) % buffer_size) == priv->c_idx) {
			/* Adjust consumer index since buffer is full */
			/* Keep the latest one and drop the oldest ones */
			priv->c_idx = (priv->c_idx + 5) % buffer_size;
			printk(KERN_WARNING "%s: buffer full\n", ir_devname);
		}

		priv->buffer[priv->p_idx] = dx[0];
		priv->p_idx = (priv->p_idx + 1) % buffer_size;
		priv->buffer[priv->p_idx] = dx[1];
		priv->p_idx = (priv->p_idx + 1) % buffer_size;
		priv->buffer[priv->p_idx] = dx[2];
		priv->p_idx = (priv->p_idx + 1) % buffer_size;
		priv->buffer[priv->p_idx] = dx[3];
		priv->p_idx = (priv->p_idx + 1) % buffer_size;
		priv->buffer[priv->p_idx] = dx[4];
		priv->p_idx = (priv->p_idx + 1) % buffer_size;

		/* Buffer was empty and block mode is on, wake up the reader */
		if (priv->c_idx == pidx)
			wake_up_interruptible(&ir_wq);

		goto out;
	}
#endif

#ifdef WITH_RC5_CONTROL
	if (status & 0x00000001) {	// RC5 IRQ
		data = gbus_read_reg32(IR_RC5_DECODER_DATA);
		gbus_write_reg32(IR_RC5_DECODER_DATA, 0);
		if ((data & 0x80000000) != 0)  /* Invalid RC5 decoder data */
			goto out;
	} else if (status & 0x00000002) {	// NEC IRQ
		data = gbus_read_reg32(IR_NEC_CAPTURE_DATA);
		gbus_write_reg32(IR_NEC_CAPTURE_DATA, 0);
	} else
		goto out;
#else
	data = gbus_read_reg32(IR_NEC_CAPTURE_DATA);
	gbus_write_reg32(IR_NEC_CAPTURE_DATA, 0);
#endif
	/* Discard not used data if needed */
	if (data == 0) {
		
		if (save_key == 0)
			goto out;
		old_key = 0;
		
#ifdef DEBUG_IR
		printk(KERN_DEBUG "%s: no data\n", ir_devname);
#endif

#ifdef USE_POWER_CHIP
		if ((save_key == DIKS_POWER_SCTV) || (save_key == DIKS_POWER_HW) || (save_key == DIKS_POWER_SD) || (save_key == DIKS_RED_SD) || (save_key == DIKS_RED_SCTV))
			wp_var = (wait_jiffies / 2) / (wait_period/100); 
#endif

		if (time_after(priv->last_jiffies + wp_var, jiffies)) {
#ifdef DEBUG_IR
			printk(KERN_DEBUG "%s: repetition too fast\n", ir_devname);
#endif
			goto out; 	/* Key repeats too fast, drop it */
		} else if (time_before(priv->last_jiffies + (wait_jiffies * 4), jiffies)) {
#ifdef DEBUG_IR
			printk(KERN_DEBUG "%s: got slow repetition, glitch?\n", ir_devname);
#endif
			save_key = 0;	/* Disallow key repitition */
			goto out;	/* Repeat too slow, drop it */
		} else if (ir_findkey(priv, save_key) == 0) {
#ifdef DEBUG_IR
			printk(KERN_DEBUG "%s: not repeatable key 0x%lx\n", ir_devname, save_key);
#endif
			goto out; /* If the key is not in the table, drop it */
		}
		
#ifdef USE_POWER_CHIP
		if ((save_key == DIKS_POWER_SCTV) || (save_key == DIKS_POWER_HW) || (save_key == DIKS_POWER_SD)){
			power_key_count++;
		}
	
		if (power_key_count > 10){
#ifdef DEBUG_IR
			printk(KERN_DEBUG "power_key_count = %d => SET POWER OFF\n", power_key_count);
#endif
			powerOff();
		}
// huyle
		if ((save_key == DIKS_RED_SD) || (save_key == DIKS_RED_SCTV)){
			red_key_count++;
		}
		if (red_key_count > 20){
#ifdef DEBUG_RED_KEY
			//printk(KERN_DEBUG "red_key_count = %d => SET VIA BIOS\n", power_key_count);
			force_updater();
			
#endif /*DEBUG_RED_KEY*/
		}
#endif

#ifdef DEBUG_IR
		printk(KERN_DEBUG "%s: got repeated key 0x%lx\n", ir_devname, save_key);
#endif
		data = save_key; /* Valid repeated key */
		repeat_key = 1;
	} else if (time_after(priv->last_jiffies + wait_jiffies, jiffies) && 
			(data == old_key)) {
		
		/* Throw away this key if this is the same key and came too
		   fast */
		
		
#ifdef DEBUG_IR
		printk(KERN_DEBUG "%s: same data\n", ir_devname);
#endif
		save_key = 0;
		goto out;
	} 
	else{
		if ((data == DIKS_POWER_SCTV) || (data == DIKS_POWER_HW) || (data == DIKS_POWER_SD) || (data == DIKS_RED_SD) || (data == DIKS_RED_SCTV)){
			//printk(KERN_DEBUG "\033[32m ================== SetAutoLED power_key_count = %d\n \033[0m", power_key_count);
			SetAutoLED();
			power_key_count = 0;
			red_key_count = 0;
		}
	}

	/* Shrink the wait time for repeat key if current one is repeated */
#ifdef USE_POWER_CHIP
	wp_var = (((repeat_key || (data == DIKS_POWER_SCTV)) || (repeat_key || (data == DIKS_POWER_HW)) || (repeat_key || (data == DIKS_POWER_SD)) || (repeat_key || (data == DIKS_RED_SD)) || (repeat_key || (data == DIKS_RED_SCTV))) ? (wait_jiffies / 2) : wait_jiffies); 
#else
	wp_var = (repeat_key ? (wait_jiffies / 2) : wait_jiffies); 
#endif
	priv->last_jiffies = jiffies;

	save_key = old_key = data;

#ifdef DEBUG_IR
	printk(KERN_DEBUG "%s: got data 0x%08lx\n", ir_devname, data);
#endif

#ifdef USE_POWER_CHIP
// 	If power and red key is being pressed, do not send data to buffer
	if(((power_key_count) && ((data == DIKS_POWER_SCTV) || (data == DIKS_POWER_HW) || (data == DIKS_POWER_SD))) || ((red_key_count) && ((data == DIKS_RED_SD))) || ((red_key_count) && ((data == DIKS_RED_SCTV))))
	{
		 data = 0;
	}
#endif
//@@ add huyle
#ifndef KEY_RED
// 	If red key is being pressed, do not send data to buffer
	if(((red_key_count) && ((data == DIKS_RED_SD))) || ((red_key_count) && ((data == DIKS_RED_SCTV))))
	{
		 data = 0;
	}
#endif
	pidx = priv->p_idx;	/* Save the old index before proceeding */

	if (repeat_sends_zero && repeat_key)
		data = 0; /* clear the data */

	/* Save it to buffer */
	if (((priv->p_idx + 1) % buffer_size) == priv->c_idx) {
		/* Adjust consumer index since buffer is full */
		/* Keep the latest one and drop the oldest one */
		priv->c_idx = (priv->c_idx + 1) % buffer_size;

		printk(KERN_WARNING "%s: buffer full\n", ir_devname);
	}

	priv->buffer[priv->p_idx] = data;
	priv->p_idx = (priv->p_idx + 1) % buffer_size;

	/* Buffer was empty and block mode is on, wake up the reader */
	if (priv->c_idx == pidx)
		wake_up_interruptible(&ir_wq);

out:
	spin_unlock(&priv->lock);
}

/* ISR for IR device */
static irqreturn_t ir_isr(int irq, void *dev_id)
{
	struct ir_private *priv = (struct ir_private *)dev_id;
	unsigned long status = 0, enabled = 0;

	if (priv != &ir_priv)		/* Paranoid check */
		return IRQ_NONE;

	// gbus_write_reg32(REG_BASE_CPU + CPU_edge_rawstat, IRQMASKOF(ir_irq));
	
	if ((gbus_read_reg32(IR_RC5_CTRL) & (1 << 4)) == 0) /* NEC is enabled */
		enabled = 1;
#ifdef WITH_RC6_CONTROL 
	if ((gbus_read_reg32(IR_RC6_CTRL) & 0x1) != 0) /* RC6 is enabled */
		enabled = 1;
#endif
#ifdef WITH_RC5_CONTROL
	if ((gbus_read_reg32(IR_RC5_CTRL) & 0x1) != 0) /* RC5 is enabled */
		enabled = 1;
#endif
	if (enabled == 0) /* none of the controller is enabled */
		return IRQ_NONE;

#ifdef WITH_RC6_CONTROL 
	status = gbus_read_reg32(IR_RC6_CTRL);
	if ((status & 0xc0000000) != 0) {
		gbus_write_reg32(IR_RC6_CTRL, status); /* Clear interrupt */
#ifdef DEBUG_IRx
		if ((status & 0x40000000) != 0) 
			printk(KERN_DEBUG "RC6 Err IRQ (0x%08lx)\n", status);
#endif
		if ((status & 0x80000000) != 0) {
			/* We have RC6 data */
#ifdef DEBUG_IRx
			printk(KERN_DEBUG "RC6 Datain IRQ (0x%08lx)\n", status);
#endif
			printk("ir_produce A\n");
			ir_produce(priv, status);

#ifdef WITH_RC5_CONTROL
			/* Force to clear RC5 interrupt status */
			status = gbus_read_reg32(IR_RC5_INT_STATUS);
			if ((status & 0x00000003) != 0)
				gbus_write_reg32(IR_RC5_INT_STATUS, status); /* Clear interrupt if any */
#endif
			return IRQ_HANDLED;
		} 
	}
#endif

#ifdef WITH_RC5_CONTROL
	status = gbus_read_reg32(IR_RC5_INT_STATUS);
#ifdef DEBUG_IRx
	if ((status & 0x00000001) != 0) {
		/* RC5 interrupt */
		printk(KERN_DEBUG "RC5 IRQ (0x%08lx)\n", status);
	}
	if ((status & 0x00000002) != 0) {
		/* NEC interrupt */
		printk(KERN_DEBUG "NEC IRQ (0x%08lx)\n", status);
	}
#endif
	gbus_write_reg32(IR_RC5_INT_STATUS, status); /* Clear interrupt */
	status &= 0x00000003;
#endif /* WITH_RC5_CONTROL */

	printk("ir_produce B\n");
	ir_produce(priv, status);
	return IRQ_HANDLED;
}

/* Reading from driver's buffer, note that it can return read size
   less than specified */
static int ir_consume(void *dev_id, unsigned long *buf, int count)
{
	struct ir_private *priv = (struct ir_private *)dev_id;
	int cnt = 0;
	unsigned long flags;

	/* If block mode is on, check the emptiness of buffer */
	if (priv->b_mode != 0) {
		/* Sleep when buffer is empty */
		wait_event_interruptible(ir_wq, priv->c_idx != priv->p_idx);
	}
	if (signal_pending(current) || (priv->c_idx == priv->p_idx))
		return(cnt);

	spin_lock_irqsave(&priv->lock, flags);

	/* Get the data out and adjust consumer index */
	for (cnt = 0; (priv->c_idx != priv->p_idx) && (cnt < count); cnt++) {
		*buf = priv->buffer[priv->c_idx];
		priv->c_idx = (priv->c_idx + 1) % buffer_size;
		buf++;
	}

	spin_unlock_irqrestore(&priv->lock, flags);

	return(cnt);
}

/* Reading data */
static int ir_read(struct file *fptr, char *bufptr, size_t size, loff_t *fp)
{
	unsigned long buf[buffer_size];
	int count;

	/* Check the alignment */
	if (size % sizeof(unsigned long)) {
		printk(KERN_WARNING "%s: read size not aligned to %d\n",
			ir_devname, sizeof(unsigned long));
		return(-EIO);
	}

	count = ir_consume(fptr->private_data, &buf[0], 
			size / sizeof(unsigned long)) * sizeof(unsigned long);

	/* Get the data to user */
	if (count && copy_to_user(bufptr, (char *)&buf[0], count)) 
		return(-EFAULT);

	return(count);
}

/* ioctl function */
static long ir_unlocked_ioctl(struct file *fptr, unsigned int cmd, unsigned long arg)
{
	unsigned long *ptr = (unsigned long *)arg;
	unsigned long key_no = 0;
	struct ir_private *priv = (struct ir_private *)fptr->private_data;
	unsigned long flags;

	if (ptr == NULL)
		return(-EIO);

	spin_lock_irqsave(&priv->lock, flags);

	switch(cmd) {
		case IR_IOCSETREPEATKEYS: /* Set the repetition keys */
			if (copy_from_user(&key_no, ptr, sizeof(unsigned long))) 
				goto efault_out;
			else if ((key_no <= 0) || (key_no > max_keys)) 
				goto einval_out;
			else if (copy_from_user(priv->key_table, ptr + 1, sizeof(unsigned long) * key_no)) 
				goto efault_out;
			priv->num_keys = key_no;
			break;
		case IR_IOCGETREPEATKEYS: /* Get the repetition keys */
			key_no = priv->num_keys;
			if (copy_to_user(ptr, &key_no, sizeof(unsigned long))) 
				goto efault_out;
			else if (key_no > 0) {
				if (copy_to_user(ptr + 1, priv->key_table, sizeof(unsigned long) * key_no)) 
					goto efault_out;
			}
			break;
		case IR_IOCSETWAITPERIOD:
			wait_period = arg;
			wait_jiffies = wp_var = ((((wait_period * HZ) / 1000) == 0) ? 1 : ((wait_period * HZ) / 1000));
			break;
		case IR_IOCGETWAITPERIOD: 
			if (copy_to_user(ptr, &wait_period, sizeof(int))) 
				goto efault_out;
			break;
		default:
			goto einval_out;
			break;
	}

	spin_unlock_irqrestore(&priv->lock, flags);
	return(0);
einval_out:
	spin_unlock_irqrestore(&priv->lock, flags);
	return(-EINVAL);

efault_out:
	spin_unlock_irqrestore(&priv->lock, flags);
	return(-EFAULT);
}

/* Poll function */
static unsigned int ir_poll(struct file *fptr, struct poll_table_struct *ptable)
{
	struct ir_private *priv = (struct ir_private *)fptr->private_data;
	unsigned int mask = 0;
	unsigned long flags;

	poll_wait(fptr, &ir_wq, ptable);

	spin_lock_irqsave(&priv->lock, flags);
	if (priv->c_idx != priv->p_idx)
		mask |= (POLLIN | POLLRDNORM);
	spin_unlock_irqrestore(&priv->lock, flags);

	return(mask);
}

/* Open the device */
static int ir_open(struct inode *inode_ptr, struct file *fptr)
{
	/* This device is exclusive, that is, only one process can use it */
	if (ir_priv.ref_cnt != 0) {
		printk(KERN_WARNING "%s: exclusive access only\n", ir_devname);
		return(-EIO);
	/* This device is read-only */
	} else if ((fptr->f_flags & O_ACCMODE) != O_RDONLY) {
		printk(KERN_WARNING "%s: read-only device\n", ir_devname);
		return(-EIO);
	} 

	/* Set the block mode and increase reference count */
	ir_priv.ref_cnt++;
	ir_priv.b_mode = ((fptr->f_flags & O_NONBLOCK) ? 0 : 1);
	ir_priv.last_jiffies = jiffies;

	/* Flush the buffer */
	ir_priv.p_idx = ir_priv.c_idx = 0;

	fptr->f_op = &ir_fops;
	fptr->private_data = (void *)&ir_priv;

	/* Enable the NEC device (CTRL register) */
	/*	31:30 - reserved */
	/*	29:24 	IR_CAPTURE_NBITS [5:0] -> set to 0x1f */
	/*	23:22 - reserved */
	/*	21:16 	GPIO_INFREARED_SEL [5:0] -> set to 12 */
	/*	15:14 - reserved */
	/*	13:0	IR_PREDIV_DEVIDER [13:0] -> set to 0x3b10 */
	gbus_write_reg32(REG_BASE_system_block + SYS_gpio_dir, GPIO_DIR_INPUT(12));
	gbus_write_reg32(IR_NEC_CAPTURE_DATA, 0);
	gbus_write_reg32(IR_NEC_CTRL, 0x1f0c3b10);

	wp_var = wait_jiffies;

	printk(KERN_DEBUG "%s: Enable NEC decoder (0x%08x)\n", 
			ir_devname, (u32)gbus_read_reg32(IR_NEC_CAPTURE_DATA));

#ifdef WITH_RC5_CONTROL
	/* Enable the RC5 device (CTRL register) */
	/*	31:10 - reserved */
	/*	9	IR_RC5_INT_ENABLE -> set */
	/*	8	IR_NEC_INT_DISABLE */
	/*	7	IR_DEBOUNCE_SEL1 -> set */
	/*	6	IR_DEBOUNCE_SEL0 -> set */
	/*	5	IR_DEBOUNCE_ENABLE -> set */
	/*	4	IR_NEC_DISABLE */
	/*	3	IR_RSYNC_1/4 -> set */
	/*	2	IR_RSYNC_1/8 */
	/*	1	IR_SIGNAL_INVERT */
	/*	0	IR_RC5_DECODE_ENABLE -> set */
	gbus_write_reg32(IR_RC5_DECODE_CLK_DIV, rc5_clk_div);
	gbus_write_reg32(IR_RC5_DECODER_DATA, 0);
	gbus_write_reg32(IR_RC5_CTRL, 0x000002e9);

	printk(KERN_DEBUG "%s: Enable RC5 decoder (0x%08x)\n", 
			ir_devname, (u32)gbus_read_reg32(IR_RC5_DECODER_DATA));
#endif /* WITH_RC5_CONTROL */

#ifdef WITH_RC6_CONTROL
	/* Enable the RC6 device (CTRL register) */
	/*	7	IR_RC6_DATA_IN_INT_ENABLE -> set */
	/*	6	IR_RC6_ERROR_INT_ENABLE -> set */
	/*	5:2	reserved */
	/*	1	IR_SIGNAL_INVERT */
	/*	0	IR_RC6_DECODE_ENABLE -> set */
	gbus_write_reg32(IR_RC6_CTRL, 0xc1);
	/* Tolerance and Duration */
	/*	31:18	Tolerance (typ. 0xbb8) */
	/*	17:0	Duration (typ. 0x2ee0) */
	gbus_write_reg32(IR_RC6_T_CTRL, (0xbb8 << 18) | 0x2ee0);

	printk(KERN_DEBUG "%s: Enable RC6 decoder\n", ir_devname);
#endif
	return(0);
}

/* Close the device */
static int ir_release(struct inode *inode_ptr, struct file *fptr) 
{
	/* Disable the NEC device */
	printk(KERN_DEBUG "%s: Disable NEC decoder\n", ir_devname);
	gbus_write_reg32(IR_NEC_CAPTURE_DATA, 0);
	gbus_write_reg32(IR_NEC_CTRL, 0);

#ifdef WITH_RC5_CONTROL
	/* Disable RC5 control */
	printk(KERN_DEBUG "%s: Disable RC5 decoder\n", ir_devname);
	gbus_write_reg32( IR_RC5_CTRL, (gbus_read_reg32(IR_RC5_CTRL) & 0xfffffdfe) | 0x00000100);
#endif /* WITH_RC5_CONTROL */

#ifdef WITH_RC6_CONTROL
	printk(KERN_DEBUG "%s: Disable RC6 decoder\n", ir_devname);
	gbus_write_reg32(IR_RC6_CTRL, 0xc0000000);
#endif /* WITH_RC6_CONTROL */

	/* Adjust reference count */
	ir_priv.ref_cnt--;

	return(0);
}

int __init ir_init_module(void)
{
	int status = 0;
	dev_t devid;

	extern int tangox_ir_enabled(void);
	if (tangox_ir_enabled() == 0)
		return(0);

	wait_jiffies = wp_var = ((((wait_period * HZ) / 1000) == 0) ? 1 : ((wait_period * HZ) / 1000));

	/* Initialize private data structure */
	memset(&ir_priv, 0, sizeof(struct ir_private)); 
	spin_lock_init(&ir_priv.lock);

#ifdef WITH_RC6_CONTROL
	if (buffer_size < 6) {
		printk(KERN_ERR "%s: buffer size (%d) error, minimum 6.\n", ir_devname,
			buffer_size); 
		return(-EIO);
	} 
#else
	if (buffer_size < 2) {
		printk(KERN_ERR "%s: buffer size (%d) error, minimum 2.\n", ir_devname,
			buffer_size); 
		return(-EIO);
	} 
#endif

	if ((ir_priv.buffer = kmalloc(buffer_size * sizeof(unsigned long), GFP_KERNEL)) == NULL) {
		printk(KERN_ERR "%s: out of memory for buffer\n", ir_devname); 
		return(-ENOMEM);
	} else if ((ir_priv.key_table = kmalloc(max_keys * sizeof(unsigned long), GFP_KERNEL)) == NULL) {
		printk(KERN_ERR "%s: out of memory for key table\n", ir_devname); 
		kfree(ir_priv.buffer);
		return(-ENOMEM);
	}

	/* Register device, and may be allocating major# */
	if (major_num) {
		devid = MKDEV(major_num, 0);
		status = register_chrdev_region(devid, 1, "ir");
	} else {
		status = alloc_chrdev_region(&devid, 0, 1, "ir");
		major_num = MAJOR(devid);
	}
	if (status < 0) {
		printk(KERN_ERR "%s: cannot get chrdev_region\n", ir_devname);
		kfree(ir_priv.buffer);
		kfree(ir_priv.key_table);
		return(status);
	}

	cdev_init(&cdev, &ir_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &ir_fops;
	if ((status = cdev_add(&cdev, devid, 1)) < 0) {
		printk(KERN_ERR "%s: cannot get major number\n", ir_devname); 
		unregister_chrdev_region(MKDEV(major_num, 0), 1);
		kfree(ir_priv.buffer);
		kfree(ir_priv.key_table);
		return(status);
	}
	if (IS_ERR(ir_class = class_create(THIS_MODULE, "ir_device"))) {
		printk(KERN_ERR "%s: error creating ir_device class.\n", ir_devname);
		cdev_del(&cdev);
		unregister_chrdev_region(MKDEV(major_num, 0), 1);
		kfree(ir_priv.buffer);
		kfree(ir_priv.key_table);
		return(-EIO);
	}
	if (device_create(ir_class, NULL, MKDEV(major_num, 0), NULL, "ir") == NULL) {
		printk(KERN_ERR "%s: error creating ir_device.\n", ir_devname);
		class_destroy(ir_class);
		cdev_del(&cdev);
		unregister_chrdev_region(MKDEV(major_num, 0), 1);
		kfree(ir_priv.buffer);
		kfree(ir_priv.key_table);
		return(-EIO);
	}

	/* Make sure interrupt is disabled, will be re-enabled in device
	   open stage */
	gbus_write_reg32(IR_NEC_CTRL, 0);

#ifdef WITH_RC5_CONTROL
	gbus_write_reg32(IR_RC5_CTRL, (gbus_read_reg32(IR_RC5_CTRL) & 0xfffffdfe) | 0x00000100);
#endif

#ifdef WITH_RC6_CONTROL
	gbus_write_reg32(IR_RC6_CTRL, 0xc0000000);
#endif

	/* Hook up ISR */
	if (request_irq(ir_irq, ir_isr, IRQF_SHARED, ir_devname, &ir_priv) != 0) {
		printk(KERN_ERR "%s: cannot register IRQ (%d)\n", ir_devname,
			ir_irq);
		device_destroy(ir_class, MKDEV(major_num, 0));
		class_destroy(ir_class);
		cdev_del(&cdev);
		unregister_chrdev_region(MKDEV(major_num, 0), 1);
		kfree(ir_priv.buffer);
		kfree(ir_priv.key_table);
		return(-EIO);
	}
	
	printk(KERN_INFO "####### SMP86xx %s (%d:0): driver loaded (wait_period = %dms, "
		"buffer_size = %d)\n", ir_devname, major_num, wait_period, buffer_size);
	return(0);
}

void __exit ir_cleanup_module(void)
{
	extern int tangox_ir_enabled(void);
	if (tangox_ir_enabled() == 0)
		return;

	free_irq(ir_irq, &ir_priv);
	device_destroy(ir_class, MKDEV(major_num, 0));
	class_destroy(ir_class);
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(major_num, 0), 1);

	if (ir_priv.buffer != NULL)
		kfree(ir_priv.buffer);
	if (ir_priv.key_table != NULL)
		kfree(ir_priv.key_table);

	printk(KERN_INFO "%s: driver unloaded\n", ir_devname);
}

module_init(ir_init_module);
module_exit(ir_cleanup_module);

