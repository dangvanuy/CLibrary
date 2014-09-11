/******************************************************************************
*
* File name		: zenoctimer.cpp
* Copyright (c) 2008, Kenvi.  All rights reserved.
* Created		: Date 27-May-2008
* Author		: y.vo@kenvi.com
* Purpose		:		
* Note			:			
*
******************************************************************************/
/**
	$Log: zenotimer.cpp,v $
	Revision 1.1.1.1  2013/02/20 09:35:12  liempham
	Imported sources

	Revision 1.2  2013/01/24 01:02:25  hdo
	BugId: 38635
	Description:
	 - Import dtv apis into sigmaobjectextension
	SEND_COMMENT_BUGZILLA

	Revision 1.1.1.1  2012/12/20 01:18:14  cvsadmin
	add new
	
	Revision 1.2  2011/08/26 06:47:33  liempham

	BugId: 31699

	 - replace function sprintf(...) by function snprintf(...)

	SEND_COMMENT_BUGZILLA

	Revision 1.1  2011/04/25 04:35:05  liempham

	BugId: 31699 add sc_sdk module
	SEND_COMMENT_BUGZILLA

	Revision 1.1  2010/12/14 09:40:39  vietmai
	insert zenotime
	
	Revision 1.1.1.1  2010/06/09 02:21:07  cvsadmin
	add new 
	
*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <ctype.h> //isdigit

#include "zenotimer.h"
#include "debug.h"

int mtda_errno;
dload_info_t dload_info;

/**
* 
* <b>FUNCTION: </b>
*
* wget_load_url(char* url, char* path)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   char* url, char* path
*       
* @return
*   0
*/
int wget_load_url(char* url, char* path)
{
	char cmd[512];
	if(!url) return 1;
	
	if(path)
		snprintf(cmd, sizeof(cmd), "wget %s -O %s", url, path);
	else 
		snprintf(cmd, sizeof(cmd), "wget %s", url);
	
	//printf("WGET CMD [%s]\n", cmd);
	printf("loading...\n");
	system(cmd);
	printf("end load\n");
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* http_func(char* url, char* path)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   char* url, char* path
*       
* @return
*   0
*/
int http_func(char* url, char* path)
{
	return wget_load_url(url, path);
}

/**
* <b>FUNCTION: </b>
*
* ftp_func(char* url, char* path)
*
* <b>DESCRIPTION: </b>
* 
* @param
*   char* url, char* path
*       
* @return
*   0
*/
int ftp_func(char* url, char* path)
{
	return wget_load_url(url, path);
}

typedef struct protocol
{
	unsigned char *prot;
	int port;
	int (*func)(/*struct connection **/char* url, char* path);
	void (*nc_func)(/*struct session *, unsigned char **/void*);
	int free_syntax;
	int need_slashes;
	int need_slash_after_host;
	int allow_post;
}protocol;

struct protocol protocols[3]=
{
	{(unsigned char*)"http", 80, http_func, NULL, 0, 1, 0, 1},
	{(unsigned char*)"ftp", 21, ftp_func, NULL, 0, 1, 1, 0},
	{NULL, 0, NULL, NULL, 0, 0, 0, 0},
}; 

/////////////////////////////////////////////////////////////////////////
// For debuging
//////////////////////////////////////////////////////////////////////////
#define KV_AUTHOR "-=*YVo*=- "
#define console_output1(str, args...)   fprintf(stderr, str, ##args)  // debug output
#define console_debug(args...) \
	printf("\n"); \
	printf("***************************************\n"); \
	printf("%s [%s],[%s]: [%d] \n", KV_AUTHOR, __FILE__, __FUNCTION__, __LINE__); \
	printf(".......................................\n"); \
	fprintf(stderr, ##args); \
printf("***************************************\n")
#define console_err console_debug
#define console_output	console_debug
///////////////////////////////////////////////////////////////////////////
// End debuging
///////////////////////////////////////////////////////////////////////////

STimeZone TimeZoneTable[MAX_TIME_ZONE]=
{
/* 0*/	{ 12, 0, MINUS, 0, "(GMT-12:00)Eniwetok, Kwajalein" },			 /* (GMT-12:00) Eniwetok, Kwajalein */
/* 1*/    { 11, 0, MINUS, 0, "(GMT-11:00)Midway Island, Samoa"},		 /* (GMT-11:00) Midway Island, Samoa */
/* 2*/    { 10, 0, MINUS, 0, "(GMT-10:00)Hawaii"},	/* (GMT-10:00) Hawaii */
/* 3*/    {  9, 0, MINUS, 1, "(GMT-09:00)Alaska"},/*	(GMT-09:00) Alaska */
/* 4*/    {  8, 0, MINUS, 1, "(GMT-08:00)Pacific Time (US & Canada); Tijuana"},/*	(GMT-08:00) Pacific Time (US & Canada); Tijuana */
/* 5*/    {  7, 0, MINUS, 1, "(GMT-07:00)Mountain Time (US & Canada)"},/*	(GMT-07:00) Mountain Time (US & Canada)*/
/* 6*/    {  7, 0, MINUS, 0, "(GMT-07:00)Arizona"},   /*(GMT-07:00) Arizona	*/
/* 7*/    {  6, 0, MINUS, 1, "(GMT-06:00)Mexico City"},/*	(GMT-06:00) Mexico City*/
/* 8*/    {  6, 0, MINUS, 0, "(GMT-06:00)Saskatchewan"},/* (GMT-06:00) Saskatchewan	*/
/* 9*/    {  6, 0, MINUS, 0, "(GMT-06:00)Central America"},/*	(GMT-06:00) Central America*/ 
/*10*/    {  6, 0, MINUS, 1, "(GMT-06:00)Central Time (US & Canada)"},/* (GMT-06:00) Central Time (US & Canada)	*/
/*11*/    {  5, 0, MINUS, 1, "(GMT-05:00)Eastern Time (US & Canada)"},/*	(GMT-05:00) Eastern Time (US & Canada)*/
/*12*/    {  5, 0, MINUS, 0, "(GMT-05:00)Bogota, Lima, Quito"},/*	(GMT-05:00) Bogota, Lima, Quito*/ 
/*13*/    {  5, 0, MINUS, 0, "(GMT-05:00)Indiana (East)"},/* (GMT-05:00) Indiana (East)	*/
/*14*/    {  4, 0, MINUS, 0, "(GMT-04:00)Caracas, La Paz"},/*	(GMT-04:00) Caracas, La Paz*/
/*15*/    {  4, 0, MINUS, 1, "(GMT-04:00)Santiago"},/* (GMT-04:00) Santiago	*/
/*16*/    {  4, 0, MINUS, 1, "(GMT-04:00)Atlantic Time (Canada)"},/*(GMT-04:00) Atlantic Time (Canada)*/
/*17*/    {  3,30, MINUS, 1, "(GMT-03:30)Newfoundland"},/* (GMT-03:30) Newfoundland */
/*18*/    {  3, 0, MINUS, 0, "(GMT-03:00)Buenos Aires, Georgetown"},/* (GMT-03:00) Buenos Aires, Georgetown*/
/*19*/    {  3, 0, MINUS, 1, "(GMT-03:00)Brasilia"},/* (GMT-03:00) Brasilia*/
/*20*/    {  3, 0, MINUS, 1, "(GMT-03:00)Greenland"},/* (GMT-03:00) Greenland*/
/*21*/    {  2, 0, MINUS, 1, "(GMT-02:00)Mid-Atlantic"},/* (GMT-02:00) Mid-Atlantic*/
/*22*/    {  1, 0, MINUS, 1, "(GMT-01:00)Azores"},/* (GMT-01:00) Azores */
/*23*/    {  1, 0, MINUS, 0, "(GMT-01:00)Cape Verde Is."},/* (GMT-01:00) Cape Verde Is.*/
/*24*/    {  0, 0, MINUS, 0, "(GMT)Casablanca, Monrovia"},/* (GMT) Casablanca, Monrovia */
/*25*/    {  0, 0, MINUS, 1, "(GMT)Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London"},/* (GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London*/
/*26*/    {  1, 0, ADD, 1, "(GMT+01:00)Belgrade, Bratislava, Budapest, Ljubljana, Prague"},/* (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague */
/*27*/    {  1, 0, ADD, 1, "(GMT+01:00)Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb"},/* (GMT+01:00) Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb*/
/*28*/    {  1, 0, ADD, 1, "(GMT+01:00)Brussels, Copenhagen, Madrid, Paris"},/*	(GMT+01:00) Brussels, Copenhagen, Madrid, Paris	*/
/*29*/    {  1, 0, ADD, 0, "(GMT+01:00)West Central Africa"}, /*	(GMT+01:00) West Central Africa*/
/*30*/    {  1, 0, ADD, 1, "(GMT+01:00)Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna"},/* (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna*/
/*31*/    {  2, 0, ADD, 0, "(GMT+02:00)Harare, Pretoria"}, /* (GMT+02:00) Harare, Pretoria	*/
/*32*/    {  2, 0, ADD, 0, "(GMT+02:00)Jerusalem"}, /* (GMT+02:00) Jerusalem */
/*33*/    {  2, 0, ADD, 1, "(GMT+02:00)Athens, Istanbul, Minsk"},/*	(GMT+02:00) Athens, Istanbul, Minsk	*/
/*34*/    {  2, 0, ADD, 1, "(GMT+02:00)Helsinki, Riga, Tallinn"},/*	(GMT+02:00) Helsinki, Riga, Tallinn	*/
/*35*/    {  2, 0, ADD, 1, "(GMT+02:00)Bucharest"}, /* (GMT+02:00) Bucharest */
/*36*/    {  2, 0, ADD, 1, "(GMT+02:00)Cairo"},  /*(GMT+02:00) Cairo */
/*37*/    {  3, 0, ADD, 0, "(GMT+03:00)Nairobi"}, /* (GMT+03:00) Nairobi*/
/*38*/    {  3, 0, ADD, 0, "(GMT+03:00)Kuwait, Riyadh"},/* (GMT+03:00) Kuwait, Riyadh*/
/*39*/    {  3, 0, ADD, 1, "(GMT+03:00)Baghdad"}, /*GMT+03:00) Baghdad	 */
/*40*/    {  3, 0, ADD, 1, "(GMT+03:00)Moscow, St. Petersburg, Volgograd"},/* (GMT+03:00) Moscow, St. Petersburg, Volgograd*/
/*41*/    {  3, 30, ADD, 1, "(GMT+03:30)Tehran"},/* (GMT+03:30) Tehran */
/*42*/    {  4, 0, ADD, 0, "(GMT+04:00)Abu Dhabi, Muscat"}, /* (GMT+04:00) Abu Dhabi, Muscat*/
/*43*/    {  4, 0, ADD, 1, "(GMT+04:00)Baku, Tbilisi, Yerevan"}, /* (GMT+04:00) Baku, Tbilisi, Yerevan */
/*44*/    {  4, 30, ADD, 0, "(GMT+04:30)Kabul"},/* (GMT+04:30) Kabul */
/*45*/    {  5, 0, ADD, 1, "(GMT+05:00)Ekaterinburg"},/* (GMT+05:00) Ekaterinburg*/
/*46*/    {  5, 0, ADD, 0, "(GMT+05:00)Islamabad, Karachi, Tashkent"},/* (GMT+05:00) Islamabad, Karachi, Tashkent*/
/*47*/    {  5, 30, ADD, 0, "(GMT+05:30)Calcutta, Chennai, Mumbai, New Delhi"},/* (GMT+05:30) Calcutta, Chennai, Mumbai, New Delhi */
/*48*/    {  5, 45, ADD, 0, "(GMT+05:45)Kathmandu"},/* (GMT+05:45) Kathmandu */ 
/*49*/    {  6, 0, ADD, 1, "(GMT+06:00)Almaty, Novosibirsk"},/*	(GMT+06:00) Almaty, Novosibirsk*/
/*50*/    {  6, 0, ADD, 0, "(GMT+06:00)Sri Jayawardenepura"},/*	(GMT+06:00) Sri Jayawardenepura*/
/*51*/    {  6, 0, ADD, 0, "(GMT+06:00)Astana, Dhaka"},/* (GMT+06:00) Astana, Dhaka */
/*52*/    {  6, 30, ADD, 0, "(GMT+06:30)Rangoon"},/*	(GMT+06:30) Rangoon	*/
/*53*/    {  7, 0, ADD, 1, "(GMT+07:00)Krasnoyarsk"},/*	(GMT+07:00) Krasnoyarsk	*/
/*54*/    {  7, 0, ADD, 0, "(GMT+07:00)Bangkok, Hanoi, Jakarta"},/*	(GMT+07:00) Bangkok, Hanoi, Jakarta	*/
/*55*/    {  8, 0, ADD, 0, "(GMT+08:00)Kuala Lumpur, Singapore"},	 /*(GMT+08:00) Kuala Lumpur, Singapore */
/*56*/    {  8, 0, ADD, 0, "(GMT+08:00)Taipei"}, /*(GMT+08:00) Taipei*/ 
/*57*/    {  8, 0, ADD, 1, "(GMT+08:00)Irkutsk, Ulaan Bataar"},  /*(GMT+08:00) Irkutsk, Ulaan Bataar*/
/*58*/    {  8, 0, ADD, 0, "(GMT+08:00)Beijing, Chongqing, Hong Kong, Urumqi"},/* (GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi*/
/*59*/    {  8, 0, ADD, 0, "(GMT+08:00)Perth"}, /*(GMT+08:00) Perth */
/*60*/    {  9, 0, ADD, 1, "(GMT+09:00)Yakutsk"},	/*(GMT+09:00) Yakutsk*/
/*61*/    {  9, 0, ADD, 0, "(GMT+09:00)Seoul"}, /*	 (GMT+09:00) Seoul */
/*62*/    {  9, 0, ADD, 0, "(GMT+09:00)Osaka, Sapporo, Tokyo"}, /*(GMT+09:00) Osaka, Sapporo, Tokyo*/
/*63*/    {  9, 30, ADD, 0, "(GMT+09:30)Darwin"},	/*(GMT+09:30) Darwin */
/*64*/    {  9, 30, ADD, 1, "(GMT+09:30)Adelaide"}, /*(GMT+09:30) Adelaide */
/*65*/    { 10, 0, ADD, 1, "(GMT+10:00)Canberra, Melbourne, Sydney"},	/*(GMT+10:00) Canberra, Melbourne, Sydney */
/*66*/    { 10, 0, ADD, 0, "(GMT+10:00)Brisbane"}, /*(GMT+10:00) Brisbane	*/
/*67*/    { 10, 0, ADD, 1, "(GMT+10:00)Vladivostok"}, /*(GMT+10:00) Vladivostok*/
/*68*/    { 10, 0, ADD, 0, "(GMT+10:00)Guam, Port Moresby"}, /*(GMT+10:00) Guam, Port Moresby	*/
/*69*/    { 10, 0, ADD, 0, "(GMT+10:00)Hobart"},/* (GMT+10:00) Hobart*/
/*70*/    { 11, 0, ADD, 0, "(GMT+11:00)Magadan, Solomon Is., New Caledonia"}, /*(GMT+11:00) Magadan, Solomon Is., New Caledonia*/
/*71*/    { 12, 0, ADD, 0, "(GMT+12:00)Fiji, Kamchatka, Marshall Is"},/*(GMT+12:00) Fiji, Kamchatka, Marshall Is.	*/
/*72*/    { 12, 0, ADD, 1, "(GMT+12:00)Auckland, Wellington"}, /*(GMT+12:00) Auckland, Wellington	*/
/*73*/    { 13, 0, ADD, 0, "(GMT+13:00)Nuku'alofa"}, /*(GMT+13:00) Nuku'alofa	 */
};

/**
* <b>FUNCTION: </b>
*
* ShowDateTime(struct SDateTime dt)
*
* <b>DESCRIPTION: </b>
* 
* Show: day, month, year; hour, minute, second
* 
* @param
*   struct SDateTime dt
*       
* @return No
*/
void CTimer::ShowDateTime(struct SDateTime dt)
{
	printf( "Day	DD/MM/YYYY: [%d] %02d/%02d/%d\n", dt.ymdw.week_day, dt.ymdw.day, dt.ymdw.month, dt.ymdw.year);
	printf( "Hour	hh:mm:ss: %02d:%02d:%02d  TimeZone: %d\n",dt.hms.hour, dt.hms.minute, dt.hms.second, uiTimeZIdx);
}

/**
* <b>FUNCTION: </b>
*
* CTimer()
*
* <b>DESCRIPTION: </b>
* 
* Constructor
* 
* @param
*       
* @return No
*/
CTimer::CTimer()
{
    console_debug("Constructor\n");
    Init();
	uiTimeZIdx = 0;
}

/**
* <b>FUNCTION: </b>
*
* ~CTimer()
*
* <b>DESCRIPTION: </b>
* 
* Destructor
* 
* @param    No
*       
* @return	No
*/
CTimer::~CTimer()
{
     console_output("Destructor\n");
     Free();
}

/**
* <b>FUNCTION: </b>
*
* Init()
*
* <b>DESCRIPTION: </b>
* 
* Init data
* 
* @param    No
*       
* @return	No
*/
void CTimer::Init()
{
	pthread_mutex_init(&m_timerMutex, NULL);
}

/**
* <b>FUNCTION: </b>
*
* Free()
*
* <b>DESCRIPTION: </b>
* 
* Free data
* 
* @param    No
*       
* @return	No
*/
void CTimer::Free()
{
	pthread_mutex_destroy(&m_timerMutex);
}

/**
* <b>FUNCTION: </b>
*
* Lock()
*
* <b>DESCRIPTION: </b>
* 
* @param    No
*       
* @return
*   0 if success. Otherwise <> 0
*/
int CTimer::Lock()
{
    return pthread_mutex_lock(&m_timerMutex);
}

/**
* <b>FUNCTION: </b>
*
* UnLock()
*
* <b>DESCRIPTION: </b>
* 
* @param    No
*       
* @return
*   0 if success. Otherwise <> 0
*/
int CTimer::UnLock()
{
    return pthread_mutex_unlock(&m_timerMutex);
}

/**
* <b>FUNCTION: </b>
*
* ChangeDateTime(struct SDateTime dt1, struct SHourMinuteSecond inc, struct SDateTime *dt2)
*
* <b>DESCRIPTION: </b>
* 
* Add inc value to the DateTime struct dt1, and return dt2.
*
* @param
*	struct SDateTime dt1			: input SDateTime struct
*	
*	struct HourMinuteSecond inc	: the times will be add to dt1.
*
*	struct SDateTime *dt2			: output SDateTime  struct.
*       
* @return
*   RM_OK if success. Otherwise RM_ERROR
*/
RMint16 CTimer::ChangeDateTime(struct SDateTime dt1, struct SHourMinuteSecond inc, struct SDateTime *dt2)
{
	RMuint8 num_day = 0;
	RMuint8 odd_hour = 0, odd_minute = 0;
	RMuint8 odd_year = 0, odd_month = 0, odd_day = 0, odd_week;
	
	//	printf("inc : hms = %d:%d:%d\n",inc.hour,inc.minute,inc.second);
	//Second
	
	RMuint8 total;
	
	total = dt1.hms.second + inc.second;
	odd_minute = (total >= 60);
	dt2->hms.second = total % 60;
	//	printf(" second %d >> %d (+%d)\t", total, dt2->hms.second, odd_minute);
	
	//Minute
	total = dt1.hms.minute + inc.minute + odd_minute;
	odd_hour = (total >= 60);
	dt2->hms.minute = total % 60;
	//	printf(" minute %d >> %d (+%d)\t", total, dt2->hms.minute, odd_hour);
	
	//Hours
	total = dt1.hms.hour + inc.hour + odd_hour;
	odd_day = (total >= 24);
	dt2->hms.hour = total % 24;
	//	printf(" hour %d >> %d (+%d)\n", total, dt2->hms.hour, odd_day);
	
	//DayWeek
	total = dt1.ymdw.week_day + odd_day;
	odd_week = (total > 7);
	if (odd_week > 0)
		dt2->ymdw.week_day = 1;
	else 
		dt2->ymdw.week_day = total;
	//	printf(" week_day %d >> %d \t", total, dt2->ymdw.week_day);
	
	//Day Month
	switch(dt1.ymdw.month) 
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		num_day = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		num_day = 30;
		break;
	case 2:
		if (((dt1.ymdw.year % 100) != 0) && (((dt1.ymdw.year % 4) == 0) || ((dt1.ymdw.year %400) == 0)))
			num_day = 29;
		else 
			num_day = 28;
		break;
	}
	
	//Day
	total = dt1.ymdw.day + odd_day;
	odd_month = total > num_day;
	if (odd_month > 0)
		dt2->ymdw.day = 1;
	else 
		dt2->ymdw.day = total;
	//	printf(" day %d >> %d (+%d)\t", total, dt2->ymdw.day, odd_month);
	
	
	//Month
	total = dt1.ymdw.month + odd_month;
	odd_year = (total > 12);
	if (odd_year)
		dt2->ymdw.month = 1;
	else 
		dt2->ymdw.month = total;
	
	//	printf(" month %d >> %d (+%d)\t", total, dt2->ymdw.month, odd_year);
	
	
	//Year
	dt2->ymdw.year = dt1.ymdw.year + odd_year;
	//	printf(" year %d \n", dt2->ymdw.year);
	
	//	printf("dt1 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt1.hms.hour,dt1.hms.minute,dt1.hms.second,dt1.ymdw.week_day, dt1.ymdw.day,dt1.ymdw.month,dt1.ymdw.year);
	//	printf("dt2 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt2->hms.hour,dt2->hms.minute,dt2->hms.second,dt2->ymdw.week_day, dt2->ymdw.day,dt2->ymdw.month,dt2->ymdw.year);
	
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* CompareDateTime(struct SDateTime dt1, struct SDateTime dt2)
*
* <b>DESCRIPTION: </b>
* 
* Compare SDateTime
*
* @param
*	struct SDateTime dt1, struct SDateTime dt2
*       
* @return
*   -1 if dt1 < dt2, 0 if dt1 == dt2, 1 if dt1 > dt2
*/
RMint16	CTimer::CompareDateTime(struct SDateTime dt1, struct SDateTime dt2)
{
	RMuint64 uiSec1, uiSec2;

	GetSecondFromDate(uiSec1, dt1);
	GetSecondFromDate(uiSec2, dt2);

	if (uiSec1 < uiSec1)
		return -1;
	else if (uiSec1 < uiSec2)
		return 1;
	
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth)
*
* <b>DESCRIPTION: </b>
*
* @param
*	RMuint16 uiYear	: Year
*
*	RMuint16 uiMonth: Month [1..12]
*       
* @return
*   Num of days of month
*/
RMuint8 CTimer::GetNumDayOfMonth(RMuint16 uiYear, RMuint16 uiMonth)
{
	
	RMuint8  uiNumDay = 0;
	switch(uiMonth) {
	case 4:
	case 6:
	case 9:
	case 11:
		uiNumDay = 30;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		uiNumDay = 31;
		break;
		
	case 2:
		if ( (uiYear % 100 != 0 && uiYear % 4 == 0) ||  (uiYear % 400 == 0) )
		{
			uiNumDay = 29;
		}
		else 
		{
			uiNumDay = 28;
		}
		break;
		
	default:
		uiNumDay = 0;
	}
    return uiNumDay;
}

/**
* <b>FUNCTION: </b>
*
* GetNumSecondOfYear(RMuint16 uiYear)
*
* <b>DESCRIPTION: </b>
*
* @param
*	RMuint16 uiYear	: Year
*
* @return
*   Num of second of year
*/
RMuint32 CTimer::GetNumSecondOfYear(RMuint16 uiYear)
{
	RMuint32 uiRet = 0;
	RMuint16 uiNumDay = 0;
	
	if ( (uiYear % 100 != 0 && uiYear % 4 == 0) ||  (uiYear % 400 == 0) )
	{
		uiNumDay = 366;
	}
	else 
	{
		uiNumDay = 365;
	}
	
	uiRet += uiNumDay * 24 * 60 * 60;
	return uiRet;
}

/**
* <b>FUNCTION: </b>
*
* GetNumSecondOfMonth(RMuint16 uiYear, RMuint16 uiMonth)
*
* <b>DESCRIPTION: </b>
*
* @param
*	RMuint16 uiYear		: Year
*
*	RMuint16 uiMonth	: Month
*
* @return
*   Num of second of month
*/
RMuint32 CTimer::GetNumSecondOfMonth(RMuint16 uiYear, RMuint16 uiMonth)
{
	RMuint32 uiRet = 0;
	RMuint8  uiNumDay = 0;
	
	
	switch(uiMonth) {
	case 4:
	case 6:
	case 9:
	case 11:
		uiNumDay = 30;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		uiNumDay = 31;
		break;
		
	case 2:
		if ( (uiYear % 100 != 0 && uiYear % 4 == 0) ||  (uiYear % 400 == 0) )
		{
			uiNumDay = 29;
		}
		else 
		{
			uiNumDay = 28;
		}
		break;
		
	default:
		uiNumDay = 0;;
	}
	
	uiRet = uiNumDay * 24 * 60 * 60;
	return uiRet ;
}

/**
* <b>FUNCTION: </b>
*
* GetSystemTime(struct SDateTime* pDt)
*
* <b>DESCRIPTION: </b>
*
* Get datetime from system and save into SDateTime struct
*
* @param
*	struct SDateTime* pDt: current time getting from system
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
RMint32 CTimer::GetSystemTime(struct SDateTime* pDt)
{


	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	GetDateFromSecond(tv.tv_sec, pDt);

	



	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* GetSystemTime(RMuint32 uiClockId, struct SDateTime* pstUTC)
*
* <b>DESCRIPTION: </b>
*
* @param
*	RMuint32 uiClockId, struct SDateTime* pstUTC
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
RMstatus CTimer::GetSystemTime(RMuint64 uiClockId, struct SDateTime* pstUTC)
{
	struct timeval	stTV;
	struct tm		sTM;

	time_t timeCurrent;
	struct tm *tstruct1;

	
	gettimeofday(&stTV, NULL);
	
	stTV.tv_sec		+= uiClockId;
	printf("\nTEST*****uiClockId=%llu*********\n", uiClockId);
	//	printf("GetsystemTime: ");
	//	PrintfUTCTime(stTV.tv_sec);
	gmtime_r(&stTV.tv_sec, &sTM);

/*
	
	//year
	pstUTC->ymdw.year		= sTM.tm_year + 1900 + 1;		//[1900, ...]
	//month
	pstUTC->ymdw.month		= sTM.tm_mon + 1;		//[0, 11]
	//day
	pstUTC->ymdw.day		= sTM.tm_mday;		//[1, 31]
	
	//hour
	pstUTC->hms.hour		= sTM.tm_hour;		//[0, 23]
	//minute
	pstUTC->hms.minute		= sTM.tm_min;		//[0, 59]
	//second
	//pstUTC->hms.second		= sTM.tm_sec;		//[0, 59]
	pstUTC->hms.second		= sTM.tm_sec;		//[0, 59]
	//week day
	pstUTC->ymdw.week_day	= sTM.tm_wday;

*/
//@@@ Minh add
	timeCurrent = (time_t)time(NULL);
    tstruct1 = localtime(&timeCurrent);
    //year
    pstUTC->ymdw.year       =tstruct1->tm_year + 1900;       //[1900, ...]
    //month
    pstUTC->ymdw.month      =tstruct1->tm_mon + 1;       //[0, 11]
    //day
    pstUTC->ymdw.day        =tstruct1->tm_mday;      //[1, 31]

    //hour
    pstUTC->hms.hour        =tstruct1->tm_hour;      //[0, 23]
    //minute
    pstUTC->hms.minute      =tstruct1->tm_min;       //[0, 59]
    //second
    //pstUTC->hms.second        = sTM.tm_sec;       //[0, 59]
    pstUTC->hms.second      =tstruct1->tm_sec;       //[0, 59]
    //week day
    pstUTC->ymdw.week_day   =tstruct1->tm_wday;
//@@@


	if (pstUTC->ymdw.week_day == 0) {
		pstUTC->ymdw.week_day = 7;
	}
	
	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* CreateClockFromDT(struct SDateTime* pDt, RMuint32* puiClockId)
*
* <b>DESCRIPTION: </b>
*
* @param
*	struct SDateTime* pDt, RMuint32* puiClockId
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
RMstatus CTimer::CreateClockFromDT(struct SDateTime* pDt, RMuint64* puiClockId, RMuint16 uiPosTimeZone)
{
	RMuint32 uiSec = 0;
	struct tm stTM;
	struct timeval stTV;
	
	
	if (pDt == NULL || puiClockId == NULL) {
		printf("Invalid arguments \n");
		return RM_ERROR;
	}
	
	stTM.tm_year	= pDt->ymdw.year;		//[1900, ...]
	if (stTM.tm_year >= 1900) {
		stTM.tm_year -= 1900;
	}
	stTM.tm_mon		= pDt->ymdw.month - 1;	//[0, 11]
	stTM.tm_mday	= pDt->ymdw.day;		//[1, 31]
	
	stTM.tm_hour	= pDt->hms.hour;		//[0, 23]
	stTM.tm_min		= pDt->hms.minute;		//[0, 59]
	stTM.tm_sec		= pDt->hms.second;		//[0, 59]
	if(TimeZoneTable[uiPosTimeZone].daylight == 1)
	{
		printf("\n\n====> day light saving..................\n\n");
		stTM.tm_isdst	= -1;	//PhuNguyen fixed
	}
	else
	{
		printf("\n\n====> no day light saving..................\n\n");
		stTM.tm_isdst	= 0;
	}

	uiSec			= mktime(&stTM);
	
	gettimeofday(&stTV, NULL);
	*puiClockId		= uiSec - stTV.tv_sec;
	//	printf("[mktime] DD:MM:YYY HH:MM:SS = %2.2d:%2.2d:%d %2.2d:%2.2d:%2.2d \n", 
	//		stTM.tm_mday, stTM.tm_mon + 1, stTM.tm_year + 1900,
	//		stTM.tm_hour, stTM.tm_min, stTM.tm_sec);
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//	//Not use timezone of OS
	extern char *tzname[2];
	extern long timezone;
	extern int daylight;
	tzset();
	printf("timezone: %s = %ld, ds = %d \n", tzname[0], timezone, daylight);
	printf("[mktime] DD:MM:YYY HH:MM:SS = %2.2d:%2.2d:%d %2.2d:%2.2d:%2.2d \n", 
		stTM.tm_mday, stTM.tm_mon + 1, stTM.tm_year + 1900,
		stTM.tm_hour, stTM.tm_min, stTM.tm_sec);
	//remove timezone of OS.
	(*puiClockId)		-= timezone;
//	(*puiClockId)		+= (daylight*60*60);
	//////////////////////////////////////////////////////////////////////////
	
#if 1
	printf("CreateClockFromDT %llu ", *puiClockId );
	
//	PrintfUTCTime((*puiClockId) + stTV.tv_sec);
#endif
	
	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* SetSystemTime(struct SDateTime dt)
*
* <b>DESCRIPTION: </b>
*
* Set datetime to system
*
* @param
*	struct SDateTime dt
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
RMint32 CTimer::SetSystemTime(struct SDateTime dt)
{
	struct timeval now;
	RMuint64 uiSec = 0;

	GetSecondFromDate(uiSec, dt);
	now.tv_sec = uiSec;
	now.tv_usec = 0;
	
	if (settimeofday(&now, NULL) != 0)
	{
		console_err ("Set ting using settimeofday is failed. Try again\n");
		if (settimeofday(&now, NULL) != 0)
			return RM_ERROR;
	}
	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* GetHttpUrlFromPlaylistUrl(const RMascii *playlistUrl, RMascii *httpUrl)
*
* <b>DESCRIPTION: </b>
*
* Check and return URL
*
* @param
*	const RMascii *playlistUrl	: IP address input. Ex: 192.168.1.29
*
*	RMascii *httpUrl			: Url output. Ex: http://192.168.1.29
*
* @return
*	1 if success. Otherwise 0
*/
RMint32 CTimer::GetHttpUrlFromPlaylistUrl(const RMascii *playlistUrl, RMascii *httpUrl)
{
	const RMascii *p;
    unsigned int ip[4];
    unsigned int length = 0;
    unsigned int i = 0;
	int n = 0;
    length = strlen(playlistUrl);
	
    while(i < length)
    {
		p = playlistUrl + i;
	
		if (isdigit(p[0]))
		{
			n = sscanf(p, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
			if (n == 4)
			{
                snprintf(httpUrl, 30, "http://%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
                httpUrl[strlen(httpUrl)] = '\0';
				break;
			}
		}
		i++;
    }
    
	return (n == 4); //0: Err 1: OK
}

/**
* <b>FUNCTION: </b>
*
* GetTimeZoneInfor(RMuint8 idx, STimeZone *stTzone)
*
* <b>DESCRIPTION: </b>
*
* Return time zone information
*
* @param
*	RMuint8		idx			: Index of TimeZone table [0..73]	
*
*	STimeZone	*stTzone	: TimeZone output
*
* @return
*	RM_OK if success. Otherwise RM_ERROR
*/
RMstatus CTimer::GetTimeZoneInfor(RMuint8 idx, STimeZone	*stTzone)
{
	
	if (idx >= MAX_TIME_ZONE)
		return RM_ERROR;

	*stTzone = TimeZoneTable[idx];

	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* ChangeDayTimeZone(struct SDateTime dt1, STimeZone stTzone, RMuint32 daylight, struct SDateTime *dt2)
*
* <b>DESCRIPTION: </b>
*
* @param
*   struct SDateTime dt1, STimeZone stTzone, RMuint32 daylight, struct SDateTime *dt2
* 
* @return
*   0 if success. Otherwise 1
*/
RMint16 CTimer::ChangeDayTimeZone(struct SDateTime dt1, STimeZone stTzone, RMuint32 daylight, struct SDateTime *dt2)
{
	RMint16 odd = 0, total = 0;
	RMint16 tz_hour, tz_minute;	
	RMuint16 num_day;
//	printf("inc : hms = [%d:%d] Daylight=[%ld]\n",stTzone.hour,stTzone.minute,daylight);
	
	if (stTzone.status == ADD)
	{
		tz_hour		= stTzone.hour;
		tz_minute	= stTzone.minute;
	}
	else //stTzone.status == MINUS)
	{
		tz_hour		= -stTzone.hour;
		tz_minute	= -stTzone.minute;
	}


	//Second
	dt2->hms.second = dt1.hms.second;
	//printf(" second %d >> %d (+%d)\t", total, dt2->hms.second, odd_minute);
		
	//Minute
	total = dt1.hms.minute + tz_minute;
	if (total >= 60) {
		dt2->hms.minute = (RMuint8) (total % 60);
		odd				=  1; 
	}
	else if (total >= 0) {
		dt2->hms.minute = (RMuint8) total;
		odd				= 0;
	}
	else { //total <= 0
		dt2->hms.minute = (RMuint16) total + 60;
		odd				= -1;		
	}
	//printf(" minute %d >> %d (+%d)\t", total, dt2->hms.minute, odd_hour);
		
	//Hours
	if (daylight == 0) {
		total = dt1.hms.hour + tz_hour + odd;
	}
	else {
		total = dt1.hms.hour + tz_hour + odd + 1;
	}
	if (total >= 24) {
		dt2->hms.hour	= (RMuint8) (total % 24);
		odd				= 1;
	}
	else if (total >= 0){
		dt2->hms.hour	= (RMuint8) (total);
		odd				= 0;
	}
	else { //total <= 0
		dt2->hms.hour	= (RMuint8) (total + 24);
		odd				= -1;
	}
	//printf(" hour %d >> %d (+%d)\n", total, dt2->hms.hour, odd_day);	
	
	//DayWeek
	total = dt1.ymdw.week_day + odd;
	if (total > 7) {
		dt2->ymdw.week_day = 1;
	} 
	else if (total > 0) {
		dt2->ymdw.week_day = (RMuint8) total;
	}
	else { // total <= 0
		dt2->ymdw.week_day = (RMuint8) (total + 7);
	}
	//printf(" week_day %d >> %d \t", total, dt2->ymdw.week_day);
	
	//Day Month
	num_day = GetNumDayOfMonth(dt1.ymdw.year, dt1.ymdw.month);
	
	//Day
	total = dt1.ymdw.day + odd;
	if (total > num_day) {
		dt2->ymdw.day	= 1;
		odd				= 1;
	}
	else if (total > 0) {
		dt2->ymdw.day	= (RMuint16)total;
		odd				= 0;
	}
	else { //total <= 0
		//set day later
		odd = -1;
	}
	
	//Month
	total = dt1.ymdw.month + odd;
	if (total > 12)
	{
		dt2->ymdw.year		= dt1.ymdw.year + 1;
		dt2->ymdw.month		= 1;
	}
	else if (total > 0)
	{
		dt2->ymdw.year		= dt1.ymdw.year;
		dt2->ymdw.month		= (RMuint16)total;
	}
	else // total == 0
	{
		dt2->ymdw.year		= dt1.ymdw.year - 1;
		dt2->ymdw.month		= 12;
	}
	
	if (odd == -1) {
		dt2->ymdw.day		= GetNumDayOfMonth(dt2->ymdw.year, dt2->ymdw.month);
	}

	//	printf("dt1 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt1.hms.hour,dt1.hms.minute,dt1.hms.second,dt1.ymdw.week_day, dt1.ymdw.day,dt1.ymdw.month,dt1.ymdw.year);
	//	printf("dt2 : hms = %d:%d:%d\t dayweek, dmy = %d, %d %d %d\n",dt2->hms.hour,dt2->hms.minute,dt2->hms.second,dt2->ymdw.week_day, dt2->ymdw.day,dt2->ymdw.month,dt2->ymdw.year);
	
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* AddTimeZone(struct SDateTime dt1, RMint16 iPosStartTimeZone, RMint16 iPosEndTimeZone, struct SDateTime *dt2)
*
* <b>DESCRIPTION: </b>
*
* @param
*   struct SDateTime dt1, RMint16 iPosStartTimeZone, RMint16 iPosEndTimeZone, struct SDateTime *dt2
* 
* @return
*   0 if success. Otherwise 1
*/
RMint16 CTimer::AddTimeZone(struct SDateTime dt1, RMint16 iPosStartTimeZone, RMint16 iPosEndTimeZone, struct SDateTime *dt2)
{
	RMuint8 num_day = 0;
	RMuint8 odd_hour = 0;
    RMint8 odd_year = 0, odd_month = 0, odd_day = 0, odd_week;

    RMint8 total;
	RMint16 iTimeZone, iStartTimeZone, iEndTimeZone; //Hour 
    RMint16 iTimeZoneMinute, iStartTimeZoneMinute, iEndTimeZoneMinute; //Minute

	if (TimeZoneTable[iPosStartTimeZone].status == MINUS)
	{
		iStartTimeZone = -TimeZoneTable[iPosStartTimeZone].hour;
        iStartTimeZoneMinute = -TimeZoneTable[iPosStartTimeZone].minute;
	}
	else
    {
        iStartTimeZone = TimeZoneTable[iPosStartTimeZone].hour;
        iStartTimeZoneMinute = TimeZoneTable[iPosStartTimeZone].minute;
    }		


	if (TimeZoneTable[iPosEndTimeZone].status == MINUS)
	{
		iEndTimeZone = -TimeZoneTable[iPosEndTimeZone].hour;
        iEndTimeZoneMinute = -TimeZoneTable[iPosEndTimeZone].minute;
	}
	else
    {
        iEndTimeZone = TimeZoneTable[iPosEndTimeZone].hour;
        iEndTimeZoneMinute = TimeZoneTable[iPosEndTimeZone].minute;
    }

	iTimeZone = iEndTimeZone - iStartTimeZone ;
    iTimeZoneMinute = iEndTimeZoneMinute - iStartTimeZoneMinute ;

    //Second
    dt2->hms.second = dt1.hms.second;

    //Minute
    total = dt1.hms.minute + iTimeZoneMinute;
    if(total < 0)
    {
        total = 60 + total;
        odd_hour = -1;
    }
    else
    {
        odd_hour = (total >= 60);
    }		
	dt2->hms.minute = total % 60;

    //Hours
	total = dt1.hms.hour + iTimeZone + odd_hour;
    if(total < 0)
    {
        total = 24 + total;
        odd_day = -1;
    }
    else
    {
        odd_day = (total >= 24);        
    }	
    dt2->hms.hour = total % 24;    
	//	printf(" hour %d >> %d (+%d)\n", total, dt2->hms.hour, odd_day);
	//DayWeek    
	total = dt1.ymdw.week_day + odd_day;
    if(total < 0)
    {
        total = 7 + total;
        odd_week = -1;
    }
    else
    {
        odd_week = (total > 7);
    }	
	if (odd_week > 0)
		dt2->ymdw.week_day = 1;
	else 
		dt2->ymdw.week_day = total;
	//	printf(" week_day %d >> %d \t", total, dt2->ymdw.week_day);
	
	//Day Month
	num_day = GetNumDayOfMonth(dt1.ymdw.year, dt1.ymdw.month);
	
	//Day
	total = dt1.ymdw.day + odd_day;
    if(total < 0)
    {
        num_day = GetNumDayOfMonth(dt1.ymdw.year, dt1.ymdw.month-1);
        total = num_day + total;
        odd_month = -1;
    }
    else
    {
        odd_month = total > num_day;
    }	
	if (odd_month > 0)
		dt2->ymdw.day = 1;
	else 
		dt2->ymdw.day = total;
	//	printf(" day %d >> %d (+%d)\t", total, dt2->ymdw.day, odd_month);
	
	
	//Month
	total = dt1.ymdw.month + odd_month;
    if(total < 0)
    {
        total = 12 + total;
        odd_year = -1;
    }
    else
    {
       	odd_year = (total > 12);
    }
	if (odd_year)
		dt2->ymdw.month = 1;
	else 
		dt2->ymdw.month = total;
	
	//	printf(" month %d >> %d (+%d)\t", total, dt2->ymdw.month, odd_year);
	
	//Year
	dt2->ymdw.year = dt1.ymdw.year + odd_year;
	//	printf(" year %d \n", dt2->ymdw.year);
	
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* GetDateFromSecondEx(RMuint32 uiSecond, STimeZone stTzone,
* RMuint32 daylight, struct SDateTime* pDt)
*
* <b>DESCRIPTION: </b>
*
* @param
*   RMuint32 uiSecond, STimeZone stTzone, RMuint32 daylight,
*   struct SDateTime* pDt
* 
* @return
*   RM_OK if success. Otherwise RM_ERROR
*/
RMint32	CTimer::GetDateFromSecondEx(RMuint32 uiSecond, STimeZone stTzone, 
							RMuint32 daylight, struct SDateTime* pDt)
{
	RMuint16 i			= 0;
	RMuint32 uiTemp		= 0;
	RMuint32 uiTempSec	= 0;
	RMuint32 MJD		= 0;
	RMuint16 Y, M, D, L;
	if (pDt == NULL)
	{
		printf("getDateFromSecond: pDt == NULL \n");
		return RM_ERROR;
	}
	//Set TimeZone
	switch(stTzone.status) {
	case ADD:
		uiSecond += (stTzone.hour * 60 + stTzone.minute) * 60;
		break;
	case MINUS:
		uiSecond -= (stTzone.hour * 60 + stTzone.minute) * 60;		
		break;
	}
	
	//Set Daylight Saving is On ( = 1)
	if (daylight == 1) {
		uiSecond += 60 * 60;		
	}

	//year
	uiTempSec	= 0;
	uiTemp		= 0;
	i			= 1970;
	while (uiTemp <= uiSecond) 
	{
		uiTempSec	= uiTemp;
		uiTemp		+= GetNumSecondOfYear(i);
		i ++;
	}
	pDt->ymdw.year	= i - 1;
	uiSecond		-= uiTempSec;

	//Month
	uiTemp		= 0;
	uiTempSec	= 0;
	i			= 1;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= GetNumSecondOfMonth(pDt->ymdw.year, i);
		i ++;
	}
	pDt->ymdw.month = i - 1;
	uiSecond		-= uiTempSec;

	//day
	uiTemp		= 0;
	uiTempSec	= 0;
	i			= 1;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= 24*60*60;
		i			++;
	}
	pDt->ymdw.day	= i - 1;
	uiSecond		-= uiTempSec;

	//Hour
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= 60*60;
		i ++;
	}
	pDt->hms.hour = i - 1;
	uiSecond -= uiTempSec;

	//Minute
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+=  60;
		i			++;
	}
	pDt->hms.minute = i - 1;
	uiSecond -= uiTempSec;

	//Second
	pDt->hms.second = uiSecond;


	//MDJ

/*
   If M = 1 or M = 2, then L = 1; else L = 0 
   MJD = 14 956 + D + int [ (Y - L) × 365,25] + int [ (M + 1 + L × 12) × 30,6001 ] 
   
*/
	Y = pDt->ymdw.year;
	M = pDt->ymdw.month;
	D = pDt->ymdw.day;
	if (M == 1 || M == 2)
	{
		L = 1;
	}
	else
	{
		L = 0;
	}
	MJD = 14956 + D + (RMuint16)( (Y - L)*365.25 ) + (RMuint16)( (M + 1 + L*12) * 30.6001);
	
	pDt->ymdw.week_day =  ((MJD + 2) % 7); //Sun = 0, Sat = 7.

	return RM_OK;
	
}
// start Set Auto Time


/**
* <b>FUNCTION: </b>
*
* GetSecondFromDate(RMuint32 &uiSecond, struct SDateTime sDt)
*
* <b>DESCRIPTION: </b>
*
* @param
*   RMuint32 &uiSecond, struct SDateTime sDt
* 
* @return
*   0 if success. Otherwise 1
*/
RMuint64 CTimer::GetSecondFromDate(RMuint64 &uiSecond, struct SDateTime sDt)
{
	RMuint16 i = 0;
	
	RMascii	s1[20];
	snprintf(s1, sizeof(s1), "date %02d%02d%02d%02d%04d.%02d", sDt.ymdw.month, sDt.ymdw.day, 
		sDt.hms.hour, sDt.hms.minute, sDt.ymdw.year, sDt.hms.second);

	uiSecond = 0;

	for (i = 1970; i < sDt.ymdw.year; i ++)
		uiSecond += GetNumSecondOfYear(i);

	for (i = 1; i < sDt.ymdw.month; i ++)
		uiSecond += GetNumSecondOfMonth(sDt.ymdw.year, i);

	uiSecond += (sDt.ymdw.day-1) * 24*60*60;
	//uiSecond += (sDt.hms.hour + daylight)* 60 * 60;
	uiSecond += sDt.hms.hour* 60 * 60;
	uiSecond += sDt.hms.minute * 60;
	uiSecond += sDt.hms.second;

	return 0;
}

/**
* <b>FUNCTION: </b>
*
* GetDateFromSecond(RMuint32 uiSecond, struct SDateTime* pDt)
*
* <b>DESCRIPTION: </b>
*
* @param
*   RMuint32 uiSecond, struct SDateTime* pDt
* 
* @return
*   RM_OK if success. Otherwise RM_ERROR
*/
RMint64 CTimer::GetDateFromSecond(RMuint32 uiSecond, struct SDateTime* pDt)
{
	RMuint16 i			= 0;
	RMuint32 uiTemp		= 0;
	RMuint32 uiTempSec	= 0;
	RMuint32 MJD		= 0;
	RMuint16 Y, M, D, L;
	if (pDt == NULL)
	{
		printf("getDateFromSecond: pDt == NULL \n");
		return RM_ERROR;
	}

	//year
	uiTempSec	= 0;
	uiTemp		= 0;
	i			= 1970;
	while (uiTemp <= uiSecond) 
	{
		uiTempSec	= uiTemp;
		//uiTemp		+= GetNumSecondOfYears(i);
		uiTemp		+= GetNumSecondOfYear(i);
		i ++;
	}
	pDt->ymdw.year	= i - 1;
	uiSecond		-= uiTempSec;

	//Month
	uiTemp		= 0;
	uiTempSec	= 0;
	i			= 1;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= GetNumSecondOfMonth(pDt->ymdw.year, i);
		i ++;
	}
	pDt->ymdw.month = i - 1;
	uiSecond		-= uiTempSec;

	
	//day
	uiTemp		= 0;
	uiTempSec	= 0;
	i			= 1;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= 24*60*60;
		i			++;
	}
	pDt->ymdw.day	= i - 1;
	uiSecond		-= uiTempSec;

	//Hour
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+= 60*60;
		i ++;
	}
	pDt->hms.hour = i - 1;
	uiSecond -= uiTempSec;

	//Minute
	uiTemp = 0;
	uiTempSec = 0;
	i = 0;
	while (uiTemp <= uiSecond)
	{
		uiTempSec	= uiTemp;
		uiTemp		+=  60;
		i			++;
	}
	pDt->hms.minute = i - 1;
	uiSecond -= uiTempSec;

	//Second
	pDt->hms.second = uiSecond;


	//MDJ

/*
   If M = 1 or M = 2, then L = 1; else L = 0 
   MJD = 14 956 + D + int [ (Y - L) × 365,25] + int [ (M + 1 + L × 12) × 30,6001 ] 
*/
	Y = pDt->ymdw.year;
	M = pDt->ymdw.month;
	D = pDt->ymdw.day;
	if (M == 1 || M == 2)
	{
		L = 1;
	}
	else
	{
		L = 0;
	}
	MJD = 14956 + D + (RMuint16)( (Y - L)*365.25 ) + (RMuint16)( (M + 1 + L*12) * 30.6001);
	
	pDt->ymdw.week_day =  ((MJD + 2) % 7); //Sun = 0, Sat = 7.

	return RM_OK;
}

/**
* <b>FUNCTION: </b>
*
* check_protocol(unsigned char *p, int l)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *p, int l
* 
* @return
*   i if success. Otherwise -1
*/
int check_protocol(unsigned char *p, int l)
{
	int i;
	for (i = 0; protocols[i].prot; i++)
		if (!casecmp((unsigned char *)protocols[i].prot, p, l) && (int)strlen((char *)protocols[i].prot) == l) {
			return i;
		}
		return -1;
}

/**
* <b>FUNCTION: </b>
*
* link_parse_url(unsigned char *url, int *prlen, unsigned char **user, int *uslen, unsigned char **pass, int *palen, unsigned char **host, int *holen, unsigned char **port, int *polen, unsigned char **data, int *dalen, unsigned char **post)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, int *prlen, unsigned char **user, int *uslen, unsigned char **pass, int *palen, unsigned char **host, int *holen, unsigned char **port, int *polen, unsigned char **data, int *dalen, unsigned char **post
* 
* @return
*   i 0if success. Otherwise 1
*/
int link_parse_url(unsigned char *url, int *prlen, unsigned char **user, int *uslen, unsigned char **pass, int *palen, unsigned char **host, int *holen, unsigned char **port, int *polen, unsigned char **data, int *dalen, unsigned char **post)
{
	unsigned char *p, *q;
	unsigned char p_c[2];
	int a;
	URLDBGMSG("link_parse_url <<<\n");
	if (prlen) *prlen = 0;
	if (user) *user = NULL;
	if (uslen) *uslen = 0;
	if (pass) *pass = NULL;
	if (palen) *palen = 0;
	if (host) *host = NULL;
	if (holen) *holen = 0;
	if (port) *port = NULL;
	if (polen) *polen = 0;
	if (data) *data = NULL;
	if (dalen) *dalen = 0;
	if (post) *post = NULL;
	if (!(char *)url || !(p = (unsigned char *)strchr((char *)url, ':'))) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
	if (prlen) *prlen = p - url;
	if ((a = check_protocol(url, p - url)) == -1) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
	if (p[1] != '/' || p[2] != '/') {
		if (protocols[a].need_slashes) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
		p -= 2;
	}
	if (protocols[a].free_syntax) {
		if (data) *data = p + 3;
		if (dalen) *dalen = strlen((char *)p + 3);
		URLDBGMSG("link_parse_url <<<\n");
		return 0;
	}
	
	p += 3;
	
	q = p + strcspn((char *)p, "@/?");
	if (!*q && protocols[a].need_slash_after_host) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
	if (*q == '@') {
		unsigned char *pp;
		while (strcspn((char *)q + 1, "@") < strcspn((char *)q + 1, "/?"))
			q = q + 1 + strcspn((char *)q + 1, "@");
		pp = (unsigned char *)strchr((char *)p, ':');
		if (!pp || pp > q) {
			if (user) *user = p;
			if (uslen) *uslen = q - p;
		} else {
			if (user) *user = p;
			if (uslen) *uslen = pp - p;
			if (pass) *pass = pp + 1;
			if (palen) *palen = q - pp - 1;
		}
		p = q + 1;
	} 
	q = p + strcspn((char *)p, ":/?");
	if (!*q && protocols[a].need_slash_after_host) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
	if (host) *host = p;
	if (holen) *holen = q - p;
	if (*q == ':') {
		unsigned char *pp = q + strcspn((char *)q, "/");
		int cc;
		if (*pp != '/' && protocols[a].need_slash_after_host) { URLDBGMSG("link_parse_url >>>\n"); return -1; }
		if (port) *port = q + 1;
		if (polen) *polen = pp - q - 1;
		for (cc = 0; cc < pp - q - 1; cc++) if (q[cc+1] < '0' || q[cc+1] > '9') { URLDBGMSG("link_parse_url >>>\n"); return -1; }
		q = pp;
	}
	if (*q && *q != '?') q++;
	p = q;
	p_c[0] = POST_CHAR;
	p_c[1] = 0;
	q = p + strcspn((char *)p, (char *)p_c);
	if (data) *data = p;
	if (dalen) *dalen = q - p;
	if (post) *post = *q ? q + 1 : NULL;
	URLDBGMSG("link_parse_url >>>\n");
	return 0;
}

/**
* <b>FUNCTION: </b>
*
* *get_protocol_name(unsigned char *url, unsigned char *prot_name, int strsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, unsigned char *prot_name, int strsize
* 
* @return
*   port name 0if success. Otherwise NULL
*/
unsigned char *get_protocol_name(unsigned char *url, unsigned char *prot_name, int strsize)
{
	int l;
	memset(prot_name, 0, strsize);
	if (link_parse_url(url, &l, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
	{
		return NULL;
	}
	
	if(l>=strsize) return NULL;
	
	if(l) {
		memcpy(prot_name, url, l);
		return prot_name;
	}
	
	return NULL;
}

/**
* <b>FUNCTION: </b>
*
* *get_user_name(unsigned char *url, unsigned char *user_name, int strsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, unsigned char *user_name, int strsize
* 
* @return
*   user name 0if success. Otherwise NULL
*/
unsigned char *get_user_name(unsigned char *url, unsigned char *user_name, int strsize)
{
	unsigned char *h;
	int hl;
	memset(user_name, 0, strsize);
	
	if (link_parse_url(url, NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) return NULL;
	
	if(hl>=strsize) return NULL;
	memcpy(user_name, h, hl);
	return user_name;
}

/**
* <b>FUNCTION: </b>
*
* *get_pass(unsigned char *url, unsigned char *pass, int strsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, unsigned char *pass, int strsize
* 
* @return
*   pass 0if success. Otherwise NULL
*/
unsigned char *get_pass(unsigned char *url, unsigned char *pass, int strsize)
{
	unsigned char *h;
	int hl;
	memset(pass, 0, strsize);
	
	if (link_parse_url(url, NULL,NULL,  NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) return NULL;
	
	if(hl>=strsize) return NULL;
	memcpy(pass, h, hl);
	return pass;
}

/**
* <b>FUNCTION: </b>
*
* *get_host_name(unsigned char *url, unsigned char *host_name, int strsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, unsigned char *host_name, int strsize
* 
* @return
*   host name 0if success. Otherwise NULL
*/
unsigned char *get_host_name(unsigned char *url, unsigned char *host_name, int strsize)
{
	unsigned char *h;
	int hl;
	memset(host_name, 0, strsize);
	if (link_parse_url(url, NULL, NULL, NULL, NULL, NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL)) return NULL;
	
	if(hl>=strsize) return NULL;
	memcpy(host_name, h, hl);
	return host_name;
}

/**
* <b>FUNCTION: </b>
*
* *get_url_data(unsigned char *url)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url
* 
* @return
*   url if success. Otherwise NULL
*/
unsigned char *get_url_data(unsigned char *url)
{
	unsigned char *d;
	if (link_parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &d, NULL, NULL)) return NULL;
	return d;
}

#define dsep(x) (lo ? dir_sep(x) : (x) == '/')

/**
* <b>FUNCTION: </b>
*
* get_prot_info(unsigned char *prot, int *port, int (**func)(char* url, char* path), void (**nc_func)(void*), int *allow_post)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *prot, int *port, int (**func)(char* url, char* path), void (**nc_func)(void*), int *allow_post
* 
* @return
*   0 if success. Otherwise -1
*/
int get_prot_info(unsigned char *prot, int *port, int (**func)(char* url, char* path), void (**nc_func)(void*), int *allow_post)
{
	int i;
	for (i = 0; protocols[i].prot; i++)
		if (!strcasecmp((char *)protocols[i].prot, (char *)prot)) {
			if (port) *port = protocols[i].port;
			if (func) *func = protocols[i].func;
			if (nc_func) *nc_func = protocols[i].nc_func;
			if (allow_post) *allow_post = protocols[i].allow_post;
			return 0;
		}
		return -1;
}

/**
* <b>FUNCTION: </b>
*
* get_port(unsigned char *url)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url
* 
* @return
*   port number if success. Otherwise -1
*/
int get_port(unsigned char *url)
{
	unsigned char *h;
	unsigned char prot_name[64];
	int hl;
	long n = -1;
	if (link_parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &h, &hl, NULL, NULL, NULL)) return -1;
	if (h) {
		n = strtol((char *)h, NULL, 10);
		if (n && n < MAXINT) return n;
	}
	if ((h = get_protocol_name(url, prot_name, 64))) {
		int nn;
		get_prot_info(h, &nn, NULL, NULL, NULL);
		n = nn;
	}
	return n;
}

/**
* <b>FUNCTION: </b>
*
* *get_url_path(unsigned char *url, unsigned char *path, int strsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *url, unsigned char *path, int strsize
* 
* @return
*   path if success. Otherwise NULL
*/
unsigned char *get_url_path(unsigned char *url, unsigned char *path, int strsize)
{
	unsigned char *d;
	unsigned char *pos;
	int pl;
	memset(path, 0, strsize);
	
	if (link_parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &d, NULL, NULL)) 
		return NULL;
	
	pl=strlen((char *)d);
	if(!pl) return NULL;
	
	pos=(unsigned char *)strrchr((char *)d, '/');
	pl=(int)(pos-d);
	
	if(pl) {
		if(pl>=strsize) return NULL;
		memcpy(path, d, pl);
		return path;
	} else return NULL;
}

/**
* <b>FUNCTION: </b>
*
* spliturl(dload_info_t *dload_info)
*
* <b>DESCRIPTION: </b>
*
* @param
*   dload_info_t *dload_info
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int spliturl(dload_info_t *dload_info)
{
    char *res, *pos, *url;
    int len;
    char tmp[256];

    dload_info->protocol = NULL;
    dload_info->server = NULL;
    dload_info->path = NULL;
    dload_info->file = NULL;
    
    /* memset */
    memset(&dload_info->url_data, 0, MAXBUFSIZE);
        
    if(dload_info->url == NULL) {
        printf("[%s:%d]spliturl(dload_info_t *dload_info) mtda_errno = MTDA_INVALID_URL\n", __FILE__, __LINE__); 
        mtda_errno = MTDA_INVALID_URL;
        return MTDA_ERROR;
    }

    pos = &(dload_info->url_data[0]);
    
    /* protocol */
    res = (char*)get_protocol_name((unsigned char*)dload_info->url, (unsigned char*)tmp, 256);
    if(!res) {
        len = 4;
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->protocol = pos;
        memcpy(pos, "http", len);
        pos = (pos + len + 1);

        /* add default protocol to url */
        len = 7;
        url = dload_info->url;
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->url = pos;
        memcpy(pos, "http://", len);
        pos = (pos + len);

        len = strlen(url);
        if((len + 1) > MAXBUFSIZE) return MTDA_INVALID_URL;
        memcpy(pos, url, len);
        fprintf(stderr, "URL [%s]\n", dload_info->url);
        pos = (pos + len + 1);

    } else {
        len = strlen(tmp);
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->protocol = pos;
        memcpy(pos, tmp, len);
        pos = (pos + len + 1);
    }

    /* user name */
    res = (char*)get_user_name((unsigned char*)dload_info->url, (unsigned char*)tmp, 256);
    if(!res) {
        len = strlen("anonymous");
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        printf("LEN: [%d]\n", len);
        printf("POS: [%p]\n", pos);
        dload_info->ftp_user = pos;
        memcpy(pos, "anonymous", len);
        pos = (pos + len + 1);
    } else {
        len = strlen(tmp);
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->ftp_user = pos;
        memcpy(pos, tmp, len);
        pos = (pos + len + 1);
    }

    /* password */
    res = (char*)get_pass((unsigned char*)dload_info->url, (unsigned char*)tmp, 256);
    if(res) {
        len = strlen(tmp);
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->ftp_pass = pos;
        memcpy(pos, tmp, len);
        pos = (pos + len + 1);
    }

    /* server */
    res = (char*)get_host_name((unsigned char*)dload_info->url, (unsigned char*)tmp, 256);
    if(res) {
        len = strlen(tmp);
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->server = pos;
        memcpy(pos, tmp, len);
        pos = (pos + len + 1);
    } else return MTDA_INVALID_URL;

    /* server port */
    dload_info->server_port = get_port((unsigned char*)dload_info->url);
    printf("get_port [%d]\n", dload_info->server_port);

    
    /* path */
    res = (char*)get_url_path((unsigned char*)dload_info->url, (unsigned char*)tmp, 256);
    printf("get_url_path [%s]\n", tmp);
    if(res) {
        len = strlen(tmp);
        if((int)((pos + len + 1) - dload_info->url_data) > MAXBUFSIZE) return MTDA_INVALID_URL;
        dload_info->path = pos;
        memcpy(pos, tmp, len);
        pos = pos + len + 1;
    }

    /* data */
    res = (char*)get_url_data((unsigned char*)dload_info->url);
   printf("get_url_data [%s]\n", res);
    dload_info->data = res;
    return MTDA_SUCCESS;
}

/**
* <b>FUNCTION: </b>
*
* dump_url(dload_info_t *__dload_info)
*
* <b>DESCRIPTION: </b>
*
* @param
*   dload_info_t *__dload_info
* 
* @return   No
*/
void dump_url(dload_info_t *__dload_info)
{
}

/**
* <b>FUNCTION: </b>
*
* connect_timeout(int tid, int socket, struct sockaddr *server, int size, int timeout_in_sec)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int tid, int socket, struct sockaddr
*    server, int size, int timeout_in_sec
* 
* @return
*   r if success. Otherwise -1
*/
int connect_timeout(int tid, int socket, struct sockaddr *server, int size, int timeout_in_sec)
{
	int r;
	fd_set myset; 
	struct timeval tv; 
	int valopt; 
	socklen_t optlen; 
	
	if(fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
	{
		//ZERROR(("%d, Error fcntl(..., F_SETFL) (%s)\n", tid, strerror(errno)));
		return(-1);
	}
	
	/* trying to connect with timeout */
	r = connect(socket, server, size);
	if(r < 0)
	{
		if(errno == EINPROGRESS)
		{
		//	ZINFO(("%d, EINPROGRESS in connect() - selecting\n", tid)); 
			do 
			{
				tv.tv_sec = timeout_in_sec; 
				tv.tv_usec = 0; 
				FD_ZERO(&myset); 
				FD_SET(socket, &myset); 
				
				r = select(socket+1, NULL, &myset, NULL, &tv); 
				
				if(r < 0 && errno != EINTR)
				{
					//ZERROR(("%d, Error connecting %d - %s\n", tid, errno, strerror(errno))); 
					return(-1); 
				}
				else
				{
					if(r > 0)
					{
						/* socket selected for write */
						optlen = sizeof(int); 
						if(getsockopt(socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &optlen) < 0)
						{
						//	ZERROR(("%d, Error in getsockopt() %d - %s\n", tid,
							//	errno, strerror(errno))); 
							printf("getsockopt  r = %d",r);
							return(-1);
						}
						
						/* check the value returned... */
						if(valopt)
						{
							//ZERROR(("%d, Error in delayed connection() %d - %s\n",
								//tid, valopt, strerror(valopt))); 
							return(-1);
						}
						break; 
					}
					else
					{
						//ZERROR(("%d, Timeout in select() - Cancelling!\n", tid)); 
						return(-1); 
					} 
				}
			} while(1); 
		}
		else
		{
			//ZERROR(("%d, Error connecting %d - %s\n", tid, errno, strerror(errno))); 
			return(-1);
		} 
	}
	return(r);
}

/**
* <b>FUNCTION: </b>
*
* base64_encode(const char *s, char *store, int length)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const char *s, char *store, int length
* 
* @return No
*/
void base64_encode(const char *s, char *store, int length)
{
	/* Conversion table.  */
	char tbl[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
	};
	int i;
	unsigned char *p = (unsigned char *) store;
	
	/* Transform the 3x8 bits to 4x6 bits, as required by base64.  */
	for(i = 0; i < length; i += 3)
	{
		*p++ = tbl[s[0] >> 2];
		*p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
		*p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
		*p++ = tbl[s[2] & 0x3f];
		s += 3;
	}
	/* Pad the result if necessary...  */
	if(i == length + 1)
		*(p - 1) = '=';
	else if(i == length + 2)
		*(p - 1) = *(p - 2) = '=';
	/* ...and zero-terminate it.  */
	*p = '\0';
}

/**
* <b>FUNCTION: </b>
*
* get_ftp_response(int tid, int remotesocket,char *expected_reply)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int tid, int remotesocket,char *expected_reply
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int get_ftp_response(int tid, int remotesocket,char *expected_reply)
{
    char buf[MAXBUFSIZE],actual_reply[MAXBUFSIZE];
    int recvlen;
    int flag;
    char *chtemp;
    //ZDMSG(("%d, Waiting for %s response from FTP server\n", tid, expected_reply));
	
    if((recvlen=read_line(remotesocket,buf,MAXBUFSIZE-1))<0)
    {
       // ZERROR(("%d, Error in retrieving FTP response: %s\n", tid, strerror(errno)));
        close(remotesocket);
        return MTDA_ERROR;
    }
	
    buf[recvlen]='\0';
	//  ZINFO(("%d, FTP: %s\n", tid, buf));
    if(strncmp(buf,expected_reply,3)!=0)
	{
        //ZERROR(("%d, Unexpected response from FTP server: %s\n", tid, buf));
	}
    snprintf(actual_reply, sizeof(actual_reply), "%.3s",buf);
    actual_reply[3]=' ';
	
    /* checking for Mutli line reply - courtsey FTP Protocol Specs */
    if(buf[3]=='-')
    {
        flag=0;
        chtemp=buf;
        while(flag==0)
        {
            chtemp=strchr(chtemp,'\n');
			//            ZDMSG(("%d, chtemp-buf=%d recvlen=%d\n", tid, chtemp-buf,recvlen));
            if(chtemp==NULL || chtemp-buf==recvlen-1)
            {
                //ZDMSG(("%d, AGAIN\n", tid ));
                if((recvlen=read_line(remotesocket,buf,MAXBUFSIZE-1))<0)
                {
                   // ZERROR(("%d, Error in retrieveing message: %s\n", tid, strerror(errno)));
                    close(remotesocket);
                    return MTDA_ERROR;
                }
				
				//              ZINFO(("%d, FTP: %s\n", tid, buf));
                chtemp=buf;
            }
            else
                chtemp++;
            if(strncmp(chtemp,actual_reply,4)==0)
            {
                flag=1;
                break;
            }
        }
    }
	
    //ZDMSG(("%d, GET_FTP_RESPONSE ABOUT TO EXIT\n", tid ));
    if(strncmp(expected_reply,actual_reply,3)==0)
        return MTDA_SUCCESS;
    else
        return MTDA_ERROR;
}

/**
* <b>FUNCTION: </b>
*
* read_char(int socket, char *c)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int socket, char *c
* 
* @return
*   recvlen if success. Otherwise -1
*/
int read_char(int socket, char *c)
{
	struct timeval timeout;
	long recvlen=0;
	fd_set fds;
	
	timeout.tv_sec = MTDA_CONNECTION_TIMEOUT;
	timeout.tv_usec = 0;
	
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	fcntl(socket, F_SETFL, O_NONBLOCK);
	recvlen = select(socket+1, &fds, NULL, NULL, &timeout);
	
	if(recvlen == -1) {
		fprintf(stderr, "\n\e[1mConnection timeout 1 [%ld]\e[0m\n", timeout.tv_sec);
		return -1;
	}
	
	recvlen = recv(socket, c, 1, 0);
	
	if(recvlen == -1) {
		fprintf(stderr, "\n\e[1mConnection timeout 1 [%ld]\e[0m\n", timeout.tv_sec);
		FD_CLR(socket, &fds);
		return -1;
	}

	
	return recvlen;
}

/**
* <b>FUNCTION: </b>
*
* read_line(int socket, char *buf, int bufsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int socket, char *buf, int bufsize
* 
* @return
*   buflen if success. Otherwise -1
*/
int read_line(int socket, char *buf, int bufsize)
{
	long buflen;
	long len;
	char c;
	
	buf[0] = '\0';
	buflen=0;
	
	do {
		len = read_char(socket, &c);
		if (len == -1){
			fprintf(stderr ,"len = -1 in read_char\n");
			return (-1);
		}
		if (len == 0){
			fprintf(stderr, "len = 0 in read_char\n");
			break;
		}
		
		buf[buflen++] = c;
		if (buflen == bufsize) {
			break;
		}
	} while (c != '\n');
	
	buf[buflen] = '\0';
	fprintf(stderr, "<<< %s", buf);
	return buflen;
}

/**
* <b>FUNCTION: </b>
*
* receive_data(int socket, char *buf, int bufsize, int flags)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int socket, char *buf, int bufsize, int flags
* 
* @return
*   recvlen if success. Otherwise -1
*/
int receive_data(int socket, char *buf, int bufsize, int flags)
{
	int recvlen=-1;
	
	struct timeval timeout;
	fd_set fds;
	
	timeout.tv_sec = MTDA_CONNECTION_TIMEOUT;
	timeout.tv_usec = 0;
	
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	fcntl(socket, F_SETFL, O_NONBLOCK);
	select(socket+1, &fds, NULL, NULL, &timeout);
	
	recvlen = recv(socket, buf, bufsize-1, 0);
	if( recvlen == -1 ) {
		fprintf(stderr, "\n\e[1mConnection timeout 1 [%ld]\e[0m\n", timeout.tv_sec);
		return -1;
	}
	
	return recvlen;

}

/**
* <b>FUNCTION: </b>
*
* get_connection_http_proxy(int tid, dload_info_t dload_info)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int tid, dload_info_t dload_info
* 
* @return
*   remotesocket if success. Otherwise -1
*/
int get_connection_http_proxy(int tid, dload_info_t dload_info)
{
	int remotesocket;
	struct sockaddr_in server;
	char buf[MAXBUFSIZE];
	int recvlen;
	
	/* 
	* Oop! no malloc please
	* char *http_auth,*http_auth_base64; 
	*/
	char http_auth[256], http_auth_base64[256];
	
	//ZDMSG(("%d, Opening Socket...\n", tid));
	remotesocket=socket(PF_INET,SOCK_STREAM,0);
	if(remotesocket<0)
	{
		//ZERROR(("%d, Could not open socket: %s\n", tid, strerror(errno)));
		mtda_errno=MTDA_SOCKET;
		return(-1);
	}
	//ZDMSG(("%d, Socket Opened, Connecting to HTTP proxy\n", tid));
	
	server.sin_family=AF_INET;
	inet_aton(dload_info.proxy_ip,&server.sin_addr);
	server.sin_port=htons(dload_info.proxy_port);
	
	//ZDMSG(("%d, Connecting to HTTP proxy....\n", tid));
	
	if(connect_timeout(tid, remotesocket,(struct sockaddr *)&server,sizeof(server),
					   MTDA_CONNECTION_TIMEOUT)<0)
	{
		close(remotesocket);
		mtda_errno=MTDA_CONNECT;
		return(-1);
	}
	
	 //ZDMSG(("%d, Connection established. Sending CONNECT command...\n", tid));
	
	if(dload_info.proxy_user!=NULL || dload_info.proxy_pass!=NULL)
	{
		//ZDMSG(("%d, Setting up HTTP Authenticatiosn %s %s\n", tid,
			//dload_info.proxy_user, dload_info.proxy_pass));
		
			/* Oop! no malloc please
			* http_auth=(char *)xmalloc(dload_info.proxy_user==NULL?0:strlen(dload_info.proxy_user) +
			* dload_info.proxy_pass==NULL?0:strlen(dload_info.proxy_pass) +
			* 2);
		*/
		snprintf(http_auth, sizeof(http_auth), "%s:%s",dload_info.proxy_user,dload_info.proxy_pass);
		/* http_auth_base64=(char *)xmalloc(strlen(http_auth)/3*4 +1); */
		base64_encode(http_auth,http_auth_base64,strlen(http_auth));
		snprintf(buf, sizeof(buf), "CONNECT %s:%d HTTP/1.0\r\nProxy-Authorization: Basic %s\r\n\r\n",
			dload_info.server, dload_info.server_port, http_auth_base64);
	}
	else
		snprintf(buf, sizeof(buf), "CONNECT %s:%d HTTP/1.0\r\n\r\n",dload_info.server,dload_info.server_port);
	if(send(remotesocket,buf,strlen(buf),0)<(int)strlen(buf))
	{
		//ZERROR(("%d, Error in sending CONNECT command: %s\n", tid, strerror(errno)));
		close(remotesocket);
		mtda_errno=MTDA_CONNECT;
		return(MTDA_CONNECT);
	}
	 //ZDMSG(("%d, CONNECT command sent\n", tid));
	
	if((recvlen=receive_data(remotesocket,buf,MAXBUFSIZE-1,0))<0)
	{
		//ZERROR(("%d, Error in recieving data from proxy: %s\n", tid, strerror(errno)));
		close(remotesocket);
		mtda_errno=MTDA_CONNECT;
		return(MTDA_CONNECT);
	}
	buf[recvlen]='\0';
	
	if(recvlen>9 && strncmp(buf+9,"200",3)!=0)
	{
		//ZERROR(("%d, Connection not established: %s\n", tid, strerror(errno)));
		close(remotesocket);
		mtda_errno=MTDA_CONNECT;
		return(MTDA_CONNECT);
	}
	//ZDMSG(("%d, CONNECTION ESTABLISHED: %d\n", tid, remotesocket));
	return(remotesocket);
}

/**
* <b>FUNCTION: </b>
*
* get_connection(int tid,dload_info_t dload_info,int logon)
*
* <b>DESCRIPTION: </b>
*
* @param
*   int tid,dload_info_t dload_info,int logon
* 
* @return
*   remotesocket if success. Otherwise -1
*/
int get_connection(int tid,dload_info_t dload_info,int logon)
{
	int remotesocket;
	struct sockaddr_in server;
	char buf[MAXBUFSIZE];

	if(dload_info.proxy_ip!=NULL)
	{
		remotesocket=get_connection_http_proxy(tid,dload_info);
		if(remotesocket<0)
		{
			//ZERROR((" %d, Error in connecting with HTTP proxy: %s", tid, strerror(errno)));
			return(MTDA_CONNECT);
		}
	}
	else
	{
		 //ZDMSG(("%d, Opening Socket...\n", tid));
		remotesocket=socket(PF_INET,SOCK_STREAM,0);
		if(remotesocket<0)
		{
			//ZERROR(("%d, Could not open socket: %s\n", tid, strerror(errno)));
			mtda_errno=MTDA_SOCKET;
			return(MTDA_SOCKET);
		}
		server.sin_family=AF_INET;
		inet_aton(dload_info.server_ip,&server.sin_addr);
		server.sin_port=htons(dload_info.server_port);
		//ZDMSG(("%d, Attempting to connect to remote host now...\n", tid));
		if(connect_timeout(tid, remotesocket,(struct sockaddr *)&server,sizeof(server),
						   MTDA_CONNECTION_TIMEOUT)<0)
		{
			//ZERROR(("%d, Could not etablish conenction with remote host: %s", tid, strerror(errno)));
			close(remotesocket);
			mtda_errno=MTDA_CONNECT;
			return(MTDA_CONNECT);
		}
	}

	if(strcmp(dload_info.protocol,"ftp")==0 && logon>0)
	{
		/* TODO - Some FTP servers have *very* large welcome msgs */
		//        ZDMSG(("%d, waiting for welcome message\n", tid));
		//if(get_ftp_response(tid,remotesocket,"220")<0)
		//{
			close(remotesocket);
			mtda_errno=MTDA_FTP;
			return(MTDA_CONNECT);
		//}

		snprintf(buf, sizeof(buf), "USER %s\r\n",dload_info.ftp_user);
		   // ZINFO(("%d, Sending ftp username: %s\n", tid, buf));

		if(send(remotesocket,buf,strlen(buf),0)<(int)strlen(buf))
		{
			//ZERROR(("%d, Error in sending username: %s\n", tid, strerror(errno)));
			close(remotesocket);
			mtda_errno=MTDA_CONNECT;
			return(MTDA_CONNECT);
		}

		        //ZDMSG(("%d, Waiting for response from FTP server\n", tid));
		//if(get_ftp_response(tid,remotesocket,"331")<0)
		//{
			//ZERROR(("%d, Unexpeced resonse from FTP server\n", tid));
			close(remotesocket);
			return(MTDA_CONNECT);
		//}

		  // ZINFO(("%d, Sending FTP password\n", tid));

		if(dload_info.ftp_pass)
		{
			snprintf(buf, sizeof(buf), "PASS %s\n",dload_info.ftp_pass);
		}
		else
		{
			snprintf(buf, sizeof(buf), "PASS \n");
		}

		if(send(remotesocket,buf,strlen(buf),0)<(int)strlen(buf))
		{
			//ZERROR(("%d, Error in sending FTP password\n", tid));
			close(remotesocket);
			return(MTDA_CONNECT);
		}

		    //ZDMSG(("%d, waiting for response from server\n", tid));
		//if(get_ftp_response(tid,remotesocket,"230")<0)
		//{
			//ZERROR(("%d, Unexpected response from FTP server\n", tid));
			close(remotesocket);
			return(MTDA_CONNECT);
		//}
		//        ZINFO(("%d, User logged on to FTP server....\n", tid));
	}

	return(remotesocket);
}

/**
* <b>FUNCTION: </b>
*
* get_file_info_http(dload_info_t *dload_info, char *buf_hdr, int bufsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   dload_info_t *dload_info, char *buf_hdr, int bufsize
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int get_file_info_http(dload_info_t *dload_info, char *buf_hdr, int bufsize)
{
    int initsock=-1;
    char *buf;
    char *chtemp=NULL;
        
    buf = buf_hdr;
    initsock=get_connection(-1,*dload_info,0);

    if(initsock<0) {
        //ZERROR(("-1, Could not connect to remote server. Aborting...\n"));
        return MTDA_ERROR;
    }

    if(dload_info->data) {
        snprintf(buf, sizeof(buf), "HEAD %s HTTP/1.1\r\nHost: %s\r\nRange: bytes=0-\r\n\r\n",
            dload_info->data, dload_info->server);
    } else {
        snprintf(buf, sizeof(buf), "HEAD HTTP/1.1\r\nHost: %s\r\nRange: bytes=0-\r\n\r\n", dload_info->server);
    }
    
    //ZDMSG(("-1, Sending HEAD request: %s\n",buf));
    if(send(initsock,buf,strlen(buf),0)< (int)strlen(buf)) {
		//ZERROR(("-1, Error in sending HEAD request: %s\n",strerror(errno)));
        close(initsock);
        return MTDA_ERROR;
    }


    { /* current stable: but its really a silly code */
        int i;
        char rc;
        int recvlen;
        
        memset(buf, 0, bufsize);
        for(i=0;i<MAXBUFSIZE;i++)
        {
            recvlen = read_char(initsock, &rc);
            if(recvlen==-1)
            {
                return MTDA_ERROR;
            }

            if(recvlen==0)
                break;
            
            buf[i]=rc;
        }
        
      
    }

    //ZDMSG(("-1, Recvd from server\n%s\n",buf));
    if(strncmp("HTTP/1.1",buf,strlen("HTTP/1.1"))==0) {
        //ZINFO(("-1, Server is HTTP/1.1 Compliant\n"));
    } else {
        //ZINFO(("-1, Server is HTTP/1.0 Compliant. Closing Connection\n"));
        close(initsock);
        initsock=-1;
    }

    //ZDMSG(("-1, Determinig file size...\n"));
    chtemp=strstr(buf,"Content-Length: ");
    if(chtemp==NULL) {
        //ZINFO(("-1, Cannot determine file size. Single threaded download\n"));
        dload_info->total_threads=1;
        return initsock;
    }

    chtemp=strchr(chtemp,':');
    if(chtemp==NULL) {
        dload_info->total_threads=1;
        return initsock;
    }
    dload_info->file_length=atoi(chtemp+2);

    if(strstr(buf,"206 Partial content")==0) {
        //ZINFO(("-1, Server can handle partial content\n"));
		printf("Server can handle partial content\n");
    }
    
    /*  
    TODO: Connection may be preserved
    return initsock;
    */
    
    close(initsock);
    return MTDA_SUCCESS;
}

