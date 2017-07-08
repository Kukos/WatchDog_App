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

/*
    Wrapper for getopt_long_only, accept only exacly option, instead of part option
*/
int my_getopt_long_only(int argc, char *const *argv, const char *short_opt, const struct option *long_opt, int *index);

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
        printf("Help\n");

    while ((i = my_getopt_long_only(argc, argv, "", long_option, &index)) != -1)
    {
        switch (i)
        {
            case OPT_DEVICE:
            {
                printf("Device\n");
                break;
            }
            case OPT_GET_TIMEOUT:
            {
                printf("GetTimeout\n");
                break;
            }
            case OPT_SET_TIMEOUT:
            {
                printf("SetTimeout\n");
                break;
            }
            case OPT_GET_PRETIMEOUT:
            {
                printf("GetPreTimeout\n");
                break;
            }
            case OPT_SET_PRETIMEOUT:
            {
                printf("SetPreTimeout\n");
                break;
            }
            case OPT_KEEPALIVE:
            {
                printf("KeepAlive\n");
                break;
            }
            case OPT_GET_TIMELEFT:
            {
                printf("GetTimeLeft\n");
                break;
            }
            case OPT_GET_BOOTSTATUS:
            {
                printf("GetBootStatus\n");
                break;
            }
            case OPT_GET_STATUS:
            {
                printf("GetStatus\n");
                break;
            }
            case OPT_GET_TEMP:
            {
                printf("GetTemp\n");
                break;
            }
            case OPT_SET_OPTIONS:
            {
                printf("SetOptions\n");
                break;
            }
            case OPT_GET_INFO:
            {
                printf("GetInfo\n");
                break;
            }
            case OPT_HELP:
            {
                printf("Help\n");
                break;
            }
            default:
                printf("Help\n");
        }
    }

    return 0;
}