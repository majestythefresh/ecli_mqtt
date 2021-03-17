/***********************************************************************
* FILENAME    :   libeclimqttclient.c
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Common mqtt client library code.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <arpa/inet.h>
#include <linux/tcp.h>
#include <errno.h>

/**********************************************************************/

#include <libeclimqttclient.h>

/**********************************************************************/
/**********************************************************************/
/** Get and Set conf values from Config file
 *
 * @param conf: structure that contains the user config options for broker conn.
 * @param cfg_file: conf file path
 *
 */
static void get_set_cfg_file(ecli_broker_t *broker, ecli_conf_t *conf, const char *cfg_file);

/**********************************************************************/
/** Get Key Value of line from Config file
 *
 * @param conf: structure that contains the user config options for broker conn.
 * @param key: key return name
 * @param value: return value
 *
 */
static void get_conf_value( char *line, char *key, char *value );

/**********************************************************************/
/** Get Topic from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 * @param topic_ptr: ptr to topic to return
 *
 */
static uint16_t ecli_get_topic(const uint8_t* packet_buffer, const uint8_t **topic_ptr);

/**********************************************************************/
/** Get Message from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 * @param msg_ptr: ptr to message to return
 *
 */
static uint32_t ecli_get_message(const uint8_t* packet_buffer, const uint8_t **msg_ptr);

/**********************************************************************/
/**********************************************************************/
/** Get and Set user configuration opts
 *
 * @param broker: structure that contains the client connection info with broker
 * @param argc: number of command arguments.
 * @param argv: command arguments.
 *
 */
