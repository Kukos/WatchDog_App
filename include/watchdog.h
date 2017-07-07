#ifndef WATCHDOG_H
#define WATCHDOG_H

typedef int watchdog_t;

/*
    Open Watchdog device

    PARAMS
    NO PARAMS

    RETURN
    -1 iff failure
    WD descriptior iff success
*/
watchdog_t wd_open(void);

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
int wd_gettimeout(watchdog_t wd, unsigned int *timeout);

/*
    Set Watchdog Timeout

    PARAMS
    @IN wd - watchdog descriptor
    @IN timeout - new WD timeout

    RETURN
    0 iff success
    Non-zero iff failure
*/
int wd_settimeout(watchdog_t wd, unsigned int timeout);

#endif