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

#define WD_CLOSE_MSG    "V"
#define WD_DEV          "/dev/watchdog"
#define WD_ERROR(fmt, err, ...) ERROR(fmt, err, ##__VA_ARGS__)
#define WD_LOG(fmt, ...)        LOG(fmt, ##__VA_ARGS__)
#define WD_TRACE(...)           TRACE(__VA_ARGS__)


watchdog_t wd_open(const char *dev)
{
    char *wd;
    int fd;

    WD_TRACE("");

    dev ? (wd = (char *)dev) : (wd = (char *)WD_DEV);

    fd = open(dev, O_RDWR);
    if (fd == -1)
        WD_ERROR("Cannot open %s device\n", -1, dev);

    return fd;
}

int wd_close(watchdog_t wd)
{
    WD_TRACE("");

    if (write(wd, WD_CLOSE_MSG, strlen(WD_CLOSE_MSG)) == -1)
        WD_ERROR("Cannot close Watchdog\n", 1, "");

    if (close(wd) == -1)
        WD_ERROR("Cannot close Watchdog\n", 1, "");

    return 0;
}

int wd_gettimeout(watchdog_t wd, unsigned int *timeout)
{
    int ret;

    WD_TRACE("");

    ret = ioctl(wd, WDIOC_GETTIMEOUT, timeout);
    if (ret)
        WD_ERROR("Cannot get Watchdog timeout\n", ret, "");

    return 0;
}

int wd_settimeout(watchdog_t wd, unsigned int timeout)
{
    int ret;

    WD_TRACE("");

    ret = ioctl(wd, WDIOC_SETTIMEOUT, &timeout);
    if (ret)
        WD_ERROR("Cannot set Watchdog timeout\n", ret, "");

    return 0;
}

int wd_keepalive(watchdog_t wd)
{
    int ret;

    WD_TRACE("");

    ret = ioctl(wd, WDIOC_KEEPALIVE, NULL);
    if (ret)
        WD_ERROR("Cannot feed watchdog\n", ret, "");

    return 0;
}