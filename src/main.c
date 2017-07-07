#include <stdio.h>
#include <watchdog.h>

int main(void)
{
    watchdog_t wd;
    unsigned int timeout;
    int ret;

    wd = wd_open(NULL);
    if (wd == -1)
        return 1;

    ret = wd_gettimeout(wd, &timeout);
    if (ret)
        return 1;

    (void)printf("Timeout = %u\n", timeout);

    (void)wd_close(wd);

    return 0;
}