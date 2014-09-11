/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
  @file   playst.c
  @brief  Implementation of Playlist

  @author Dat Nguyen
  @date   2006-08-21

  $Log: mediaplaylist.c,v $
  Revision 1.1.1.1  2013/02/20 09:35:10  liempham
  Imported sources

  Revision 1.2  2013/01/24 01:02:24  hdo
  BugId: 38635
  Description:
   - Import dtv apis into sigmaobjectextension
  SEND_COMMENT_BUGZILLA

  Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
  add new

  Revision 1.2  2011/08/26 06:47:32  liempham

  BugId: 31699

   - replace function sprintf(...) by function snprintf(...)

  SEND_COMMENT_BUGZILLA

  Revision 1.1  2011/04/25 04:35:04  liempham

  BugId: 31699 add sc_sdk module
  SEND_COMMENT_BUGZILLA

  Revision 1.1.1.1  2010/07/19 02:47:55  cvsadmin
  add new

  Revision 1.2  2008/06/17 04:58:07  pvu
  BugId: none
  clean-up.

  Revision 1.1.1.1  2008/06/09 11:51:54  pvu
  Initial check-in 6/9/2008.


 * 
 * 1     3/28/08 10:56a Trihuynh
 * 
 * 1     3/28/08 9:48a Trihuynh
  Revision 1.4  2006/11/07 01:56:14  sorinp
  Added playlist support for optical media

  Revision 1.3  2006/09/12 22:08:33  sorinp
  Fixes for file playlist case

  Revision 1.2  2006/09/12 02:24:31  sorinp
  Added support for BD-J video drips
  Added support for independent lifecycle for each AMP

  Revision 1.1.6.1  2006/09/11 22:26:20  sorinp
  playlist and other changes

  Revision 1.1  2006/08/21 22:23:46  dnguyen
  added playlist support for LPB, to test specify -f:lpb and url to a directory
  containing media files


*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "include/mediaplaylist.h"

enum EPlaylistType
{
	LIST_DIR,	// a list containing all the files in a specified directory
	LIST_FILE	// a list stored in the specified file (not implemented yet)
};

struct SPlaylist
{
	enum EPlaylistType type;
	char url[1024];

	union // context
	{
		DIR* dir;
		FILE* listFile;
	};
};

playlist_t OpenPlaylist(const char *url, int dirsOK)
{
	struct SPlaylist *p = (struct SPlaylist *) calloc(1, sizeof(struct SPlaylist));
	const char plext[] = ".playlist";
	
	if (p != NULL)
	{
		// *.playlist are file playlists
		if ((strlen(url) > strlen(plext)) &&
			!strcasecmp(&url[strlen(url)-strlen(plext)], plext))
		{
			strncpy(p->url, url, sizeof(p->url) / sizeof(p->url[0]));
			goto _file_list;			
		}
			
		// check first for a directory playlist
		if (dirsOK && ((p->dir = opendir(url)) != NULL))
		{
			p->type = LIST_DIR;
			strncpy(p->url, url, sizeof(p->url) / sizeof(p->url[0]));
		}
		else
		{
			// it must be a file playlist
			snprintf(p->url, sizeof(p->url), "%s%s", url, plext);
			
	_file_list:
			if((p->listFile = fopen(p->url, "r")))
			{
				p->type = LIST_FILE;
			}
			else
			{
				free (p);
				return NULL;
			}
		}
	}

	return p;
}

void ClosePlaylist(playlist_t pl)
{
	if (pl)
	{
		struct SPlaylist *p = (struct SPlaylist *) pl;

		switch (p->type)
		{
			case LIST_DIR:
				closedir(p->dir);
				break;

			case LIST_FILE:
				fclose(p->listFile);
				break;

			default:
				break;
		}

		free (p);
	}
}

int GetNextPlaylistItem(playlist_t pl, char *url, int maxlen)
{
	int n = 0;

	if (pl)
	{
		struct SPlaylist *p = (struct SPlaylist *) pl;

		switch (p->type)
		{
			case LIST_DIR:
			{
				struct dirent *ent;
				struct stat statbuf;

				while ((ent = readdir(p->dir)) != NULL)
				{
					n = snprintf(url, maxlen, "%s/%s", p->url, ent->d_name);

					if (stat(url, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
						break;

					n = 0;
				}

				break;
			}

			case LIST_FILE:
				*url = 0;
				do
				{
					if (!fgets(url, maxlen, p->listFile))
						break;

					n = strlen(url);
					while ((n > 0) && (url[n-1] == '\n'))
					{
						n--;
						url[n] = 0;
					}
				}
				while (n == 0);
				
				break;

			default:
				break;
		}
	}

	return n;
}
