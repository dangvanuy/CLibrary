/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
  @file   playlist.h
  @brief  Definitions for Playlist

  @author Dat Nguyen
  @date   2006-08-21

  $Log: mediaplaylist.h,v $
  Revision 1.1.1.1  2013/02/20 09:35:11  liempham
  Imported sources

  Revision 1.2  2013/01/24 01:02:24  hdo
  BugId: 38635
  Description:
   - Import dtv apis into sigmaobjectextension
  SEND_COMMENT_BUGZILLA

  Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
  add new

  Revision 1.1  2011/04/25 04:35:04  liempham

  BugId: 31699 add sc_sdk module
  SEND_COMMENT_BUGZILLA

  Revision 1.1.1.1  2010/07/19 02:47:56  cvsadmin
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
  Revision 1.2  2006/11/07 01:56:14  sorinp
  Added playlist support for optical media

  Revision 1.1  2006/08/21 22:23:46  dnguyen
  added playlist support for LPB, to test specify -f:lpb and url to a directory
  containing media files


*/

#ifndef MEDIAPLAYLIST_H_
#define MEDIAPLAYLIST_H_

typedef void *playlist_t;

playlist_t OpenPlaylist(const char *url, int dirsOK);
void ClosePlaylist(playlist_t pl);

int GetNextPlaylistItem(playlist_t pl, char *url, int maxlen);

#endif /*PLAYLIST_H_*/
