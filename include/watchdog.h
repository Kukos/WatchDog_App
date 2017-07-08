#ifndef WATCHDOG_H
#define WATCHDOG_H

/*
    Simple Library to "talk" with HW WatchDog

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com
    LICENCE: GPL3.0
*/

#include <linux/watchdog.h>

typedef int watchdog_t;

/*
    Open Watchdog device

    PARAMS
    @IN dev - path to WD device

    RETURN
    -1 iff failure
    WD descriptior iff success
*/
watchdog_t wd_open(const char *dev);

/*
    Close Watchdog device

    PARAMS
    @IN wd - wd descriptor

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int wd_close(watchdog_t wd);

/*
    Get Timeout from WatchDog

    PARAMS
    @IN wd - watchdog descriptor
    @OUT timeout - WD timeout

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_get_timeout(watchdog_t wd, unsigned int *timeout);

/*
    Set Watchdog Timeout

    PARAMS
    @IN wd - watchdog descriptor
    @IN timeout - new WD timeout

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_set_timeout(watchdog_t wd, unsigned int timeout);

/*
    Feed Watchdog

    PARAMS
    @IN wd - watchdog descriptor

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_keepalive(watchdog_t wd);

/*
    Get PRE Timeout from WatchDog

    PARAMS
    @IN wd - watchdog descriptor
    @OUT timeout - WD pre timeout

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_get_pretimeout(watchdog_t wd, unsigned int *timeout);

/*
    Set PRE Watchdog Timeout

    PARAMS
    @IN wd - watchdog descriptor
    @IN timeout - new WD pre timeout

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_set_pretimeout(watchdog_t wd, unsigned int timeout);

/*
    Get time left to reset system by WD

    PARAMS
    @IN wd - watchdog descriptor
    @OUT timeout - time left to reset

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_get_timeleft(watchdog_t wd, unsigned int *time);

/*
    Get Bootstatus from WatchDog

    PARAMS
    @IN wd - watchdog descriptor
    @OUT status - bootstatus

    RETURN
    0 iff success
    Non-zero iff failure

*/
int wd_get_bootstatus(watchdog_t wd, int *status);

/*
    Get Status from WatchDog

    PARAMS
    @IN wd - watchdog descriptor
    @OUT status - status

    RETURN
    0 iff success
    Non-zero iff failure

*/
int wd_get_status(watchdog_t wd, int *status);

/*
    Get Temperature in degrees fahrenheit from WatchDog

    PARAMS
    @IN wd - watchdog descriptor
    @OUT temp - temperatue

    RETURN
    0 iff success
    Non-zero iff failure

*/
int wd_get_temp(watchdog_t wd, int *temp);

/*
    Set Watchfog options

    AVAILABLE: (from <linux/watchdog.h>)

    #define	WDIOS_DISABLECARD	0x0001	 Turn off the watchdog timer
    #define	WDIOS_ENABLECARD	0x0002	 Turn on the watchdog timer
    #define	WDIOS_TEMPPANIC		0x0004	 Kernel panic on temperature trip

    PARAMS
    @IN wd - watchdog descriptor
    @IN option - new options

    RETURN
    0 iff success
    Non-zero iff failure

*/
int wd_set_options(watchdog_t wd, int options);

/*
    Decode given flag (set of bits) to human format (print string)

    PARAMS
    @IN flag - set of bits

    RETURN
    This is a void function
*/
void wd_print_decoded_flag(int flag);

/*
    Get WatchDog info

    PARAMS
    @IN wd - watchdog descriptor
    @OUT wd_info - watchdog info

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_get_info(watchdog_t wd, struct watchdog_info *wd_info);

/*
    Print WatchDog info

    PARAMS
    @IN info - pointer to WatchDog info

    RETURN
    This is a void function
*/
void wd_print_info(struct watchdog_info *wd_info);

#endif