/******************************************************************************
*
* File name		: zenotimer.h
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created Date	: 27-May-2008
* Autho	r		: y.vo@kenvi.com
* Purpose		: 
* Note			: 
*
******************************************************************************/

/**
	$Log: zenotimer.h,v $
	Revision 1.1.1.1  2013/02/20 09:35:12  liempham
	Imported sources

	Revision 1.2  2013/01/24 01:02:25  hdo
	BugId: 38635
	Description:
	 - Import dtv apis into sigmaobjectextension
	SEND_COMMENT_BUGZILLA

	Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
	add new
	
	Revision 1.1  2011/04/25 04:35:05  liempham

	BugId: 31699 add sc_sdk module
	SEND_COMMENT_BUGZILLA

	Revision 1.1  2010/12/14 09:38:18  vietmai
	insert zenotime
	
	Revision 1.1.1.1  2010/06/09 02:21:07  cvsadmin
	add new 
	
*/

#ifndef __ZENO_CTIMER_H__
#define __ZENO_CTIMER_H__

#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <fcntl.h>

#define ALLOW_OS_CODE 1
#include "rmdef/rmdef.h"

#define MAX_TIME_ZONE	74
#define ADD				0
#define MINUS			1
#define MAXBUFSIZE 1024
#ifndef MAXINT
#define MAXINT 0x7fffffff
#endif

typedef struct st_part_info
{
	int tid; 
	int socket;
	int control_socket;
	RMuint64 start,stop,done;
	char fname[256];
	int fd;
}part_info_t;


//////////////////////////////////////////////////////////////////////////
// Define structures, enums
//////////////////////////////////////////////////////////////////////////

struct SYearMontDayWeek
{
	RMuint16 day;				// [1, 31]
	RMuint16 month;				// [1, 12]
	RMuint16 year;				// [1900, ..]
	RMuint8  week_day;			// [1, 7] = [SUN, SAT]
};

struct SHourMinuteSecond
{
	RMuint8 hour;				// [0, 24)
	RMuint8 minute;				// [0, 60)
	RMuint8 second;				// [0, 60)
};

struct SDateTime
{
	struct SYearMontDayWeek ymdw;
	struct SHourMinuteSecond hms;
};

typedef struct 
{	
	
	RMuint8		hour;	
	RMuint8		minute;
	RMuint8		status;		// ADD or MINUS
	RMuint8     daylight;	// Automatically adjust clock for daylight saving changes. 
							// Base on Time Zone on PC (Not use now)
	RMascii		acSTimeZoneName[128];
} STimeZone;

//Start set Auto time
typedef struct st_mtda_dload_info
{
	char *url;
	char *ftp_user;
	char *ftp_pass;
	char *proxy_ip;
	short int proxy_port;
	char *proxy_user;
	char *proxy_pass;
	int total_threads;
	char *out_file;
}mtda_dload_info;

typedef struct st_dload_info
{
	char url_data[MAXBUFSIZE];
	char *url; 
	char server_ip[16];
	short int server_port;
 	char *protocol;
	char *server;
	char *data;
	char *path;
	char *file;
	char *ftp_user;
	char *ftp_pass;
	char *proxy_ip; 
	short int proxy_port;
	char *proxy_user;
	char *proxy_pass;
	int total_threads;
	RMuint64 file_length;
}dload_info_t;

typedef struct
{
	int tid;
	dload_info_t *dload_info;
	part_info_t *part;
}thread_args_t;

enum mtda_errors
{
	MTDA_SUCCESS=1,
		MTDA_ERROR=-32768,
		MTDA_INVALID_URL,
		MTDA_CONNECT,
		MTDA_SOCKET, 
		MTDA_FTP
};

#ifdef URLDBG 
#define URLDBGMSG(x) printf("[%s:%d]", __FILE__, __LINE__); printf(x);
#else
#define URLDBGMSG(x)
#endif

#define POST_CHAR 1
#define MTDA_CONNECTION_TIMEOUT 5 /* seconds */

static inline unsigned upcase(unsigned a)
{
	if (a>='a' && a<='z') a -= 0x20;
	return a;
}
static inline int casecmp(unsigned char *c1, unsigned char *c2, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++) if (upcase(c1[i]) != upcase(c2[i])) return 1;
	return 0;
}

static inline int end_of_dir(unsigned char *url, unsigned char c)
{
	return c == POST_CHAR || c == '#' || ((c == ';' || c == '?') && (!url || !casecmp(url, (unsigned char *)"http://", 7)));
}
static inline int dir_sep(char x) { return x == '/'; }