void ecli_get_conf(ecli_broker_t *broker, ecli_conf_t *conf, int argc, char* argv[]){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    extern   char *optarg;
    char     *config_file      = CONFIGFILE_DEFAULT;
    char     *broker_ip        = BROKER_IP_DEFAULT;
    char     *username         = USERNAME_DEFAULT;
    char     *password         = PASSWORD_DEFAULT;
    char     *topic            = TOPIC_DEFAULT;
    char     *client_id        = CLIENTID_DEFAULT;
    char     *text_message     = TXT_MSG_DEFAULT;
    char     *output_file      = OUT_FILE_DEFAULT;
    char     *will_msg         = WILL_MSG_DEFAULT;
    char     *will_topic       = WILL_TOPIC_DEFAULT;
    uint8_t  clean_session     = CLEAN_SESSION_DEFAULT;
    uint8_t  will_qos          = WILL_QOS_DEFAULT;
    uint8_t  will_retain       = WILL_RETAIN_DEFAULT;
    uint8_t  will_flag         = WILL_FLAG_DEFAULT;
    uint8_t  qos               = QOS_DEFAULT;
    uint8_t  retain_flag       = RETAIN_DEFAULT;
    uint8_t  sequence          = SEQUENCE_DEFAULT;
    uint8_t  datafile_trans    = FILE_TRANS_DEFAULT;
    uint8_t  client_loop_flg   = READ_LOOP_DEFAULT;
    uint8_t  cfg_file_flag     = CFG_FILE_FLAG_DEFAULT;
    uint8_t  pub_online_flag   = PUBONLINE_FLG_DEFAULT;
    uint16_t alive             = ALIVE_CON_DEFAULT;
    int16_t  persist_conn_time = PERSIST_CON_DEFAULT;
    uint16_t broker_port       = BROKER_PORT_DEFAULT;
    uint32_t c;

    /* Get Values from Opt Args */
    while ((c = getopt (argc, argv, "a:c:b:p:u:k:i:t:m:o:q:Q:T:M:P:lfrhRWCO")) != -1) {
        switch (c) {
            case 'c': /* Config File */
                cfg_file_flag = 1;
                config_file = optarg;
                break;
            case 'b': /* Broker IP */
                broker_ip = optarg;
                break;
            case 'p': /* Broker Port */
                broker_port = atoi( optarg );
                break;
            case 'u': /* Broker Username */
                username = optarg;
                break;
            case 'k': /* Broker Password */
                password = optarg;
                break;
            case 'i': /* Client ID */
                client_id = optarg;
                break;
            case 't': /* Topic */
                topic = optarg;
                break;
            case 'm': /* Message */
                text_message = optarg;
                break;
            case 'o': /* Output file for datafile recv */
                output_file = optarg;
                break;
            case 'q': /* Quality of Service */
                qos = atoi( optarg );
                break;
            case 'a': /* Alive */
                alive = atoi( optarg );
                break;
            case 'Q': /* Will QOS */
                will_qos = atoi( optarg );
                break;
            case 'T': /* Will QOS */
                will_topic = optarg;
                break;
            case 'M': /* Will QOS */
                will_msg = optarg;
                break;
            case 'P': /* Persist on Connection */
                persist_conn_time = atoi( optarg );
                break;
            case 'l': /* Sub Read Loop */
                client_loop_flg = TRUE_FLAG;
                break;
            case 'f': /* Datafile transfer flag */
                datafile_trans = TRUE_FLAG;
                break;
            case 'r': /* Publish Retain */
                retain_flag = TRUE_FLAG;
                break;
            case 'R': /* Will Retain */
                will_retain = TRUE_FLAG;
                break;
            case 'W': /* Will Flag */
                will_flag = TRUE_FLAG;
                break;
            case 'C': /* Clean Session */
                clean_session = TRUE_FLAG;
                break;
            case 'O': /* Online Message at first connect */
                pub_online_flag = TRUE_FLAG;
                break;
            case 'h': /* Help */
                printf(HELP_TXT);
                exit( CLI_NO_ERROR );
            case '?':
                fprintf (stderr, UNKNOW_OPT_ERROR, optopt);
                exit( CLI_ERROR );
          default:
            fprintf (stderr, UNKNOW_OPT_ERROR, optopt);
            exit( CLI_ERROR );
        }
    }


    memset( broker->will_topic, 0, sizeof( broker->will_topic ) );
    memset( broker->will_msg, 0, sizeof( broker->will_msg ) );
    memset( broker->username, 0, sizeof( broker->username ) );
    memset( broker->password, 0, sizeof( broker->password ) );
    memset( broker->topic, 0, sizeof( broker->topic ) );
    memset( broker->client_id, 0, sizeof( broker->client_id ) );
    memset( conf->broker_hostname, 0, sizeof( conf->broker_hostname ) );
    memset( conf->msg_txt, 0, sizeof( conf->msg_txt ) );
    memset( conf->datafile_path, 0, sizeof( conf->datafile_path ) );

    /* Get & Set Values from Config file */
    if ( cfg_file_flag ) {
        get_set_cfg_file(broker, conf, config_file);
    }

    /* Conn opts */
    broker->will_flag = will_flag;
    if ( broker->will_flag ){
        strncpy(broker->will_topic, will_topic, sizeof( broker->will_topic ) );
        strncpy(broker->will_msg, will_msg, sizeof( broker->will_msg ) );
    }
    broker->will_retain = will_retain;
    broker->will_qos = will_qos;
    broker->clean_session = clean_session;

    /* Client credentials */
    strncpy(broker->username, username, sizeof( broker->username ) );
    strncpy(broker->password, password, sizeof( broker->username ) );

    /*Publish Opts*/
    strncpy( broker->topic, topic, sizeof( broker->topic ) );
    broker->retain = retain_flag;
    broker->qos = qos;
    broker->alive = alive;
    broker->sequence = sequence;

    /* Client ID */
    strncpy( broker->client_id, client_id, sizeof( broker->client_id ) );

    /* Set Values to configuration */
    strncpy(conf->broker_hostname, broker_ip, sizeof( conf->broker_hostname ) );
    conf->broker_port = broker_port;
    conf->client_loop_flg = client_loop_flg;
    conf->publish_online_flg = pub_online_flag;
    conf->persist_conn_time = persist_conn_time;
    if ( datafile_trans ) {
        conf->msg_type = CLI_DATAFILE_MSG;
        strncpy(conf->msg_txt, text_message, sizeof( conf->msg_txt ) );
        strncpy(conf->datafile_path, output_file, sizeof( conf->datafile_path ) );
    }
    else {
        conf->msg_type = CLI_TXT_MSG;
        strncpy(conf->msg_txt, text_message, sizeof( conf->msg_txt ) );
    }

}

