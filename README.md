## Deku Daemon
---
#### Dependencies
- Make - https://www.tutorialspoint.com/unix_commands/make.htm
- g++ 4.8 or later - https://courses.cs.washington.edu/courses/cse373/99au/unix/g++.html
- Most development was done with mysql-dev 5.7 in ubuntu, should change that to 8.0
> ###### ubuntu
> `sudo apt install build-essential`
> ###### manjaro
> `sudo pacman -S base-devel`
- MySQL server
> ###### ubuntu
> `sudo apt install mysql-server`\
> `sudo apt install libmysqld-dev`
> ###### Debian
> `sudo apt install mysql-server`\
> `sudo apt install libmysqlclient-dev`
> ###### manjaro
> `sudo pacman -S mysql`\
> `sudo pacman -S libmysqld-dev`
- Doxygen _(optional)_ - https://riptutorial.com/doxygen
 > ###### ubuntu
> `sudo apt install doxygen`
> ###### manjaro
> `sudo pacman -S doxygen`
- ModemManager - _comes installed on most Linux systems_

#### For Testing we use the CppUTest; https://cpputest.github.io/manual.html
> ###### ubuntu
> `sudo apt install cpputest`
> ###### manjaro
> `sudo pacman -S cpputest`

###### VARIABLES OF CONFIGURATION FILE (.ENV)
```env
- DIR_ISP=""
- DIR_REQUEST_FILE=""
- DIR_SUCCESS=""
- DIR_SCRIPTS=""
- DIR_ERROR=""
- STD_NAME_REQUEST_FILE=""
- STD_NAME_WHITELIST_FILE=""
- MYSQL_SERVER=""
- MYSQL_USER=""
- MYSQL_PASSWORD=""
- MYSQL_DATABASE=""
```
##### How to build and run
```bash
make -C src/
./bins/start --c <path to personal configuration file> --mode TEST(default)|PRODUCTION
```
---
#### Start arguments
##### `--c [configuration file]`
##### `--mode PRODUCTION|TEST` _changes what the console outputs_
##### `--show_isp [number]`
##### `--run_ussd [code]`
##### `--run_ussd_script <path to file>`
##### `--exhaust_count [number]` _warning: do not set less than 3 if using MTN or say, could be very dangerous if automated_
##### `--show_isp [number]`
##### `--generate_request [number]`
##### `--st [number]` _sleep time for modems, before iterating to check again_
##### `--f [filename]`
##### `--cleanse`
##### `--cleanse-only`
##### `--stat-only`
##### `--sms-only`
##### `--ussd-only --script=key=value e.g --script=modem=0,command=#99#,retry_count=3`
##### `--sms`
##### `--ls-locked-files-only`
##### `--release-locked-files-only`
##### `--release-locked-files`
##### `--remote-control` _must be used with --sms switch_