/**
* <b>FUNCTION: </b>
*
* get_file_info_ftp(dload_info_t *dload_info, char *buf_hdr)
*
* <b>DESCRIPTION: </b>
*
* @param
*   dload_info_t *dload_info, char *buf_hdr
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int get_file_info_ftp(dload_info_t *dload_info, char *buf_hdr)
{

    int initsock,recvlen;
    char *buf,*chtemp;
    initsock=get_connection(-1,*dload_info,1);
	
    buf=buf_hdr;
    if(initsock<0) {
        return MTDA_ERROR;
    }
	
    snprintf(buf, sizeof(buf), "SIZE %s\n", dload_info->data);
	
    if(send(initsock,buf,strlen(buf),0)<(int)strlen(buf)) {
        close(initsock);
        return MTDA_ERROR;
    }
	

    if((recvlen=read_line(initsock,buf,MAXBUFSIZE))<0) {
        close(initsock);
        return MTDA_ERROR;
    }
    buf[recvlen]='\0';
    if(strncmp(buf,"213",3)!=0) {
        dload_info->total_threads=1;
    } else {
        chtemp=strchr(buf,' ');
        if(chtemp==NULL) {
            dload_info->total_threads=1;
        } else {
            dload_info->file_length=atoi(chtemp+1);
        }
    }
    
    close(initsock);
	
    return MTDA_SUCCESS;
}

/**
* <b>FUNCTION: </b>
*
* get_file_info(dload_info_t *dload_info, char *buf_hdr, int bufsize)
*
* <b>DESCRIPTION: </b>
*
* @param
*   dload_info_t *dload_info, char *buf_hdr, int bufsize
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int get_file_info(dload_info_t *dload_info, char *buf_hdr, int bufsize)
{
    if(strcmp(dload_info->protocol, "http")==0)
	{	
		
        return get_file_info_http(dload_info, buf_hdr, bufsize);
	}
    if(strcmp(dload_info->protocol, "ftp")==0)
	{	
		
        return get_file_info_ftp(dload_info, buf_hdr);
	}
    //ZERROR(("-1, protocol not implemented yet\n", dload_info->protocol));
    return MTDA_ERROR;
}

/**
* <b>FUNCTION: </b>
*
* mtda_get_header(mtda_dload_info __dload_info, char *buf_hdr)
*
* <b>DESCRIPTION: </b>
*
* @param
*   mtda_dload_info __dload_info, char *buf_hdr
* 
* @return
*   MTDA_SUCCESS if success. Otherwise MTDA_ERROR
*/
int mtda_get_header(mtda_dload_info __dload_info, char *buf_hdr)
{
    struct hostent *server_info;
    char *hdr = buf_hdr;
	
    memset(&dload_info, 0, sizeof(dload_info));
    dload_info.url = __dload_info.url;
    dload_info.proxy_port = __dload_info.proxy_port;
    dload_info.total_threads = __dload_info.total_threads;
    //ZDMSG(("Starting....\n"));
	/* url parsing */
    if(spliturl(&dload_info) < 0)
    {
        printf( "[%s:%d] %s Invalid URL\n\n",__FILE__, __LINE__, dload_info.url);
        return(MTDA_INVALID_URL);
    }

    /* TODO - parse the URL/ command line for ports */
    if((strcmp(dload_info.protocol,"http") == 0) && (dload_info.server_port == -1))
        dload_info.server_port = 80;
    if((strcmp(dload_info.protocol,"ftp") == 0) && (dload_info.server_port == -1))
        dload_info.server_port = 21;
	if(dload_info.server == NULL)
    {
        printf( "\ndload_info.server == NULL[%s:%d] %s Invalid URL\n\n",__FILE__, __LINE__, dload_info.url); 
        return(MTDA_INVALID_URL); 
    }
	
    if(dload_info.proxy_ip == NULL)
    {
        server_info = gethostbyname(dload_info.server);
        if(server_info == NULL)
        {
            //ZERROR(("Error in lookup for server: %s\n", strerror(errno)));
            return(MTDA_ERROR);
        }
		
        snprintf(dload_info.server_ip, sizeof(dload_info.server_ip), "%d.%d.%d.%d",
			(unsigned char)server_info->h_addr_list[0][0],
			(unsigned char)server_info->h_addr_list[0][1],
			(unsigned char)server_info->h_addr_list[0][2],
			(unsigned char)server_info->h_addr_list[0][3]);
    }
	
    dump_url(&dload_info);
	
    memset(hdr, 0, MAXBUFSIZE);
  
    if(get_file_info(&dload_info, hdr, MAXBUFSIZE - 1) < 0)
           {
               printf( "[%s:%d]Error in Contacting server.... Aborting Download\n\n",__FILE__, __LINE__);
               return(MTDA_ERROR);
           }

         printf("url=%s\n   protocol=%s, user=%s, pass=%s,"
            "   server=%s, server_port=%d, path=%s, file=%s\n"
            "   file_length=%lld, total_threads=%d\n",
            dload_info.url, dload_info.protocol, dload_info.ftp_user, dload_info.ftp_pass,
            dload_info.server, dload_info.server_port, dload_info.path, dload_info.file,
            dload_info.file_length, dload_info.total_threads);
    return(MTDA_SUCCESS);
}