/**********************************************************************/
/** Set connection data & options
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t ecli_init(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char    buffer_str[CLI_BUF_SIZE] = {0};
    int32_t opt_flag    = 1;
    uint8_t return_code = CLI_NO_ERROR;
    uint32_t conn_secs   = 0;
    struct sockaddr_in socket_addr;

    socket_addr.sin_addr.s_addr = inet_addr(conf->broker_hostname);
    socket_addr.sin_port = htons(conf->broker_port);
    socket_addr.sin_family = AF_INET;

    if ( ( broker->socketid = socket( PF_INET, SOCK_STREAM, 0 ) ) < CLI_NO_ERROR ) {
        return CLI_SOCK_ERROR;
    }
    if ( setsockopt( broker->socketid, IPPROTO_TCP, TCP_NODELAY,
        ( const void * ) &opt_flag, sizeof( opt_flag ) ) < CLI_NO_ERROR ) {
        return CLI_SET_SOCK_OPTS_ERROR;
    }
    do {
        if ( ( connect ( broker->socketid,
             (struct sockaddr *) &socket_addr,
              sizeof(socket_addr) ) ) < CLI_NO_ERROR ) {
                  return_code = CLI_CON_ERROR;
        }
        else {
            return_code = CLI_NO_ERROR;
            sprintf(buffer_str, CONNECTED_MSG, conf->broker_hostname, conf->broker_port);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        }
        sleep(1);
        conn_secs++;
        sprintf(buffer_str, CONN_TRY_MSG, conf->broker_hostname, conf->broker_port);
        eclilog_show(__FILE__, __func__, buffer_str, LOG_INFO);
    }
    while(conn_secs != conf->persist_conn_time && conf->persist_conn_time );
    broker->send_data = ecli_send;

    return return_code;
}

/**********************************************************************/
/** Send packet function
 *
 * @param socketid: socket id / file descriptor
 * @param buffer: packet buffer.
 * @param count: size of packet.
 *
 */
uint32_t ecli_send(uint32_t socketid, const void* buffer, int32_t count) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    return send(socketid, buffer, count, 0);
}

