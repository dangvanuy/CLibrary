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

 	$Header: /kenvi/cvsroot/TAC_Box/sigmaobjectsextension/mediasetting/mediasettinglib/src/hotplug_handler.c,v 1.1.1.1 2013/02/20 09:35:11 liempham Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <mntent.h>
#include <limits.h>
#include <assert.h>
#include <directfb.h>
#include <direct/messages.h>
#include "include/hotplug_handler.h"

static int open_fifo(void);
static void close_fifo(void);
static int check_mnt_point(const char *mpoint);
static int read_dev_properties(const char *storage_mnt, struct s_dev_prop *prop);

int hotplug_fd = -1;								/* file descriptor of the named FIFO for receiving hotplug events */
static struct s_dev_prop prop_i_stor;				/* properties of the internal storage device */
static struct s_dev_prop prop_e_stor;				/* properties of the external storage device */
static unsigned int s_status = STORAGE_NONE;		/* status of the storage devices */

int init_hotplug()
{
	printf("\n\033[34m<<<<init_hotplug\033[0m\n");
	/* check if the internal, i.e. non-removable storage was mounted */
	if ((check_mnt_point(INT_STOR) == 0) && (read_dev_properties(INT_STOR, &prop_i_stor) == 0))
		s_status |= ISTORAGE_PRESENT;

	/* check if an external, i.e. removable storage was already mounted */
	if ((check_mnt_point(EXT_STOR) == 0) && (read_dev_properties(EXT_STOR, &prop_e_stor) == 0))
		s_status |= ESTORAGE_ADDED;

	if (s_status == STORAGE_NONE)
		D_WARN("init_hotplug: No internal OR external storage mounted.\n");
	
	if (open_fifo() == -1)
	{
		D_WARN("init_hotplug: open_fifo() failed. Hotplug messages will not be received.\n");
		return -1;
	}

	D_INFO("Hotplug handler initialized.\n");

	return 0;
}

void uninit_hotplug()
{
	printf("\n\033[34m<<<<uninit_hotplug\033[0m\n");
	close_fifo();
	s_status = STORAGE_NONE;
}

unsigned int get_storage_status()
{
	printf("\n\033[34m<<<<get_storage_status\033[0m\n");
	return s_status;
}

void set_status_estorage_invalid()
{
	printf("\n\033[34m<<<<set_status_estorage_invalid\033[0m\n");
	char *penv;
	
	penv = getenv("PERSISTFS");
	if (penv == NULL)
		D_WARN("set_status_estorage_invalid: PERSISTFS is not set.");
	else if (strcmp(penv, EXT_STOR) == 0)
	{
		unsetenv("PERSISTFS");
		unsetenv("PERSISTFSSIZE");
		unsetenv("PERSISTFSSPEED");
	}
	
	s_status &= ~ESTORAGE_VALID;
}

int update_persistfs_env(int storage)
{
	printf("\n\033[34m<<<<update_persistfs_env\033[0m\n");
	char sz[32], sp[32];
	char cmd1[256], cmd2[256];
	//memset(cmd, 0, 256);
	memset(cmd1, 0, 256);
	memset(cmd2, 0, 256);
	if (storage == ESTORAGE_ADDED)
	{
		snprintf(sz, 32, "%llu", prop_e_stor.size);
		snprintf(sp, 32, "%u", prop_e_stor.speed);
		
		if ((setenv("PERSISTFS", EXT_STOR, 1) == 0) &&
		    (setenv("PERSISTFSSIZE", sz, 1) == 0) &&
			(setenv("PERSISTFSSPEED", sp, 1) == 0))
		{
			s_status &= ~ISTORAGE_VALID;		// invalidate the internal storage
			s_status |= ESTORAGE_VALID;
			//create /bdj/ada and /bdj/buda in EXT_STOR
			//snprintf(cmd, sizeof(cmd), "rm -rf %s/bdj/", EXT_STOR);
			snprintf(cmd1, sizeof(cmd1), "mkdir -p %s/bdj/ada", EXT_STOR);
			snprintf(cmd2, sizeof(cmd2), "mkdir -p %s/bdj/buda", EXT_STOR);
			//system(cmd);
			system(cmd1);
			system(cmd2);
		}
		else
			D_WARN("update_persistfs_env: Failed to set PERSISTFS environment for external storage.\n");
	}
	else if (storage == ISTORAGE_PRESENT)
	{
		snprintf(sz, 32, "%llu", prop_i_stor.size);
		snprintf(sp, 32, "%u", prop_i_stor.speed);
		
		if ((setenv("PERSISTFS", INT_STOR, 1) == 0) &&
		    (setenv("PERSISTFSSIZE", sz, 1) == 0) &&
			(setenv("PERSISTFSSPEED", sp, 1) == 0))
		{
			s_status &= ~ESTORAGE_VALID;		//invalidate the external storage
			s_status |= ISTORAGE_VALID;
		}
		else
			D_WARN("update_persistfs_env: Failed to set PERSISTFS environment for internal storage.\n");
	}
	else if (storage == STORAGE_NONE)
	{
		char *penv = getenv("PERSISTFS");

		if (penv == NULL)
			D_WARN("Unsetting non-exitent PERSISTFS variable\n");
		else if (strncmp(penv, INT_STOR, strlen(INT_STOR)))
			s_status &= ~ISTORAGE_VALID;
		else if (strncmp(penv, EXT_STOR, strlen(EXT_STOR)))
			s_status &= ~ESTORAGE_VALID;
		else
			D_WARN("update_persistfs_env: Invalid PERSISTFS value!\n");
		
		unsetenv("PERSISTFS");
		unsetenv("PERSISTFSSIZE");
		unsetenv("PERSISTFSSPEED");
	}
	else
	{
		D_ERROR("update_persistfs_env: Invalid parameter\n");
		return -1;
	}

	assert(!((s_status & ESTORAGE_VALID) && (s_status & ISTORAGE_VALID)));

	D_INFO("update_persistfs_env: PERSISTFS: %s\n", getenv("PERSISTFS") ? getenv("PERSISTFS") : "NONE");
	
	return 0;
}