/**
* <b>FUNCTION: </b>
*
* parse_day(const unsigned char **date_p, unsigned char *end)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const unsigned char **date_p, unsigned char *end
* 
* @return
*   day if success. Otherwise 32
*/
int parse_day(const unsigned char **date_p, unsigned char *end)
{
	const unsigned char *date = *date_p;
	int day;
	
	if ((end && date >= end) || !isdigit(*date))
		return 32;
	day = *date++ - '0';
	
	if ((!end || date < end) && isdigit(*date)) {
		day = day * 10 + *date++ - '0';
	}
	
	*date_p = date;
	return day;
}

/**
* <b>FUNCTION: </b>
*
* my_timegm(struct tm *tm)
*
* <b>DESCRIPTION: </b>
*
* @param
*   struct tm *tm
* 
* @return
*   0 or t
*/
static time_t my_timegm(struct tm *tm)
{
	time_t t = 0;
	
#if 0
	printf("[%s:%d] tm_mday [%d]\n", __FILE__, __LINE__, tm->tm_mon);
	printf("tm_year [%d]\n", tm->tm_year);
	printf("tm_mday [%d]\n", tm->tm_mday);
	
	printf("tm_hour [%d]\n", tm->tm_hour);
	printf("tm_min [%d]\n", tm->tm_min);
	printf("tm_sec [%d]\n", tm->tm_sec);
#endif
	
	/* Okay, the next part of the code is somehow problematic. Now, we use
	* own code for calculating the number of seconds from 1.1.1970,
	* brought here by SC from w3m. I don't like it a lot, but it's 100%
	* portable, it's faster and it's shorter. --pasky */
#if 0
	
#ifdef HAVE_TIMEGM
	t = timegm(tm);
#else
	/* Since mktime thinks we have localtime, we need a wrapper
	* to handle GMT. */
	/* FIXME: It was reported that it doesn't work somewhere :/. */
	{
		unsigned char *tz = getenv("TZ");
		
		if (tz && *tz) {
			/* Temporary disable timezone in-place. */
			unsigned char tmp = *tz;
			
			*tz = '\0';
			tzset();
			
			t = mktime(tm);
			
			*tz = tmp;
			tzset();
			
		} else {
			/* Already GMT, cool! */
			t = mktime(tm);
		}
	}
#endif
	
#else
	/* Following code was borrowed from w3m, and its developers probably
	* borrowed it from somewhere else as well, altough they didn't bother
	* to mention that. */ /* Actually, same code appears to be in lynx as
	* well.. oh well. :) */
	/* See README.timegm for explanation about how this works. */
	tm->tm_mon -= 2;
	if (tm->tm_mon < 0) {
		tm->tm_mon += 12;
		tm->tm_year--;
	}
	tm->tm_mon *= 153; tm->tm_mon += 2;
	tm->tm_year -= 68;
	
	tm->tm_mday += tm->tm_year * 1461 / 4;
	tm->tm_mday += ((tm->tm_mon / 5) - 672);
	
	t = ((tm->tm_mday * 60 * 60 * 24) +
		(tm->tm_hour * 60 * 60) +
		(tm->tm_min * 60) +
		tm->tm_sec);
#endif
	
	if (t == (time_t) -1) return 0;
	
#if 0
	printf("[%s:%d] GET [%ld]\n", __FILE__, __LINE__, t);
	{
		unsigned long t0;
		struct timeval nowTime;
		gettimeofday(&nowTime, NULL);
		t0 = nowTime.tv_sec;
		printf("LOC [%ld]\n", t0);
	}
#endif
	
	return t;
}

