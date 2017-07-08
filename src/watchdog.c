#include <watchdog.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/watchdog.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <common.h>
#include <log.h>
#include <inttypes.h>

#define WD_CLOSE_MSG    "V"
#define WD_DEV          "/dev/watchdog"
#define WD_ERROR(fmt, err, ...) ERROR(fmt, err, ##__VA_ARGS__)
#define WD_LOG(fmt, ...)        LOG(fmt, ##__VA_ARGS__)
#define WD_TRACE(...)           TRACE(__VA_ARGS__)

void wd_print_info(struct watchdog_info *wd_info)
{
    WD_TRACE("");

    if (wd_info == NULL)
        return;

    (void)printf("WD INFO\n");
    (void)printf("WD Firmware:\t" "%" PRId32 "\n", wd_info->firmware_version);
    (void)printf("WD Identity:\t%s\n", (char *)wd_info->identity);
    wd_print_decoded_flag((int)wd_info->options);
}

void wd_print_decoded_flag(int flag)
{
    WD_TRACE("");

    (void)printf("WD FLAGS\n");

    if (flag == WDIOF_UNKNOWN)
    {
        (void)printf("\tUnknown flag error\n");
        return;
    }

    if (GET_FLAG(flag, WDIOF_OVERHEAT))
        (void)printf("\tReset due to CPU overheat\n");

    if (GET_FLAG(flag, WDIOF_FANFAULT))
        (void)printf("\tFan failed\n");

    if (GET_FLAG(flag, WDIOF_EXTERN1))
        (void)printf("\tExternal relay 1\n");

    if (GET_FLAG(flag, WDIOF_EXTERN2))
        (void)printf("\tExternal relay 2\n");

    if (GET_FLAG(flag, WDIOF_POWERUNDER))
        (void)printf("\tPower bad/power fault\n");

    if (GET_FLAG(flag, WDIOF_CARDRESET))
        (void)printf("\tCard previously reset the CPU\n");

    if (GET_FLAG(flag, WDIOF_POWEROVER))
        (void)printf("\tPower over voltage\n");

    if (GET_FLAG(flag, WDIOF_SETTIMEOUT))
        (void)printf("\tSet timeout (in seconds)\n");

    if (GET_FLAG(flag, WDIOF_MAGICCLOSE))
        (void)printf("\tSupports magic close char\n");

    if (GET_FLAG(flag, WDIOF_PRETIMEOUT))
        (void)printf("\tPretimeout (in seconds), get/set\n");

    if (GET_FLAG(flag, WDIOF_ALARMONLY))
        (void)printf("\tWatchdog triggers external alarm not a reboot\n");

    if (GET_FLAG(flag, WDIOF_KEEPALIVEPING))
        (void)printf("\tKeep alive ping reply\n");

}

watchdog_t wd_open(const char *dev)
{
    char *wd;
    int fd;

    WD_TRACE("");

    dev ? (wd = (char *)dev) : (wd = (char *)WD_DEV);

    fd = open(wd, O_RDWR);
    if (fd == -1)
        WD_ERROR("Cannot open %s device\n", -1, wd);

    return fd;
}

int wd_close(watchdog_t wd)
{
    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (write(wd, WD_CLOSE_MSG, strlen(WD_CLOSE_MSG)) == -1)
        WD_ERROR("Cannot close Watchdog\n", 1, "");

    if (close(wd) == -1)
        WD_ERROR("Cannot close Watchdog\n", 1, "");

    return 0;
}

int wd_get_timeout(watchdog_t wd, unsigned int *timeout)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (timeout == NULL)
        ERROR("timeout == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETTIMEOUT, timeout);
    if (ret)
        WD_ERROR("Cannot get Watchdog timeout\n", ret, "");

    return 0;
}

int wd_set_timeout(watchdog_t wd, unsigned int timeout)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    ret = ioctl(wd, WDIOC_SETTIMEOUT, &timeout);
    if (ret)
        WD_ERROR("Cannot set Watchdog timeout\n", ret, "");

    return 0;
}

int wd_keepalive(watchdog_t wd)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    ret = ioctl(wd, WDIOC_KEEPALIVE, NULL);
    if (ret)
        WD_ERROR("Cannot feed watchdog\n", ret, "");

    return 0;
}

int wd_get_pretimeout(watchdog_t wd, unsigned int *timeout)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (timeout == NULL)
        ERROR("Timeout == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETPRETIMEOUT, timeout);
    if (ret)
        WD_ERROR("Cannot get Watchdog pretimeout\n", ret, "");

    return 0;
}

int wd_set_pretimeout(watchdog_t wd, unsigned int timeout)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    ret = ioctl(wd, WDIOC_SETPRETIMEOUT, &timeout);
    if (ret)
        WD_ERROR("Cannot set Watchdog pretimeout\n", ret, "");

    return 0;
}

int wd_get_timeleft(watchdog_t wd, unsigned int *time)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (time == NULL)
        ERROR("time == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETTIMELEFT, time);
    if (ret)
        WD_ERROR("Cannot get time left to reset by Watchdog\n", ret, "");

    return 0;
}

int wd_get_bootstatus(watchdog_t wd, int *status)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (status == NULL)
        ERROR("status == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETBOOTSTATUS, status);
    if (ret)
        WD_ERROR("Cannot get Bootstatus\n", ret, "");

    return 0;
}

int wd_get_status(watchdog_t wd, int *status)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (status == NULL)
        ERROR("status == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETSTATUS, status);
    if (ret)
        WD_ERROR("Cannot get Status\n", ret, "");

    return 0;
}

int wd_get_temp(watchdog_t wd, int *temp)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (temp == NULL)
        ERROR("temp == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETTEMP, temp);
    if (ret)
        WD_ERROR("Cannot get Temperature\n", ret, "");

    return 0;
}

int wd_set_options(watchdog_t wd, int options)
{
    int temp;
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    temp = options;
    if (temp == WDIOS_UNKNOWN)
        WD_ERROR("Unknown option\n", 1, "");

    CLEAR_FLAG(temp, WDIOS_DISABLECARD | WDIOS_ENABLECARD | WDIOS_TEMPPANIC);
    if (temp)
        ERROR("Incorrect option\n", 1, "");

    ret = ioctl(wd, WDIOC_SETOPTIONS, &options);
    if (ret)
        WD_ERROR("Cannot set options\n", ret, "");

    return 0;
}

int wd_get_info(watchdog_t wd, struct watchdog_info *wd_info)
{
    int ret;

    WD_TRACE("");

    if (wd == -1)
        ERROR("Incorrect WatchDog descriptor\n", 1, "");

    if (wd_info == NULL)
        ERROR("wd_info == NULL\n", 1, "");

    ret = ioctl(wd, WDIOC_GETSUPPORT, wd_info);
    if (ret)
        WD_ERROR("Cannot get WatchDog info\n", ret, "");

    return 0;
}