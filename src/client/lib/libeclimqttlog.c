/***********************************************************************
* FILENAME    :   libeclimqttlog.c
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Library code to log
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <libeclimqttlog.h>

/**********************************************************************/
/** Show message in term.
 *
 * @param caller: caller file string.
 * @param call: function that calls string.
 * @param msg: message string.
 * @param log_level: log level flag
 *
 */
void eclilog_show(const char * caller, const char * call, const char *msg, const char * log_level) {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if ( strcmp( log_level, "INFO" ) == 0 ) {
#ifdef LOG_TRACE
        printf("[ %d-%02d-%02d %02d:%02d:%02d ] [ %s  ] : [%s (%s)] %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                log_level, caller, call, msg);
#else
        printf("[ %d-%02d-%02d %02d:%02d:%02d ] [ %s  ] : %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                log_level, msg);
#endif
    }

#ifdef LOG_DEBUG
    else if ( strcmp( log_level, "DEBUG" ) == 0 ) {
#ifdef LOG_TRACE
        printf("[ %d-%02d-%02d %02d:%02d:%02d ] [ %s  ] : [%s (%s)] %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                log_level, caller, call, msg);
#else
        printf("[ %d-%02d-%02d %02d:%02d:%02d ] [ %s  ] : %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                log_level, msg);
#endif
    }
#endif

}

/**********************************************************************/
/** log message in file.
 *
 * @param caller: caller file string.
 * @param call: function that calls string.
 * @param msg: message string.
 * @param logfile: log file path string.
 * @param log_level: log level flag
 *
 */
int8_t eclilog_log(const char * caller, const char * call, const char *msg, const char *logfile, const char * log_level) {

    int8_t return_code = 0;

    #ifdef LOG_BULK
        /* TODO: write log in file*/
    #endif

    return return_code;

}

/**********************************************************************/
/** show and log message in file.
 *
 * @param caller: caller file string.
 * @param call: function that calls string.
 * @param msg: message string.
 * @param logfile: log file path string.
 * @param log_level: log level flag
 *
 */
int8_t eclilog_logshow(const char * caller, const char * call, const char *msg, const char *logfile, const char * log_level) {

    int8_t return_code;

    eclilog_show(caller, call, msg, log_level);
    return_code = eclilog_log(caller, call, msg, logfile, log_level);

    return return_code;
}

/**********************************************************************/