/**
* <b>FUNCTION: </b>
*
* month2num(const unsigned char *str)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const unsigned char *str
* 
* @return
*   >= 0 if success. Otherwise -1
*/
int month2num(const unsigned char *str)
{
	unsigned char month[3] = { str[0]|32, str[1]|32, str[2]|32 };
	
	switch (month[0]) {
	case 'j': /* jan, jun, jul */
		if (month[1] == 'a') {
			if (month[2] == 'n') return 0; /* jan */
			return -1;
		}
		if (month[1] == 'u') {
			if (month[2] == 'n') return 5; /* jun */
			if (month[2] == 'l') return 6; /* jul */
		}
		return -1;
	case 'm': /* mar, may */
		if (month[1] == 'a') {
			if (month[2] == 'r') return 2; /* mar */
			if (month[2] == 'y') return 4; /* may */
		}
		return -1;
	case 'a': /* apr, aug */
		if (month[1] == 'p') {
			if (month[2] == 'r') return 3; /* apr */
			return -1;
		}
		if (month[1] == 'u' && month[2] == 'g') return 7; /* aug */
		return -1;
	case 's':
		if (month[1] == 'e' && month[2] == 'p') return 8; /* sep */
		return -1;
	case 'o':
		if (month[1] == 'c' && month[2] == 't') return 9; /* oct */
		return -1;
	case 'n':
		if (month[1] == 'o' && month[2] == 'v') return 10; /* nov */
		return -1;
	case 'd':
		if (month[1] == 'e' && month[2] == 'c') return 11; /* dec */
		return -1;
	case 'f':
		if (month[1] == 'e' && month[2] == 'b') return 1; /* feb */
		return -1;
	default:
		return -1;
	}
}

