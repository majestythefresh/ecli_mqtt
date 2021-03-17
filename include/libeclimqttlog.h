/***********************************************************************
* FILENAME    :   libeclimqttlog.h
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Library header to log.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

/**********************************************************************/

#ifndef LIBECLIMQTTLOG_H_
#define LIBECLIMQTTLOG_H_

/**********************************************************************/
/*Default Conf. Values*/
#define LOG_INFO          "INFO"
#define LOG_DEBUG         "DEBUG"
#define LOG_ERROR         "ERROR"
#define LOG_TRACE         "TRACE"

/**********************************************************************/
/** Show message in term.
 *
 * @param caller: caller file string.
 * @param call: function that calls string.
 * @param msg: message string.
 * @param log_level: log level flag
 *
 */
void eclilog_show(const char * caller, const char * call, const char *msg, const char * log_level);

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
int8_t eclilog_log(const char * caller, const char * call, const char *msg, const char *logfile, const char * log_level);

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
int8_t eclilog_logshow(const char * caller, const char * call, const char *msg, const char *logfile, const char * log_level);

#endif
