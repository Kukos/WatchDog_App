#include <stdio.h>
#include <watchdog.h>
#include <getopt.h>
#include <string.h>

enum {
    OPT_DEVICE          = 1000,
    OPT_GET_TIMEOUT,
    OPT_SET_TIMEOUT,
    OPT_GET_PRETIMEOUT,
    OPT_SET_PRETIMEOUT,
    OPT_KEEPALIVE,
    OPT_GET_TIMELEFT,
    OPT_GET_BOOTSTATUS,
    OPT_GET_STATUS,
    OPT_GET_TEMP,
    OPT_SET_OPTIONS,
    OPT_GET_INFO,
    OPT_HELP
} OPTIONS;

#define OPT_NO_MATCH    0
#define OPT_ERROR       -1

/* Wrapper for getopt_long_only, accept only exacly option, instead of part option */
int my_getopt_long_only(int argc, char *const *argv, const char *short_opt, const struct option *long_opt, int *index);

/* Print help */
void usage(void);

void usage(void)
{
    (void)printf("HELP\n\n");
    (void)printf("--dev [x]\t\t- change watchdog, default is /dev/watchdog\n");
    (void)printf("--get-timeout\t\t- get timeout in seconds\n");
    (void)printf("--set-timeout [x]\t- set timeout in seconds\n");
    (void)printf("--get-pretimeout\t- get pretimeout in seconds\n");
    (void)printf("--set-pretimeout [x]\t- set pretimeout in seconds\n");
    (void)printf("--kepalive\t\t- feed watchdog\n");
    (void)printf("--get-timeleft\t\t- get time left to reset in seconds\n");
    (void)printf("--get-bootstatus\t- get bootstatus\n");
    (void)printf("--get-status\t\t- get status\n");
    (void)printf("--get-temp\t\t- get temperature in in degrees fahrenheit\n");
    (void)printf("--set-options [x]\t- set options in hex\n");
    (void)printf("--get-info\t\t- get info about WatchDog\n");
    (void)printf("--help\t\t\t- print this usage\n");
    (void)printf("\n");
    (void)printf("Examples\n");
    (void)printf("./watchdog --dev /dev/watchdog0 --set-timeout 8\n");
    (void)printf("./watchdog --set-options 0x3\n");
    (void)printf("./watchdog --get-info --get-temp --get-bootstatus --get-timeleft\n");
    (void)printf("\n");
}

int my_getopt_long_only(int argc, char *const *argv, const char *short_opt, const struct option *long_opt, int *index)
{
    int temp;
    int opt_size;
    int i;

    if (long_opt == NULL)
        return OPT_ERROR;

    i = 0;
    while (long_opt[i].name != NULL)
        ++i;

    opt_size = i;

    temp = getopt_long_only(argc, argv, short_opt, long_opt, index);
    if (temp == -1)
        return OPT_ERROR;

    if (optind < argc)
        return OPT_NO_MATCH;

    if (*index >= opt_size)
        return OPT_NO_MATCH;

    /* -option */
    if (strcmp(long_opt[*index].name, argv[optind - 1] + 1) == 0)
        return temp;

    /* --option */
    if (strcmp(long_opt[*index].name, argv[optind - 1] + 2) == 0)
        return temp;

    /* only part of word */
    return OPT_NO_MATCH;
}

int main(int argc, char **argv)
{
    //watchdog_t wd;
    //unsigned int timeout;
    //int ret;
    //char *dev = NULL;
    int i;
    int index;

    struct option long_option[] =
	{
        {"dev",             no_argument,        0,  OPT_DEVICE},
        {"get-timeout",     no_argument,        0,  OPT_GET_TIMEOUT},
        {"set-timeout",     required_argument,  0,  OPT_SET_TIMEOUT},
        {"get-pretimeout",  no_argument,        0,  OPT_GET_PRETIMEOUT},
        {"set-pretimeout",  required_argument,  0,  OPT_SET_PRETIMEOUT},
        {"keepalive",       no_argument,        0,  OPT_KEEPALIVE},
        {"get-timeleft",    no_argument,        0,  OPT_GET_TIMELEFT},
        {"get-bootstatus",  no_argument,        0,  OPT_GET_BOOTSTATUS},
        {"get-status",      no_argument,        0,  OPT_GET_STATUS},
        {"get-temp",        no_argument,        0,  OPT_GET_TEMP},
        {"set-options",     required_argument,  0,  OPT_SET_OPTIONS},
        {"get-info",        no_argument,        0,  OPT_GET_INFO},
        {"help",            no_argument,        0,  OPT_HELP},
        {NULL,              0,                  0,  0}
    };

    opterr = 0;
    if (argc < 2)
    {
        usage();
        return 1;
    }

    while ((i = my_getopt_long_only(argc, argv, "", long_option, &index)) != -1)
    {
        switch (i)
        {
            case OPT_DEVICE:
            {
                (void)printf("Device\n");
                break;
            }
            case OPT_GET_TIMEOUT:
            {
                (void)printf("GetTimeout\n");
                break;
            }
            case OPT_SET_TIMEOUT:
            {
                (void)printf("SetTimeout\n");
                break;
            }
            case OPT_GET_PRETIMEOUT:
            {
                (void)printf("GetPreTimeout\n");
                break;
            }
            case OPT_SET_PRETIMEOUT:
            {
                (void)printf("SetPreTimeout\n");
                break;
            }
            case OPT_KEEPALIVE:
            {
                (void)printf("KeepAlive\n");
                break;
            }
            case OPT_GET_TIMELEFT:
            {
                (void)printf("GetTimeLeft\n");
                break;
            }
            case OPT_GET_BOOTSTATUS:
            {
                (void)printf("GetBootStatus\n");
                break;
            }
            case OPT_GET_STATUS:
            {
                (void)printf("GetStatus\n");
                break;
            }
            case OPT_GET_TEMP:
            {
                (void)printf("GetTemp\n");
                break;
            }
            case OPT_SET_OPTIONS:
            {
                (void)printf("SetOptions\n");
                break;
            }
            case OPT_GET_INFO:
            {
                (void)printf("GetInfo\n");
                break;
            }
            case OPT_HELP:
            {
                (void)printf("Help\n");
                break;
            }
            default:
                usage();
        }
    }

    return 0;
}