/**********************************************************************/
/** Read mqtt headers from packet
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint32_t ecli_read_header(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    int32_t totalbytes = 0;
    int32_t rcv_bytes  = 0;

    memset(conf->packet_buffer, 0, sizeof( conf->packet_buffer ) );

    rcv_bytes = recv( broker->socketid, conf->packet_buffer, CLI_BUF_SIZE, 0 );
    if( rcv_bytes <= 0 ) {
        return CLI_ERROR;
    }
    totalbytes += rcv_bytes;

    return totalbytes;
}

/**********************************************************************/
/** Read and return Payload
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint32_t ecli_read_get_msg(ecli_broker_t *broker, ecli_conf_t *conf,
                            char *topic, uint8_t *msg_buffer,
                            uint32_t *msg_len, uint8_t timeout) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    const uint8_t* topic_ptr;
    const uint8_t* msg_ptr;

    uint8_t  pos            = 0;
    uint8_t  rem_len_bytes  = 1;
    uint8_t  i              = 0;
    uint16_t topic_len      = 0;
    uint32_t multiplier     = 1;
    uint32_t digit          = 0;
    uint32_t rem_len        = 0;
    int32_t  totalbytes     = 0;
    int32_t  rcv_bytes      = 0;

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    setsockopt( broker->socketid, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    memset(conf->packet_buffer, 0, sizeof( conf->packet_buffer ) );
    /* buffer size according to Type of Message [ text msg | datafile msg ]*/
    if ( conf->msg_type == CLI_DATAFILE_MSG ) {
        memset(msg_buffer, 0, MAX_CHUNK_SIZE );
    }
    else {
        memset(msg_buffer, 0, MAX_TXT_MSG_SIZE );
    }

    /*Getting first chunk to get remaining len*/
    rcv_bytes = recv( broker->socketid, conf->packet_buffer, CLI_BUF_SIZE, 0 );
    if( rcv_bytes <= 0 ) {
        if ( rcv_bytes < 0 ) {
            return CLI_READ_TIMEOUT_ERROR;
        }
        return CLI_ERROR;
    }
    totalbytes += rcv_bytes;
    /*Fix header flag skip*/
    pos++;
    /*Get num of bytes for rem len*/
    do {
        digit = conf->packet_buffer[pos];
        rem_len += ( digit & ( CLI_REMAIN_LEN -1 ) ) * multiplier;
        multiplier *= CLI_REMAIN_LEN;
        pos++;
    }
    while ( ( digit & CLI_REMAIN_LEN ) != 0 );
    /*Get num of bytes for rem len number of bytes*/
    for ( i = 1; i < 5; i++ ) {
        if ( ( conf->packet_buffer[i] & CLI_REMAIN_LEN ) == CLI_REMAIN_LEN ) {
            rem_len_bytes++;
        }
        else {
            break;
        }
    }

    /*Get remaining bytes of data*/
    uint32_t packet_length = rem_len + rem_len_bytes + 1;
    uint8_t packet_buffer[packet_length];
    memset( packet_buffer, 0, packet_length );
    /* Guard buffer integrity copying memory*/
    if( packet_length >= sizeof( conf->packet_buffer ) ) {
        memcpy( packet_buffer, conf->packet_buffer, sizeof( conf->packet_buffer ) );
    }
    else {
        memcpy( packet_buffer, conf->packet_buffer, packet_length );
    }
    while(totalbytes < packet_length) // Reading the packet
    {
        rcv_bytes = recv( broker->socketid, ( packet_buffer + totalbytes ),
                          CLI_BUF_SIZE, 0 );
        if( rcv_bytes <= 0 ) {
            return CLI_ERROR;
        }
        totalbytes += rcv_bytes;
    }

    /*Get Topic buffer*/
    topic_len = ecli_get_topic(packet_buffer, &topic_ptr);
    if(topic_len != 0 && topic_ptr != NULL) {
        memcpy(topic, topic_ptr, topic_len);
    }
    topic[topic_len] = '\0';
    /*Get Message buffer*/
    *msg_len = ecli_get_message(packet_buffer, &msg_ptr);
    if( msg_len != 0 && msg_ptr != NULL) {
        memcpy( msg_buffer, msg_ptr, *msg_len);
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Close connection Socket for client
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
uint8_t ecli_close(ecli_broker_t *broker){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    return close(broker->socketid);
}

/**********************************************************************/
/** Get Message ID from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 *
 */
uint16_t ecli_get_msg_id(const uint8_t *packet_buffer) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t type          = CLI_MSG_TYPE( packet_buffer );
    uint8_t qos           = CLI_QOS_TYPE( packet_buffer );
    uint8_t rem_len_bytes = 0;
    uint16_t msg_id       = 0;
    uint8_t offset        = 0;

    /***** Fixed header ****/
    /***********************/
    if(type >= CLI_CTRLPKT_PUBLISH && type <= CLI_CTRLPKT_UNSUBACK) {
        if(type == CLI_CTRLPKT_PUBLISH) {
            if(qos != 0) {
                rem_len_bytes = ecli_get_remain_len_b( packet_buffer );
                offset  = CLI_LSHIFT_BYTE( *( packet_buffer + 1 + rem_len_bytes ) );
                offset |= *( packet_buffer + 1 + rem_len_bytes + 1 );
                offset += ( 1 + rem_len_bytes + 2 );
                msg_id  = CLI_LSHIFT_BYTE( *( packet_buffer + offset ) );
                msg_id |= *( packet_buffer + offset + 1 );
            }
            } else {
                rem_len_bytes = ecli_get_remain_len_b( packet_buffer );
                msg_id  = CLI_LSHIFT_BYTE( *( packet_buffer + 1 + rem_len_bytes ) );
                msg_id |= *( packet_buffer + 1 + rem_len_bytes + 1 );
            }
        }

    return msg_id;
}

/**********************************************************************/
/** Get number of Remaining Len bytes from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 *
 */
uint8_t ecli_get_remain_len_b(const uint8_t *packet_buffer) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t num_bytes = 1;
    uint8_t  i        = 0;

    for ( i = 1; i < 5; i++ ) {
        if ( ( packet_buffer[i] & CLI_REMAIN_LEN ) == CLI_REMAIN_LEN ) {
            num_bytes++;
        }
        else {
            break;
        }
    }

    return num_bytes;
}

/**********************************************************************/
/** Get Remaining Len from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 *
 */
uint32_t ecli_get_remain_len(const uint8_t *packet_buffer) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint32_t multiplier = 1;
    uint32_t value      = 0;
    uint32_t digit      = 0;

    /*Fix header flag skip*/
    packet_buffer++;
    do {
        digit = *packet_buffer;
        value += ( digit & ( CLI_REMAIN_LEN -1 ) ) * multiplier;
        multiplier *= CLI_REMAIN_LEN;
        packet_buffer++;
    }
    while ( ( digit & CLI_REMAIN_LEN ) != 0 );

    return value;
}

