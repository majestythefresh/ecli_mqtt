/***********************************************************************
* FILENAME    :   libeclimqttclient.h
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Common mqtt client library functions.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**********************************************************************/

#include <libeclimqttconf.h>
#include <libeclimqttlog.h>

/**********************************************************************/

#ifndef LIBECLIMQTTCLIENT_H_
#define LIBECLIMQTTCLIENT_H_

/**********************************************************************/
/* Max size in msg transer */
#define CLI_MAX_MSG_SIZE     MAX_MSG_SIZE

#define CLI_CFGLINE_LEN      128
#define CLI_HOSTNAME_LEN     25
#define CLI_CLIENTID_LEN     255
#define CLI_USERNAME_LEN     13
#define CLI_PASSWORD_LEN     13
#define CLI_TOPIC_LEN        255
#define CLI_MSG_LEN          102400
#define CLI_PATH_LEN         510
#define CLI_BUF_SIZE         1024
#define CLI_BUF_STR          50

#define CLI_EMPTY_BYTE       0x00
#define CLI_BYTE             0xFF
#define CLI_REMAIN_LEN       0x80  /* 128 */
#define CLI_CTRLPKT_PUBLISH  3<<4  /* 0011 0000 */
#define CLI_CTRLPKT_UNSUBACK 11<<4 /* 1011 0000 */

#define CLI_RSHIFT_BYTE( value )      ( ( value ) >> 8 )
#define CLI_LSHIFT_BYTE( value )      ( ( value ) << 8 )
#define CLI_MSG_TYPE( packet_buffer ) ( ( *packet_buffer & 0xF0 ) )
#define CLI_QOS_TYPE( packet_buffer ) ( ( *packet_buffer & 0x06 ) >> 1 )
#define CLI_MSG_QOS( packet_buffer )  ( ( *packet_buffer & 0x06 ) >> 1 )

/**********************************************************************/
/*Msg types*/
typedef enum {
    CLI_TXT_MSG = 0,
    CLI_DATAFILE_MSG,
} ecli_msg_type;

/*Error types*/
typedef enum {
    CLI_NO_ERROR = 0,
    CLI_ERROR,
    CLI_SOCK_ERROR,
    CLI_SET_SOCK_OPTS_ERROR,
    CLI_CON_ERROR,
    CLI_BRK_CON_ERROR,
    CLI_BRK_CON_READ_ERROR,
    CLI_BRK_CON_EXP_ERROR,
    CLI_BRK_CON_ACK_ERROR,
    CLI_BRK_DISCONNECT_ERROR,
    CLI_PUBLISH_ERROR,
    CLI_PUBLISH_SIZE_ERROR,
    CLI_SUB_SEND_ERROR,
    CLI_SUB_READ_ERROR,
    CLI_SUB_ACK_ERROR,
    CLI_SUB_MSGID_ERROR,
    CLI_PUB1_READ_ERROR,
    CLI_PUB1_ACK_ERROR,
    CLI_PUB1_MSGID_ERROR,
    CLI_PUB2_COMP_READ_ERROR,
    CLI_PUB2_COMP_ACK_ERROR,
    CLI_PUB2_COMP_MSGID_ERROR,
    CLI_PUB2_REC_READ_ERROR,
    CLI_PUB2_REC_ACK_ERROR,
    CLI_PUB2_REC_MSGID_ERROR,
    CLI_READ_SIZE_ERROR,
    CLI_FILE_ERROR,
    CLI_READ_TIMEOUT_ERROR,
    CLI_CONN_SESS_PRE,            /** Session present CONNACK*/
    CLI_UKNOW_FLAG_CONN,          /** Unknown case CONNACK*/
    CLI_UNACC_PRO_VER,            /** Unacceptable protocol version CONACK*/
    CLI_IDEN_REJEC,               /** Identifier rejected CONNACK*/
    CLI_SERVER_UNAVAI,            /** Server unavailable CONNACK*/
    CLI_USER_PASS_BAD,            /** Bad user name or password CONNACK*/
    CLI_NOT_AUTH                  /** Not authorized CONNACK*/
} ecli_conn_msg;