int get_dev_prop(const char *mpoint, struct s_dev_prop *dprop)
{
	printf("\n\033[34m<<<<get_dev_prop\033[0m\n");
	if (!mpoint || !dprop)
	{
		D_ERROR("get_dev_prop: Invalid input parameters\n");
		return -1;
	}

	if (!strncmp(mpoint, INT_STOR, strlen(INT_STOR)) && (s_status & ISTORAGE_PRESENT))
		*dprop = prop_i_stor;
	else if (!strncmp(mpoint, EXT_STOR, strlen(EXT_STOR)) && (s_status & ESTORAGE_ADDED))
		*dprop = prop_e_stor;
	else
	{
		D_ERROR("get_dev_prop: Invalid mount point (%s) or storage not present [0x%x]\n", mpoint, s_status);
		return -1;
	}
	
	return 0;
}

int get_EStorage_capability()
{
	printf("\n\033[34m<<<<get_EStorage_capability:\033[0m");
	if(s_status & ESTORAGE_ADDED) //external storage added already
	{
		printf("\033[31m%llu = %dMB\033[0m\n", prop_e_stor.size, (int)(prop_e_stor.size/1000000));
		return (int)(prop_e_stor.size/1000000);
	}
	printf("\n\033[34m<<<< Cannot get_EStorage_capability\033[0m\n");
	return -1;
}

int get_EStorage_memory_space(void)
{
	if(s_status & ESTORAGE_ADDED)
	{
		if(system("du -ms /e-stor > /tmp/stor_log") == -1)
			return -1;
		FILE *f;
		f = fopen("/tmp/stor_log", "rt");
		if(f == NULL)
			return -1;
		char buf[10];
		fscanf(f, "%s", buf);
		fclose(f);
		system("rm -f /tmp/stor_log");
		//printf("buf = %s\n", buf);
		printf("disk space used = %dMB\n", atoi(buf));
		printf("\033[31mMemory space left:%dMB\033[0m\n", get_EStorage_capability()-atoi(buf));
		return get_EStorage_capability()-atoi(buf);
	}
	return -1;
}


int check_Ext_mount_point()
{
	return check_mnt_point(EXT_STOR);
}
int handle_hotplug_event()
{
	printf("\n\033[34m<<<<handle_hotplug_event, hotplug_fd:%d\033[0m\n", hotplug_fd);
	int ret = 0;
	char buf[PIPE_BUF];
	int nread;

	nread = read(hotplug_fd, buf, PIPE_BUF);

	if (nread == -1)
	{
		D_ERROR("handle_hotplug_event: Error reading from FIFO.\n");
		return -1;
	}
	else if (nread == 0)
	{
		D_WARN("handle_hotplug_event: No more writers?\n");
		return -1;
	}
	
	/* parse the message */
	if (strncmp(buf, MSG_HEADER, strlen(MSG_HEADER)))					/* check the header */
	{
		D_ERROR("handle_hotplug_event: Invalid message header %s\n", buf);
		ret = -1;
	}
	else if (strncmp(buf + strlen(MSG_HEADER), MSG_ADDED, strlen(MSG_ADDED)) == 0)			/* device was added & mounted */
	{
		D_INFO("handle_hotplug_event: USB storage added\n");
		if(read_dev_properties(EXT_STOR, &prop_e_stor) == 0)
			s_status |= ESTORAGE_ADDED;
		else
			D_WARN("Could not read properties of %s. Storage is disabled.", EXT_STOR);
	}
	else if (strncmp(buf + strlen(MSG_HEADER), MSG_REMOVED, strlen(MSG_REMOVED)) == 0)		/* device was removed */
	{
		D_INFO("handle_hotplug_event: USB storage removed\n");
		s_status &= ~ESTORAGE_ADDED;
	}
	else
	{
		D_ERROR("handle_hotplug_event: Invalid message: %s", buf);
		ret = -1;
	}
	printf("\n\033[34m<<<<[TR]handle_hotplug_event, hotplug_fd:%d\033[0m\n", hotplug_fd);
	printf("\n\033[34m<<<<[TR]handle_hotplug_event, hotplug_fd:%d\033[0m\n", hotplug_fd);
	return ret;
}

