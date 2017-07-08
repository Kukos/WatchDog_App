/*
    Simple Application to "talk" with HW WatchDog

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com
    LICENCE: GPL3.0
*/

#include <stdio.h>
#include <watchdog.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define WD_OPEN(wd, dev) \
    __extension__ \
    ({ \
        int ______ret = wd; \
        if (!is_open) \
        { \
            ______ret = wd_open(dev); \
            if (______ret == -1) \
                return 1; \
            is_open = false; \
        } \
        ______ret; \
    })

#define WD_CLOSE(wd) \
    __extension__ \
    ({ \
        int ______ret = 0; \
        if (is_open) \
        { \
            ______ret = wd_close(wd); \
            is_open = false; \
        } \
        ______ret; \
    })

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
    int opt_index;
    int _index;

    if (long_opt == NULL)
        return OPT_ERROR;

    opt_size = 0;
    while (long_opt[opt_size].name != NULL)
        ++opt_size;

    temp = getopt_long_only(argc, argv, short_opt, long_opt, &_index);
    if (index != NULL)
        *index = _index;

    if (temp == -1)
        return OPT_ERROR;

    if (optind > argc)
        return OPT_NO_MATCH;

    if (_index >= opt_size)
        return OPT_NO_MATCH;

    opt_index = optind - 1;
    if (long_opt[_index].has_arg == required_argument)
        --opt_index;

    /* -option */
    if (strcmp(long_opt[_index].name, argv[opt_index] + 1) == 0)
        return temp;

    /* --option */
    if (strcmp(long_opt[_index].name, argv[opt_index] + 2) == 0)
        return temp;

    /* only part of word */
    return OPT_NO_MATCH;
}

int main(int argc, char **argv)
{
    /* logic */
    int i;
    bool is_open = false;
    int ret;

    /* WD parameters */
    watchdog_t wd;
    unsigned int timeout;
    char *dev = NULL;
    int flag;
    int temperature;
    struct watchdog_info info;

    /* options */
    struct option long_option[] =
	{
        {"dev",             required_argument,  0,  OPT_DEVICE},
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

    /* no error msg */
    opterr = 0;
    if (argc < 2)
    {
        usage();
        return 1;
    }

    while ((i = my_getopt_long_only(argc, argv, "", long_option, NULL)) != -1)
    {
        switch (i)
        {
            case OPT_DEVICE:
            {
                dev = optarg;
                break;
            }
            case OPT_GET_TIMEOUT:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_timeout(wd, &timeout);
                if (ret)
                    return 1;

                (void)printf("WD Timeout = %d [s]\n", timeout);

                break;
            }
            case OPT_SET_TIMEOUT:
            {
                timeout = (unsigned int)atoi(optarg);
                if (timeout == 0 && *optarg != '0')
                {
                    (void)fprintf(stderr, "Timeout [%s] - Incorrect argument\n", optarg);
                    WD_CLOSE(wd);
                    return 1;
                }

                wd = WD_OPEN(wd, dev);
                ret = wd_set_timeout(wd, timeout);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("Timeout set to %u\n", timeout);

                break;
            }
            case OPT_GET_PRETIMEOUT:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_pretimeout(wd, &timeout);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("WD PreTimeout = %d [s]\n", timeout);

                break;
            }
            case OPT_SET_PRETIMEOUT:
            {
                timeout = (unsigned int)atoi(optarg);
                if (timeout == 0 && *optarg != '0')
                {
                    (void)fprintf(stderr, "PreTimeout [%s] - Incorrect argument\n", optarg);
                    WD_CLOSE(wd);
                    return 1;
                }

                wd = WD_OPEN(wd, dev);
                ret = wd_set_pretimeout(wd, timeout);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("PreTimeout set to %u\n", timeout);

                break;
            }
            case OPT_KEEPALIVE:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_keepalive(wd);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("Watchdog fed\n");

                break;
            }
            case OPT_GET_TIMELEFT:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_timeleft(wd, &timeout);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("WD Time left to reset = %d [s]\n", timeout);

                break;
            }
            case OPT_GET_BOOTSTATUS:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_bootstatus(wd, &flag);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("WD Boot Status:\n");
                wd_print_decoded_flag(flag);

                break;
            }
            case OPT_GET_STATUS:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_status(wd, &flag);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("WD Status:\n");
                wd_print_decoded_flag(flag);

                break;
            }
            case OPT_GET_TEMP:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_temp(wd, &temperature);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("Temperature = %d [F]\n", temperature);

                break;
            }
            case OPT_SET_OPTIONS:
            {
                flag = atoi(optarg);
                if (flag == 0 && *optarg != '0')
                {
                    (void)fprintf(stderr, "Flag [%s] - Incorrect argument\n", optarg);
                    WD_CLOSE(wd);
                    return 1;
                }

                wd = WD_OPEN(wd, dev);
                ret = wd_set_options(wd, flag);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                (void)printf("Option set to %#x\n", flag);

                break;
            }
            case OPT_GET_INFO:
            {
                wd = WD_OPEN(wd, dev);
                ret = wd_get_info(wd, &info);
                if (ret)
                {
                    WD_CLOSE(wd);
                    return 1;
                }

                wd_print_info(&info);
                wd_print_decoded_flag(flag);

                break;
            }
            case OPT_HELP:
            {
                usage();
                break;
            }
            default:
                usage();
        }
    }

    WD_CLOSE(wd);

    return 0;
}