/**********************************************************************/
/** Show message according to error
 *
 * @param msg: msg type
 *
 */
void ecli_show_error(ecli_conn_msg return_code){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char buffer_str[CLI_BUF_SIZE] = {0};

    switch(return_code) {
        case CLI_SOCK_ERROR:
            sprintf(buffer_str, SOCK_ERROR, strerror( errno ));
            break;
        case CLI_SET_SOCK_OPTS_ERROR:
            sprintf(buffer_str, SET_SOCK_OPTS_ERROR, strerror( errno ) );
            break;
        case CLI_CON_ERROR:
            sprintf(buffer_str, CON_ERROR, strerror( errno ) );
            break;
        case CLI_BRK_CON_ERROR:
            sprintf(buffer_str, BRK_CON_ERROR, strerror( errno ) );
            break;
        case CLI_BRK_CON_READ_ERROR:
            sprintf(buffer_str, BRK_CON_READ_ERROR, strerror( errno ) );
            break;
        case CLI_BRK_CON_EXP_ERROR:
            sprintf(buffer_str, BRK_CON_EXP_ERROR, strerror( errno ) );
            break;
        case CLI_BRK_CON_ACK_ERROR:
            sprintf(buffer_str, BRK_CON_ACK_ERROR, strerror( errno ) );
            break;
        case CLI_BRK_DISCONNECT_ERROR:
            sprintf(buffer_str, BRK_DISCONNECT_ERROR, strerror( errno ) );
            break;
        case CLI_PUBLISH_ERROR:
            sprintf(buffer_str, PUBLISH_ERROR, strerror( errno ) );
            break;
        case CLI_PUBLISH_SIZE_ERROR:
            sprintf(buffer_str, PUBLISH_SIZE_ERROR);
            break;
        case CLI_SUB_SEND_ERROR:
            sprintf(buffer_str, SUB_SEND_ERROR, strerror( errno ) );
            break;
        case CLI_PUB1_READ_ERROR:
            sprintf(buffer_str, PUB1_READ_ERROR , strerror( errno ) );
            break;
        case CLI_PUB1_ACK_ERROR:
            sprintf(buffer_str, PUB1_ACK_ERROR, strerror( errno ) );
            break;
        case CLI_PUB1_MSGID_ERROR:
            sprintf(buffer_str, PUB1_MSGID_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_REC_READ_ERROR:
            sprintf(buffer_str, PUB2_REC_READ_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_REC_ACK_ERROR:
            sprintf(buffer_str, PUB2_REC_ACK_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_REC_MSGID_ERROR:
            sprintf(buffer_str, PUB2_REC_MSGID_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_COMP_READ_ERROR:
            sprintf(buffer_str, PUB2_COMP_READ_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_COMP_ACK_ERROR:
            sprintf(buffer_str, PUB2_COMP_ACK_ERROR, strerror( errno ) );
            break;
        case CLI_PUB2_COMP_MSGID_ERROR:
            sprintf(buffer_str, PUB2_COMP_MSGID_ERROR, strerror( errno ) );
            break;
        case CLI_READ_SIZE_ERROR:
            sprintf(buffer_str, READ_SIZE_ERROR);
            break;
        case CLI_SUB_MSGID_ERROR:
            sprintf(buffer_str, SUB_READ_ERROR, strerror( errno ) );
            break;
        case CLI_SUB_ACK_ERROR:
            sprintf(buffer_str, SUB_ACK_ERROR, strerror( errno ) );
            break;
        case CLI_SUB_READ_ERROR:
            sprintf(buffer_str, SUB_MSGID_ERROR, strerror( errno ) );
            break;
        case CLI_FILE_ERROR:
            sprintf(buffer_str, OPEN_FILE_ERROR);
            break;
        case CLI_ERROR:
            sprintf(buffer_str, CONN_BRR_ERROR);
            break;
        case CLI_READ_TIMEOUT_ERROR:
            sprintf(buffer_str, READ_TIMEOUT_ERROR);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            return;
            break;
        case CLI_CONN_SESS_PRE:
            sprintf(buffer_str, CONN_SESS_PRE);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_UKNOW_FLAG_CONN:
            sprintf(buffer_str, UKNOW_FLAG_CONN);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_UNACC_PRO_VER:
            sprintf(buffer_str, UNACC_PRO_VER);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_IDEN_REJEC:
            sprintf(buffer_str, IDEN_REJEC);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_SERVER_UNAVAI:
            sprintf(buffer_str, SERVER_UNAVAI);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_USER_PASS_BAD:
            sprintf(buffer_str, USER_PASS_BAD);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        case CLI_NOT_AUTH:
            sprintf(buffer_str, NOT_AUTH);
            eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
            break;
        break;
        default:
            sprintf(buffer_str, UNKNOW_ERROR, strerror( errno ));
  }
  eclilog_show(__FILE__, __func__, buffer_str, LOG_ERROR);

}

/**********************************************************************/
/** Get and Set conf values from Config file
 *
 * @param conf: structure that contains the user config options for broker conn.
 * @param cfg_file: conf file path
 *
 */
static void get_set_cfg_file(ecli_broker_t *broker, ecli_conf_t *conf, const char *cfg_file) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char     key[CLI_CFGLINE_LEN]    = {0};
    char     value[CLI_CFGLINE_LEN]  = {0};
    char     buffer_str[CLI_BUF_STR] = {0};

    sprintf(buffer_str, CFG_FILE_MSG, cfg_file);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
    FILE *fileptr = fopen ( cfg_file, "r" );
    if ( fileptr != NULL ) {
        char line [CLI_CFGLINE_LEN];
        while ( fgets ( line, sizeof line, fileptr ) != NULL ) {
            memset( key, 0, sizeof( key ) );
            memset( value, 0, sizeof( value ) );
            get_conf_value( line, key, value );
            //printf(" [%s] = [%s]\n", key, value);
            if ( strcmp( key, BROKER_IP_ID ) == EQUAL_STR_CMP ){
                strncpy( conf->broker_hostname, value, sizeof( conf->broker_hostname ) );
            }
            else if ( strcmp( key, BROKER_PORT_ID ) == EQUAL_STR_CMP ) {
                conf->broker_port = atoi( value );
            }
            else if ( strcmp( key, BROKER_USER_ID ) == EQUAL_STR_CMP ) {
                strncpy( broker->username, value, sizeof( broker->username ) );
            }
            else if ( strcmp( key, BROKER_PASSWD_ID ) == EQUAL_STR_CMP ) {
                strncpy( broker->password, value, sizeof( broker->username ) );
            }
            else if ( strcmp( key, CLIENT_ID ) == EQUAL_STR_CMP ) {
                strncpy( broker->client_id, value, sizeof( broker->client_id ) );
            }
            else if ( strcmp( key, TOPIC_ID ) == EQUAL_STR_CMP ) {
                strncpy( broker->topic, value, sizeof( broker->topic ) );
            }
            else if ( strcmp( key, QOS_ID ) == EQUAL_STR_CMP ) {
                broker->qos = atoi( value );
            }
            else if ( strcmp( key, RETAIN_ID ) == EQUAL_STR_CMP ) {
                broker->retain = atoi( value );
            }
            else if ( strcmp( key, KEEP_ALIVE_ID ) == EQUAL_STR_CMP ) {
                broker->alive = atoi( value );
            }
            else if ( strcmp( key, WILL_FLAG_ID ) == EQUAL_STR_CMP ) {
                broker->will_flag = atoi( value );
            }
            else if ( strcmp( key, WILL_RETAIN_ID ) == EQUAL_STR_CMP ) {
                broker->will_retain = atoi( value );
            }
            else if ( strcmp( key, WILL_QOS_ID ) == EQUAL_STR_CMP ) {
                broker->will_qos = atoi( value );
            }
            else if ( strcmp( key, CLEAN_SESSION_ID ) == EQUAL_STR_CMP ) {
                broker->clean_session = atoi( value );
            }
            else if ( strcmp( key, WILL_TOPIC_ID ) == EQUAL_STR_CMP ) {
                strncpy(broker->will_topic, value, sizeof( broker->will_topic ) );
            }
            else if ( strcmp( key, WILL_MSG_ID ) == EQUAL_STR_CMP ) {
                strncpy(broker->will_msg, value, sizeof( broker->will_msg ) );
            }
            else if ( strcmp( key, SEQUENCE_ID ) == EQUAL_STR_CMP ) {
                broker->sequence = atoi( value );
            }
            else if ( strcmp( key, OUTPUT_FILE_ID ) == EQUAL_STR_CMP ) {
                strncpy(conf->datafile_path, value, sizeof( conf->datafile_path ) );
            }
            else if ( strcmp( key, INPUT_FILE_ID ) == EQUAL_STR_CMP ) {
                strncpy(conf->msg_txt, value, sizeof( conf->msg_txt ) );
            }
            else if ( strcmp( key, CLIENT_LOOP_ID ) == EQUAL_STR_CMP ) {
                conf->client_loop_flg = atoi( value );
            }
            else if ( strcmp( key, PERSIST_CON_ID ) == EQUAL_STR_CMP ) {
                conf->persist_conn_time = atoi( value );
            }
            else if ( strcmp( key, ONLINE_MSG_ID ) == EQUAL_STR_CMP ) {
                conf->publish_online_flg = atoi( value );
            }
            else if ( strcmp( key, FILE_TRANS_ID ) == EQUAL_STR_CMP ) {
                if ( atoi( value ) ) {
                    conf->msg_type = CLI_DATAFILE_MSG;
                }
                else {
                    conf->msg_type = CLI_TXT_MSG;
                }
            }
        }
        fclose ( fileptr );
    }
    else {
        perror ( cfg_file );
        exit( CLI_ERROR );
    }

}

/**********************************************************************/
/**********************************************************************/
/** Get Key Value of line from Config file
 *
 * @param conf: structure that contains the user config options for broker conn.
 * @param key: key return name
 * @param value: return value
 *
 */
static void get_conf_value( char *line, char *key, char *value ) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char delim[] = "=";
    /*Get Key*/
    char *ptr = strtok(line, delim);
    strcpy( key, ptr);
    /*Get Value*/
    ptr = strtok(NULL, delim);
    /*Quit new line*/
    char *newline = strchr( ptr, '\n' );
    if ( newline )
        *newline = 0;
    strcpy( value, ptr);

}

/**********************************************************************/
/** Get Topic from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 * @param topic_ptr: ptr to topic to return
 *
 */
static uint16_t ecli_get_topic(const uint8_t* packet_buffer, const uint8_t **topic_ptr) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t  rem_len_bytes = 0;
    uint16_t topic_len     = 0;

    /***** Fixed header ****/
    /***********************/
    if( CLI_MSG_TYPE( packet_buffer ) == CLI_CTRLPKT_PUBLISH ) {
        rem_len_bytes = ecli_get_remain_len_b( packet_buffer );
        topic_len = CLI_LSHIFT_BYTE( *( packet_buffer + 1 + rem_len_bytes ) );
        topic_len |= *( packet_buffer + 1 + rem_len_bytes + 1 );
        *topic_ptr = ( packet_buffer + ( 1 + rem_len_bytes + 2 ) );
    } else {
        *topic_ptr = NULL;
    }

    return topic_len;
}

/**********************************************************************/
/** Get Message from mqtt packet
 *
 * @param packet_buffer: mqtt packet
 * @param msg_ptr: ptr to message to return
 *
 */
static uint32_t ecli_get_message(const uint8_t* packet_buffer, const uint8_t **msg_ptr) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t  offset        = 0;
    uint8_t  rem_len_bytes = 0;
    uint32_t msg_len       = 0;

    /***** Fixed header ****/
    /***********************/
    if(CLI_MSG_TYPE( packet_buffer ) == CLI_CTRLPKT_PUBLISH) {
        rem_len_bytes = ecli_get_remain_len_b( packet_buffer );
        offset = CLI_LSHIFT_BYTE( *( packet_buffer + 1 + rem_len_bytes ) );
        offset |= *( packet_buffer + 1 + rem_len_bytes + 1 );
        offset += ( 1 + rem_len_bytes + 2 );
        if( CLI_MSG_QOS( packet_buffer ) ) {
            offset += 2;
        }
        *msg_ptr = ( packet_buffer + offset );
        msg_len = ecli_get_remain_len( packet_buffer ) - ( offset - ( rem_len_bytes + 1 ) );
    }
    else {
        *msg_ptr = NULL;
    }

    return msg_len;
}