/**********************************************************************/
/*Main structure to create connection and send data to broker*/
typedef struct {
    char     client_id[CLI_CLIENTID_LEN];       /* Conn data */
    char     username[CLI_USERNAME_LEN];        /* Credentials */
    char     password[CLI_PASSWORD_LEN];        /* Credentials */
    char     topic[CLI_TOPIC_LEN];              /* Publish opts - Will */
    char     will_topic[CLI_TOPIC_LEN];         /* Conn opts - Will */
    char     will_msg[CLI_MSG_LEN];             /* Conn opts - Will */
    char     retain_msg[CLI_MSG_LEN];           /* Conn opts - Will */
    uint8_t  will_flag;	                          /* Conn opts - Will */
    uint8_t  will_retain;	                      /* Conn opts - Will */
    uint8_t  will_qos;                            /* Conn opts - Will */
    uint8_t  clean_session;                       /* Conn opts */
    uint8_t  qos;                                 /* Publish opts */
    uint8_t  retain;	                          /* Publish opts */
    uint16_t sequence;                            /* Management */
    uint16_t alive;                               /* Management */
    uint16_t msg_id;                              /* Management */
    int32_t  socketid;                            /* Conn data */
    uint32_t (*send_data)(uint32_t socketid, const void* buffer, int32_t count);
} ecli_broker_t;

/*User Configuration structure*/
typedef struct {
    char     broker_hostname[CLI_HOSTNAME_LEN]; /* Broker Hostname */
    char     msg_txt[CLI_MSG_LEN];              /* Text Message */
    char     datafile_path[CLI_PATH_LEN];       /* File Path */
    uint8_t  client_loop_flg;                     /* Read in a Loop - Flag */
    uint8_t  publish_online_flg;                  /* Publish Online message when first connect */
    uint8_t  ack_flg;                             /* ack flag */
    uint8_t  packet_buffer[CLI_BUF_SIZE];       /* Packet buffer part */
    uint16_t broker_port;                         /* Broker Port */
    int16_t persist_conn_time;                    /* Conn persistence time */
    ecli_msg_type  msg_type;                     /* Message Type (File or Txt)*/
} ecli_conf_t;

/**********************************************************************/
/** Get and Set user configuration opts
 *
 * @param broker: structure that contains the client connection info with broker
 * @param argc: number of command arguments.
 * @param argv: command arguments.
 *
 */
void ecli_get_conf( ecli_broker_t *broker, ecli_conf_t *conf, int argc, char* argv[] );

/**********************************************************************/
/** Set connection data & options
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t ecli_init( ecli_broker_t *broker, ecli_conf_t *conf );

/**********************************************************************/
/** Send packet function
 *
 * @param socketid: socket id / file descriptor
 * @param buffer: packet buffer.
 * @param count: size of packet.
 *
 */
uint32_t ecli_send( uint32_t socketid, const void* buffer, int32_t count );

/**********************************************************************/
/** Read mqtt header from packet
*
* @param broker: structure that contains the client connection info with broker
* @param conf: structure that contains the user config options for broker conn.
*
*/
uint32_t ecli_read_header( ecli_broker_t *broker, ecli_conf_t *conf );

/**********************************************************************/
/** Read and return Payload
*
* @param broker: structure that contains the client connection info with broker
* @param conf: structure that contains the user config options for broker conn.
*
*/
uint32_t ecli_read_get_msg( ecli_broker_t *broker, ecli_conf_t *conf,
                            char *topic, uint8_t *msg_buffer,
                            uint32_t *msg_len, uint8_t timeout );

/**********************************************************************/
/** Close connection Socket for client
*
* @param broker: structure that contains the client connection info with broker
*
*/
uint8_t ecli_close(ecli_broker_t *broker);

/**********************************************************************/
/** Show message according to error
*
* @param msg: msg type
*
*/
void ecli_show_error(ecli_conn_msg msg);

/**********************************************************************/
/** Get Message ID from mqtt packet
*
* @param packet_buffer: mqtt packet
*
*/
uint16_t ecli_get_msg_id(const uint8_t *packet_buffer);

/**********************************************************************/
/** Get number of Remaining Len bytes from mqtt packet
*
* @param packet_buffer: mqtt packet
*
*/
uint8_t  ecli_get_remain_len_b(const uint8_t *packet_buffer);

/**********************************************************************/
/** Get Remaining Len from mqtt packet
*
* @param packet_buffer: mqtt packet
*
*/
uint32_t ecli_get_remain_len(const uint8_t *packet_buffer);

#endif
