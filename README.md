### Deku Daemon
##### TODO - The following needs to be built... please help
- Update mechanism: Allowing for remote updates
- Modem state reports: Logs each modems state with: _active_, _not_active_
- Store every request in a local sql database with state: _sent_, _not_sent_
- Include Unit testing for every component
#### Dependencies
- g++ 4.8
- make
- doxygen - _for generating documentations found in docs/_
- ModemManager - _comes installed on most Linux systems_
- Linux system with Bash support
#### How to build and run
_Project's root dir_\
`make -B start`\
`./start --c <path to file> --mode TEST`
#### Start arguments
##### `--c [configuration file]`
* `DIR_ISP=""`
* `STD_NAME_REQUEST_FILE=""`
* `DIR_REQUEST_FILE=""`
* `DIR_SUCCESS=""`
* `DIR_SCRIPTS=""`
* `SOCKET_HOST=""`
* `SOCKET_PORT=""`
##### `--modem PRODUCTION|TEST` _changes what the console outputs_
##### `--show_isp [number]`
##### `--run_ussd [code]`
##### `--run_ussd_script <path to file>`
##### `--exhaust_count [number]` _warning: do not set less than 3 if using MTN or say, could be very dangerous if automated_
