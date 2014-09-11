/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/	
#ifndef NO_BLUE_SUPPORT
struct fs_stat
{
	long fs_total;			/// total data blocks in file system, 1K units
	long fs_used;			/// used blocks, 1K units
	long fs_avail;			/// free blocks available to non-superuser,  1K units
	long d_used;			/// used blocks in ADA / BUDA directory, 1K units
};
struct fs_stat ada_stat = {0, };
struct fs_stat buda_stat = {0, };
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			Kscale(long b, long bs)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *				long b, long bs
 * @return
 *			long
*/
static long Kscale(long b, long bs)
{
	return ( b * (long long) bs + 1024/2 ) / 1024;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			check_persistfs_usage(const char *ada, struct fs_stat *ada_stats, 
								const char *buda, struct fs_stat *buda_stats)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *				const char *ada, struct fs_stat *ada_stats, 
 *				const char *buda, struct fs_stat *buda_stats)
 * @return
 *			int
*/
//	reports the total, used and available space in ADA and / or BUDA directories
static int check_persistfs_usage(const char *ada, struct fs_stat *ada_stats, const char *buda, struct fs_stat *buda_stats)
{
	struct statfs stats;
	FILE *fp;
	char buf[128];
	
	if (ada && ada_stats)
	{
		memset(ada_stats, 0, sizeof(struct fs_stat));
		
		if (!ada[0] || (statfs(ada, &stats) < 0))
		{
			D_ERROR("Could not stat %s\n", ada);
		}
		else
		{
			ada_stats->fs_total = Kscale(stats.f_blocks, stats.f_bsize);
			ada_stats->fs_used = Kscale((stats.f_blocks - stats.f_bfree), stats.f_bsize);
			ada_stats->fs_avail = Kscale(stats.f_bavail, stats.f_bsize);
			
			snprintf(buf, sizeof(buf), "du -s %s", ada);
			
			fp = popen(buf, "r");
			
			if (fp == NULL)
			{
				D_ERROR("popen failed\n");
			}
			else
			{
				fread(buf, sizeof(char), 128, fp);
				sscanf(buf, "%ld", &ada_stats->d_used);
				pclose(fp);
			}
		}
	}
	else
	{
		D_ERROR("One of the ADA parameters is invalid\n");
		return -1;
	}
	
	if (buda && buda_stats)
	{
		memset(buda_stats, 0, sizeof(struct fs_stat));
		
		if (!buda[0] || (statfs(buda, &stats) < 0))
		{
			D_ERROR("Could not stat %s\n", buda);
		}
		else
		{
			buda_stats->fs_total = Kscale(stats.f_blocks, stats.f_bsize);
			buda_stats->fs_used = Kscale((stats.f_blocks - stats.f_bfree), stats.f_bsize);
			buda_stats->fs_avail = Kscale(stats.f_bavail, stats.f_bsize);
			
			snprintf(buf, sizeof(buf), "du -s %s", buda);
			
			fp = popen(buf, "r");
			
			if (fp == NULL)
			{
				D_ERROR("popen failed\n");
			}
			else
			{
				fread(buf, sizeof(char), 128, fp);
				sscanf(buf, "%ld", &buda_stats->d_used);
				pclose(fp);
			}
		}
	}
	else
	{
		D_ERROR("One of the BUDA parameters is invalid\n");
		return -1;
	}
	
	return 0;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			handle_persistfs(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *				void
 * @return
 *				void
*/
void handle_persistfs(void)
{
#ifndef NO_BLUE_SUPPORT
	// gather ADA/BUDA statistics when the info page becomes visible 
	char ada[256] = "", buda[256] = "";
	char *persistfsName = getenv("PERSISTFS");
	char *appstorefsName = getenv("APPSTOREFS");
	
	if( !persistfsName || !appstorefsName )
	{
		return;
	}
	if (appstorefsName)
	{
		strcpy(ada, appstorefsName);		
		strcat(ada, "/bdj/ada");
	}
	if (persistfsName)
	{
		strcpy(buda, persistfsName);
		
		strcat(buda, "/bdj/buda");
	}
	if (check_persistfs_usage(ada, &ada_stat, buda, &buda_stat) < 0)
	{
		GUI_INFO("--------------- getenv(PERSISTFS) ----------------------\n");
		ada_stat.fs_total = buda_stat.fs_total = 0;
		
	}
	
#endif
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			install_storage(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *				void
 * @return
 *				int
*/
int install_storage(void)
{
	struct SMediaSession *media = NULL;
	
	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];
	
	if(media && media->amp)
	{
		struct SLPBCommand cmd;
		struct SResult res;
		DFBResult err;
		
		cmd.common.dataSize = sizeof(cmd);
		cmd.common.mediaSpace = MEDIA_SPACE_UNKNOWN;
		cmd.cmd = (enum ELPBCmd)Cmd_storage_installed;
		
		res.dataSize = sizeof(res);
		res.mediaSpace = MEDIA_SPACE_UNKNOWN;
		
		err = media->amp->ExecutePresentationCmd(media->amp, (struct SCommand *)&cmd, &res);
		
		if ((err != DFB_OK) || IS_ERROR(res.value))
		{
			printf("\n\033[34mCouldnot install storage\033[0m>>>>\n");
			return -1;
		}
		else
		{
			printf("\n\033[34mStorage installed\033[0m>>>>\n");
			return 0;
		}
		
	}
	return -1;
}
/**
 * 
 * <b>FUNCTION: </b>
 *
 *			uninstall_storage(void)
 *
 * <b>DESCRIPTION: </b>
 * 
 *
 * 
 * @param
 *				void
 * @return
 *				int
*/
int uninstall_storage(void)
{
	struct SMediaSession *media = NULL;
	//Phathv use 2 amps
	if (ampIndex < MAX_AMP_INSTANCES)
		media = &mediaTab[ampIndex];
	
	if(media && media->amp)
	{
		struct SLPBCommand cmd;
		struct SResult res;
		DFBResult err;
		
		// simulate a storage removal in order to restart de AMP if VP is active
		cmd.common.dataSize = sizeof(cmd);
		cmd.common.mediaSpace = MEDIA_SPACE_UNKNOWN;
		cmd.cmd = (enum ELPBCmd)Cmd_storage_removed;
		
		res.dataSize = sizeof(res);
		res.mediaSpace = MEDIA_SPACE_UNKNOWN;
		
		err = media->amp->ExecutePresentationCmd(media->amp, (struct SCommand *)&cmd, &res);
		if ((err == DFB_OK) && (res.value == TRESULT_SUCCESS))
		{
			// AMP has signaled that Virtual Package is not active
			// switch back automatically to the internal storage if it exists
			printf("\n\033[34mAMP has signaled that Virtual Package is not active\033[0m>>>>\n");
			return 0;
		}
		else if (TRESULT_VALUE(res.value) == RESULT_IO)
		{
			printf("\n\033[34mAMP has signaled that Virtual Package is active\033[0m>>>>\n");
			return 1;
		}
		else
		{
			printf("\nUnknown result\n");
			return -1;
		}
	}
	return -1;
}
#endif // NO_BLUE_SUPPORT