/**
* <b>FUNCTION: </b>
*
* parse_month(const unsigned char **buf, unsigned char *end)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const unsigned char **buf, unsigned char *end
* 
* @return
*   monthnum if success. Otherwise -1
*/
int parse_month(const unsigned char **buf, unsigned char *end)
{
	const unsigned char *month = *buf;
	int monthnum;
	
	/* Check that the buffer has atleast 3 chars. */
	if ((end && month + 2 > end)
		|| !month[0] || !month[1] || !month[2])
		return -1;
	
	monthnum = month2num(month);
	
	if (monthnum != -1)
		*buf += 3;
	
	return monthnum;
}

/**
* <b>FUNCTION: </b>
*
* parse_year(const unsigned char **date_p, unsigned char *end)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const unsigned char **date_p, unsigned char *end
* 
* @return
*   year if success. Otherwise -1
*/
int parse_year(const unsigned char **date_p, unsigned char *end)
{
	const unsigned char *date = *date_p;
	int year;
	
	if ((end && date + 1 >= end)
		|| !isdigit(date[0])
		|| !isdigit(date[1]))
		return -1;
	
	year = (date[0] - '0') * 10 + date[1] - '0';
	
	if ((!end || date + 3 < end)
		&& isdigit(date[2])
		&& isdigit(date[3])) {
		/* Four digits date */
		year = year * 10 + date[2] - '0';
		year = year * 10 + date[3] - '0' - 1900;
		date += 4;
		
	} else if (year < 70) {
	/* Assuming the epoch starting at 1.1.1970 so it's already next
		* century. --wget */
		year += 100;
		date += 2;
	}
	
	*date_p = date;
	return year;
}

