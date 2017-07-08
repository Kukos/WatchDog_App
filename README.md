# WatchDog_App
Application to "talk" with WatchDog from userspace

#### To compile
Just make

#### To clean
Just make clean

## Usage
HELP

--dev [x]               - change watchdog, default is /dev/watchdog

--get-timeout           - get timeout in seconds

--set-timeout [x]       - set timeout in seconds

--get-pretimeout        - get pretimeout in seconds

--set-pretimeout [x]    - set pretimeout in seconds

--kepalive              - feed watchdog

--get-timeleft          - get time left to reset in seconds

--get-bootstatus        - get bootstatus

--get-status            - get status

--get-temp              - get temperature in in degrees fahrenheit

--set-options [x]       - set options in hex

--get-info              - get info about WatchDog

--help                  - print this usage


Examples

./watchdog --dev /dev/watchdog0 --set-timeout 8

./watchdog --set-options 0x3

./watchdog --get-info --get-temp --get-bootstatus --get-timeleft