int strcasecmp(const char *s1, const char *s2);
int check_protocol(unsigned char *, int);
void dump_url(dload_info_t *__dload_info);
int wget_load_url(char* url, char* path);
int http_func(char* url, char* path);
int ftp_func(char* url, char* path);
int mtda_get_header(mtda_dload_info __dload_info, char *buf_hdr);
int spliturl(dload_info_t *dload_info);
int link_parse_url(unsigned char *url, int *prlen, unsigned char **user, int *uslen, unsigned char **pass, int *palen, unsigned char **host, int *holen, unsigned char **port, int *polen, unsigned char **data, int *dalen, unsigned char **post);
unsigned char *get_protocol_name(unsigned char *url, unsigned char *prot_name, int strsize);
unsigned char *get_pass(unsigned char *url, unsigned char *pass, int strsize);
unsigned char *get_host_name(unsigned char *url, unsigned char *host_name, int strsize);
unsigned char *get_url_data(unsigned char *url);
int get_port(unsigned char *url);
int get_prot_info(unsigned char *prot, int *port, int (**func)(char* url, char* path), void (**nc_func)(void*), int *allow_post);
unsigned char *get_url_path(unsigned char *url, unsigned char *path, int strsize);
int parse_http_header(unsigned char *head, unsigned char *item,
					  unsigned char **ptr, int *len);
time_t parse_date(unsigned char **date_pos, unsigned char *end,
				  int update_pos, int skip_week_day);
int parse_day(const unsigned char **date_p, unsigned char *end);
int parse_month(const unsigned char **buf, unsigned char *end);
int parse_year(const unsigned char **date_p, unsigned char *end);
int parse_time(const unsigned char **time, struct tm *tm, unsigned char *end);
int month2num(const unsigned char *str);
int get_file_info(dload_info_t *dload_info, char *buf_hdr, int bufsize);
int get_file_info_http(dload_info_t *dload_info, char *buf_hdr, int bufsize);	
int get_file_info_ftp(dload_info_t *dload_info, char *buf_hdr);
int get_connection(int tid,dload_info_t dload_info,int logon);
int get_connection_http_proxy(int tid, dload_info_t dload_info);
int connect_timeout(int tid, int socket, struct sockaddr *server, int size, int timeout_in_sec);
void base64_encode(const char *s, char *store, int length);
int receive_data(int socket, char *buf, int bufsize, int flags);
int get_ftp_response(int tid, int remotesocket,char *expected_reply);
int get_ftp_response(int tid, int remotesocket,char *expected_reply);
int read_line(int socket, char *buf, int bufsize);
int read_char(int socket, char *c);

//end auto time

//////////////////////////////////////////////////////////////////////////
// Define class
//////////////////////////////////////////////////////////////////////////
class CTimer
{
//variables
private:
    pthread_mutex_t m_timerMutex;
public:
    
//Methods
private:
    void Init();
    void Free();
	int Lock();
    int UnLock();

public:	
	RMuint8		uiTimeZIdx;

public:
    CTimer();
	CTimer(CTimer *pCTimer);
    ~CTimer();
	
	//Begin
	RMint16		ChangeDateTime(struct SDateTime dt1, struct SHourMinuteSecond inc, struct SDateTime *dt2);
	RMint16		CompareDateTime(struct SDateTime dt1, struct SDateTime dt2);

	RMuint8		GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth);
	RMuint32	GetNumSecondOfYear(RMuint16 uiYear);
	RMuint32	GetNumSecondOfMonth(RMuint16 uiYear, RMuint16 uiMonth);
	//End

	RMint32		GetSystemTime(struct SDateTime* pDt);
	RMstatus	GetSystemTime(RMuint64 uiClockId, struct SDateTime* pstUTC);
	//RMstatus	CreateClockFromDT(struct SDateTime* pDt, RMuint64* puiClockId);
	RMstatus	CreateClockFromDT(struct SDateTime* pDt, RMuint64* puiClockId, RMuint16 uiPosTimeZone);
	RMint32		SetSystemTime(struct SDateTime dt);
	void		ShowDateTime(struct SDateTime Dt);
	RMint32		GetHttpUrlFromPlaylistUrl(const RMascii *playlistUrl, RMascii *httpUrl);
	RMstatus	GetTimeZoneInfor(RMuint8 idx, STimeZone	*stTzone);
	

	RMint16		ChangeDayTimeZone(struct SDateTime dt1, STimeZone uiTzone, RMuint32 daylight, struct SDateTime *dt2);
	RMint16		AddTimeZone(struct SDateTime dt1, RMint16 iPosStartTimeZone, RMint16 iPosEndTimeZone, struct SDateTime *dt2);
	RMint32		GetDateFromSecondEx(RMuint32 uiSecond, STimeZone uiTzone, RMuint32 daylight, struct SDateTime* pDt);
//Auto Time
	int			SyncTime(const char *playlistUrl,RMint8 m_TimeZone );
	RMint64		GetDateFromSecond(RMuint32 uiSecond, struct SDateTime* pDt);
	RMuint64	GetSecondFromDate(RMuint64 &uiSecond, struct SDateTime sDt);
};

#endif //__ZENO_CTIMER_H__
/* end */