/**
* <b>FUNCTION: </b>
*
* parse_time(const unsigned char **time, struct tm *tm, unsigned char *end)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const unsigned char **time, struct tm *tm, unsigned char *end
* 
* @return
*   0 if success. Otherwise 1
*/
int parse_time(const unsigned char **time, struct tm *tm, unsigned char *end)
{
	unsigned char h1, h2, m1, m2;
	const unsigned char *date = *time;
	
#define check_time(tm) \
	((tm)->tm_hour <= 23 && (tm)->tm_min <= 59 && (tm)->tm_sec <= 59)
	
	/* Eat HH:MM */
	if (end && date + 5 > end)
		return 0;
	
	h1 = *date++; if (!isdigit(h1)) return 0;
	h2 = *date++; if (!isdigit(h2)) return 0;
	
	if (*date++ != ':') return 0;
	
	m1 = *date++; if (!isdigit(m1)) return 0;
	m2 = *date++; if (!isdigit(m2)) return 0;
	
	tm->tm_hour = (h1 - '0') * 10 + h2 - '0';
	tm->tm_min  = (m1 - '0') * 10 + m2 - '0';
	
	/* Eat :SS or [PA]M or nothing */
	if (end && date + 2 >= end) {
		*time = date;
		return check_time(tm);
	}
	
	if (*date == ':') {
		unsigned char s1, s2;
		
		date++;
		
		if (end && date + 2 >= end)
			return 0;
		
		s1 = *date++; if (!isdigit(s1)) return 0;
		s2 = *date++; if (!isdigit(s2)) return 0;
		
		tm->tm_sec = (s1 - '0') * 10 + s2 - '0';
		
	} else if (*date == 'A' || *date == 'P') {
	/* Adjust hour from AM/PM. The sequence goes 11:00AM, 12:00PM,
		* 01:00PM ... 11:00PM, 12:00AM, 01:00AM. --wget */
		if (tm->tm_hour == 12)
			tm->tm_hour = 0;
		
		if (*date++ == 'P')
			tm->tm_hour += 12;
		
		if (*date++ != 'M')
			return 0;
	}
	
	*time = date;
	
	return check_time(tm);
}