static int open_fifo()
{
	printf("\n\033[34m<<<<open_fifo\033[0m\n");
	if (mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR) == -1)
	{
		if (errno == EEXIST)
		{
			D_WARN("open_fifo: FIFO already exists.\n");
		}
		else
		{
			D_ERROR("open_fifo: Could not create FIFO.\n");
			return -1;
		}
	}

	hotplug_fd = open(FIFO_NAME, O_RDWR);

	if (hotplug_fd == -1)
	{
		unlink(FIFO_NAME);
		D_ERROR("open_fifo: Could not open fifo.\n");
		return -1;
	}

	return 0;
}

static void close_fifo()
{
	printf("\n\033[34m<<<<close_fifo\033[0m\n");
	if (hotplug_fd != -1)
		close(hotplug_fd);

	unlink(FIFO_NAME);
}

static int check_mnt_point(const char *mpoint)
{
	printf("\n\033[34m<<<<check_mnt_point\033[0m\n");
	FILE *fp;
	struct mntent *ment;
	
	fp = setmntent("/etc/mtab", "r");

	if (fp == NULL)
	{
		D_ERROR("check_mnt_point: Failed to open /etc/mtab\n");
		return -1;
	}

	while (!feof(fp))
	{
		ment = getmntent(fp);
		
		if (ment != NULL && !strcmp(ment->mnt_dir, mpoint))
		{
			D_INFO("ment->mnt_opts:%s\n", ment->mnt_opts);
			D_INFO("ment->mnt_opts:[%c%c]\n", ment->mnt_opts[0], ment->mnt_opts[1]);
			if((ment->mnt_opts[0] == 'r') && (ment->mnt_opts[1] == 'w')) //rw: writable
			{
				D_INFO("%s\n", ment->mnt_dir);
				return 0;
			}
			else
			{
				D_INFO("\033[31mRead-only device\033[0m\n");
				return -1;
			}
		}
	}
	
	return -1;
}

static int read_dev_properties(const char *storage_mnt, struct s_dev_prop *prop)
{
	printf("\n\033[34m<<<<read_dev_properties\033[0m\n");
	char buf[64];
	FILE *fp;
	int ret = 0;
	enum {GET_HEADER, GET_SIZE, GET_SPEED} e_state;

	if(!storage_mnt || !prop)
	{
		D_ERROR("read_dev_properties: invalid input parameters\n");
		return -1;
	}

	memset(prop, 0, sizeof(struct s_dev_prop));
	
	snprintf(buf, sizeof(buf), "%s/%s", storage_mnt, DEV_PROP_FILE);
	printf("\033[34m<<<<open file:%s\033[0m\n", buf);
	fp = fopen(buf, "r");

	if (fp == NULL)
	{
		D_ERROR("read_dev_properties: failed to open the device properties file %s.\n", buf);
		return -1;
	}

	e_state = GET_HEADER;

	while(1)
	{
		if (fgets(buf, 64, fp) == NULL)
		{
			if (feof(fp))
			{
				D_INFO("read_dev_properties: EOF\n");
				ret = 0;
			}
			else
			{
				D_ERROR("read_dev_properties: error reading from the file.\n");
				ret = -1;
			}
			break;
		}

		switch (e_state)
		{
			case GET_HEADER:
				if (strncmp(buf, "SIGM", 4))
				{
					D_ERROR("read_dev_properties: invalid file format [%s]\n", buf);
					ret = -1;
					goto _close_file;
				}
				e_state = GET_SIZE;
				break;

			case GET_SIZE:
			{
				char sz[6];
				unsigned long long size;
				
				if (sscanf(buf, "%5c%llu", sz, &size) < 2 || strncmp(sz, "SIZE=", 5))
				{
					D_ERROR("read_dev_properties: invalid size line [%s]\n", buf);
					ret = -1;
					goto _close_file;
				}
				prop->size = size;
				e_state=GET_SPEED;
				break;
			}
			case GET_SPEED:
			{
				char sp[7];
				unsigned int speed;
				
				if (sscanf(buf, "%6c%u", sp, &speed) < 2 || strncmp(sp, "SPEED=", 6))
				{
					D_ERROR("read_dev_properties: invalid speed line [%s]\n", buf);
					ret = -1;
					goto _close_file;
				}
				prop->speed = speed;
				goto _close_file;
				break;
			}
			default:
				D_ERROR("read_dev_properties: invalid file\n");
				ret = -1;
				goto _close_file;
		}
	}

_close_file:
	fclose(fp);
	return ret;
}
