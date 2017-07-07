#ifndef WATCHDOG_H
#define WATCHDOG_H

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

#endif