/**
* <b>FUNCTION: </b>
*
* parse_date(unsigned char **date_pos, unsigned char *end, int
* update_pos, int skip_week_day)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char **date_pos, unsigned char *end, int
*   update_pos, int skip_week_day
* 
* @return
*   0 if success. Otherwise 1
*/

//&pd, pd+l, 0, 1 ///// pd = [Tue, 23 Sep 2008 02:52:59 GMT]
time_t parse_date(unsigned char **date_pos, unsigned char *end,
				  int update_pos, int skip_week_day)
{
#define skip_time_sep(date, end) \
	do 	\
	{ \
	const unsigned char *start = (date); \
	while ((!(end) || (date) < (end)) \
	&& (*(date) == ' ' || *(date) == '-')) \
	(date)++; \
	if (date == start) \
	{ \
		return 0; \
	}\
	} while (0)
	
	struct tm tm;
	const unsigned char *date = (const unsigned char *) *date_pos;
	
	if (!date) 
	{
		
		return 0;
	}

	if (skip_week_day)
	{
		/* Skip day-of-week */
	
		for (; (!end || date < end) && *date != ' '; date++)
			if (!*date) 
			{
				
				return 0;
			}
			
			/* As pasky said who cares if we allow '-'s here? */
			skip_time_sep(date, end);
		
	}

	if (isdigit(*date))
	{
		/* RFC 1036 / RFC 1123 */
		/* Eat day */
		
		/* date++; */
		tm.tm_mday = parse_day(&date, end);
		if (tm.tm_mday > 31) 
		{
			return 0;
		}
		
		skip_time_sep(date, end);
		
		/* Eat month */
		tm.tm_mon = parse_month(&date, end);
		if (tm.tm_mon < 0) 
		{
			return 0;
		}
		
		skip_time_sep(date, end);
		
		/* Eat year */
		tm.tm_year = parse_year(&date, end);
		if (tm.tm_year < 0) 
		{
			return 0;
		}
		
		skip_time_sep(date, end);
		
		/* Eat time */
		
		if (!parse_time(&date, &tm, end)) 
		{
		
			return 0;
		}
		
	} else {
		/* ANSI C's asctime() format */
		/* Eat month */
	
		tm.tm_mon = parse_month(&date, end);
		if (tm.tm_mon < 0) return 0;
		
		/* I know, we shouldn't allow '-', but who cares ;). --pasky */
		skip_time_sep(date, end);
		
		/* Eat day */
		tm.tm_mday = parse_day(&date, end);
		if (tm.tm_mday > 31) return 0;
		
		skip_time_sep(date, end);
		
		/* Eat time */
		if (!parse_time(&date, &tm, end)) 
		{
			
			return 0;
		}
		
		skip_time_sep(date, end);
		
		/* Eat year */
		tm.tm_year = parse_year(&date, end);
		if (tm.tm_year < 0) return 0;
	}
#undef skip_time_sep
	if (update_pos)
		
	{
		*date_pos = (unsigned char *) date;
	}
		
	return (time_t) my_timegm(&tm);
}

