/*****************************************
 *  Copyright © 2001-2004
 *  Sigma Designs, Inc. All Rights Reserved
 *  Proprietary and Confidential
 ******************************************/
/**
* @file   ir.h
* @brief  IR related definitions, and function prototypes.
*/
                                                                                
/**
$Log : $
*/

#ifndef _IR_H_
#define _IR_H_
                                                                                
#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/ioctl.h>
#elif !defined(BOOTLOADER)
#include <sys/ioctl.h>
#endif /* __KERNEL__ */

#if defined(__KERNEL__) || !defined(BOOTLOADER)
/* ioctl commands for user level applications*/
#define IR_IOC_MAGIC           'I'
#define IR_IOCSETREPEATKEYS	_IO(IR_IOC_MAGIC, 0)
#define IR_IOCGETREPEATKEYS	_IO(IR_IOC_MAGIC, 1)
#define IR_IOCSETWAITPERIOD	_IO(IR_IOC_MAGIC, 2)
#define IR_IOCGETWAITPERIOD	_IO(IR_IOC_MAGIC, 3)
#endif /* __KERNEL__ || !BOOTLOADER */
                                                                                
#endif /* _IR_H_ */

