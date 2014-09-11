/*****************************************
 Copyright  2005
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
 	@file	hotplug_handler.c
	@brief	Handles the messages received from the hotplug system regarding the external storage device

	@author Sebastian Manciulea
	@date	2008-06-02

	$Header: /kenvi/cvsroot/TAC_Box/sigmaobjectsextension/mediasetting/mediasettinglib/include/hotplug_handler.h,v 1.1.1.1 2013/02/20 09:35:11 liempham Exp $
 */

#ifndef _HOTPLUG_HANDLER_H_
#define _HOTPLUG_HANDLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define INT_STOR			"/i-stor"	/// internal storage mount point
#define EXT_STOR 			"/e-stor"	/// external storage mount point
#define DEV_PROP_FILE		"prop.dev"	/// file containing the device size and speed

#define MSG_HEADER			"ESTOR:"
#define MSG_ADDED			"ADDED"
#define MSG_REMOVED			"REMOVED"

#define STORAGE_NONE 		0x0000		/// no internal or external USB storage device
#define ISTORAGE_PRESENT 	0x0001		/// internal storage device
#define ISTORAGE_VALID		0x0002		/// internal storage device is validated for PERSISTFS, mutually exclusive with ESTORAGE_VALID
#define ESTORAGE_ADDED 		0x0004		///	an external USB storage device was mounted by the hotplug system
#define ESTORAGE_VALID		0x0008		/// the external storage is validated for PERSISTFS, mutually exclusive with ISTORAGE_VALID
#define USB_CAPABILITY		100		//1GB


#define FIFO_NAME		"/tmp/bdp.fifo"
extern int hotplug_fd;					/// file descriptor of the FIFO receiving the messages from the hotplug system
struct s_dev_prop
{
	unsigned long long size;			/// size of the device in bytes
	unsigned int speed;					/// transfer speed of the device in bytes/sec.
};

/**
	Initializes the hotplug handler. The function checks
	whether the internal and/or external USB storage is/are
	mounted and sets PERSISTFS and the internal state accordingly.

	@return 0 - success
			-1 - failure
 */
int init_hotplug(void);

/**
	Uninitializes the hotplug handler.
 */
void uninit_hotplug(void);

/**
	Retrieves the message sent by the hotplug system and updates
	the internal state and the content of PERSISTFS if there
	is no internal storage device.
 
	@return 0 - success
			-1 - failure
 */
int handle_hotplug_event(void);

/**
	Returns the internal state. If the bit corresponding to a storage device is not set
	the device is either not present or not mounted.

	@return state of the storage device(s)
 */
unsigned int get_storage_status(void);

/**
	Changes the internal state and PERSISTFS (if set to external storage) as to indicate
	that the external storage is unavailable. The function MUST be called whenever the
	external storage is programatically unmounted but not physically removed.

	@note	The application MUST never unmount the internal storage.
 */
void set_status_estorage_invalid(void);

/**
	Updates the PERSISTFS environment variable and, if successfull,
	validates or invalidates the storage to be used as PERSISTFS.
	Validating the internal storage invalidates the external one and viceversa.

	@param storage - STORAGE_NONE: unset current PERSISTFS
					 ISTORAGE_PRESENT: set PERSISTFS to internal storage
					 ESTORAGE_ADDED:  set PERSISTFS to external storage
 */
int update_persistfs_env(int storage);

/**
	Retrieves the properties of a storage device mounted under mpoint.
	The function will fail if it is called for a storage device which
	is not mounted.

   	@param mpoint - mount point
   	@param dprop - pointer to property structure
   	@return 0 - success
			-1 - failure	
 */
int get_dev_prop(const char *mpoint, struct s_dev_prop *dprop);

int get_EStorage_capability(void);
int get_EStorage_memory_space(void);
int check_Ext_mount_point(void); //return -1 if not exist

#ifdef __cplusplus
}
#endif

#endif	// _HOTPLUG_HANDLER_H_