/**
* <b>FUNCTION: </b>
*
* parse_http_header(unsigned char *head, unsigned char *item,
* unsigned char **ptr, int *len)
*
* <b>DESCRIPTION: </b>
*
* @param
*   unsigned char *head, unsigned char *item,
*   unsigned char **ptr, int *len
* 
* @return
*   0 if success. Otherwise -1
*/

//buf, Param, &pd, &l
int parse_http_header(unsigned char *head, unsigned char *item,
					  unsigned char **ptr, int *len)
{
	unsigned char *i, *f, *g, *h;
	
	if (!head) 
	{	
	
		return -1;
	}
	
	if (!item) 
	{
		
		return -1;
	}
	h = NULL;
	
	for (f = head; *f; f++) 
	{
		
		if (*f != 10) 	continue;
		f++;
		
		/* compare item */
		
		for (i = item; *i && *f; i++, f++) 
		{
			
			if (upcase(*i) != upcase(*f))
			{
				printf("GOTO\n");
				goto cont;
			}
		}
		
		if (!*f) break;
		
		if (f[0] == ':') { /* found item */
			while (f[1] == ' ') f++;
			for (g = ++f; *g >= ' '; g++) ;
			while (g > f && g[-1] == ' ') g--;
			
			*ptr = f  ;
			*len = (int)(g-f);
		
			return 0;
		}
cont:;
	
	 f--;

	}

	return -1;
}

/**
* <b>FUNCTION: </b>
*
* SyncTime(const char *playlistUrl, RMint8 m_TimeZone)
*
* <b>DESCRIPTION: </b>
*
* @param
*   const char *playlistUrl, RMint8 m_TimeZone
* 
* @return
*   0 if success. Otherwise -1
*/
int CTimer::SyncTime(const char *playlistUrl, RMint8 m_TimeZone)
{
//	ZDMSG(("=========================================XXX: SyncTime()\n"));
	unsigned char *pd=NULL;
	unsigned char buf[MAXBUFSIZE];
	unsigned char *Param= (unsigned char*)"Date";
	int l=0;
	time_t ti=0;
	struct timeval tv;
	struct SDateTime stDt;

    char http[30] = "225.225.225.225";
    if (GetHttpUrlFromPlaylistUrl(playlistUrl, http) != 1)
    {
        printf("[%s:%d] Cannot find ip from [%s]\n", 
               __FILE__, __LINE__, playlistUrl);
        return -1;
    }
        
	
	mtda_dload_info d=
	{
		NULL, //"http://192.168.1.23",		/* url */
		NULL,						/* ftp username */ 
		NULL,						/* ftp password */
		NULL,						/* proxy IP */ 
		-1,							/* proxy port */ 
		NULL,						/* proxy user */ 
		NULL,						/* proxy pass */ 
		1							/* number of file parts */ 
	};
		
    d.url = http;
    printf("****************************\n");
    printf("[%s:%d] SyncTime(%s)\n", 
           __FILE__, __LINE__, http);
    printf("****************************\n");

	/* get http header */
	if(mtda_get_header(d, (char*)buf)!=MTDA_SUCCESS)
	{
		return -1;
	}

	/* Parse header to get date string */
	/* Tue, 08 May 2007 02:48:38 GMT */
	
	if(parse_http_header(buf, Param, &pd, &l)==-1)
	{
		return -1;
	}
	
	/* get date time from date string */

	ti = parse_date(&pd, pd+l, 0, 1);
	if(ti==0) 		return -1;
	
	memset(&stDt, 0, sizeof(stDt));
	tv.tv_sec = ti;
	tv.tv_usec = 0;

	if(TimeZoneTable[m_TimeZone].status == MINUS)
		tv.tv_sec = tv.tv_sec - (TimeZoneTable[m_TimeZone].hour*60 + TimeZoneTable[m_TimeZone].minute)*60;
	else	
		tv.tv_sec = tv.tv_sec + (TimeZoneTable[m_TimeZone].hour*60 + TimeZoneTable[m_TimeZone].minute)*60;

	if (settimeofday(&tv, NULL) != 0) 
	{
		console_err ("Set ting using settimeofday is failed. Try again\n");
		if (settimeofday(&tv, NULL) != 0)
			return -1;
	}

	return 0;
}
//end Auto time

/*================================= end of file =================================*/
