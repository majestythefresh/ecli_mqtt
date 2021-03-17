/***********************************************************************
* FILENAME    :   libeclimqtt.c
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Library code to work with MQTT protocol.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <libeclimqtt.h>

/**********************************************************************/
/**********************************************************************/
/** Recv Connect CONNACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_connack(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Recv Subscribe SUBACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_suback(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Recv QOS1 PUBACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_puback(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Recv QOS2 PUBREC.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_pubrec(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Send QOS2 PUBREL.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
static uint8_t eclimqtt_pubrel(ecli_broker_t *broker);

/**********************************************************************/
/** Recv QOS2 PUBCOMP.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
static uint8_t eclimqtt_pubcomp(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/**********************************************************************/
/** Connect with broker.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_connect(ecli_broker_t *broker, ecli_conf_t *conf){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t  conn_flags       = CLI_EMPTY_BYTE;
    uint8_t  clientid_len     = strlen(broker->client_id);
    uint8_t  will_topic_len   = strlen(broker->will_topic);
    uint8_t  will_msg_len     = strlen(broker->will_msg);
    uint8_t  username_len     = strlen(broker->username);
    uint8_t  passwd_len       = strlen(broker->password);
    uint8_t  payload_len      = clientid_len + 2;
    uint8_t  fixed_header_len = MQTT_FIXED_HEADER_LEN;
    uint8_t  remain_value     = 0;
    uint8_t  return_code      = CLI_NO_ERROR;
    uint16_t packet_offset    = 0;

    /*****  Var header *****/
    /***********************/
    /* Set connection flags*/
    if(username_len) {
        payload_len += username_len + 2;
        conn_flags |= MQTT_USERNAME_FLAG;
    }
    if(passwd_len) {
        payload_len += passwd_len + 2;
        conn_flags |= MQTT_PASSWORD_FLAG;
    }
    if(broker->clean_session) {
        conn_flags |= MQTT_CLEAN_SESSION;
    }
    if(broker->will_flag) {
        conn_flags |= MQTT_WILL_FLAG;
        if(will_topic_len) {
            payload_len += will_topic_len + 2;
        }
        if(will_msg_len) {
            payload_len += will_msg_len + 2;
        }
        if(broker->will_retain) {
            conn_flags |= MQTT_WILL_RETAIN;
        }
        if(broker->will_qos == 1) {
            conn_flags |= MQTT_WILL_QOS1 ;
        }
        if(broker->will_qos == 2) {
            conn_flags |= MQTT_WILL_QOS2 ;
        }
    }
    uint8_t var_header[] = {
        MQTT_311_PROTOCOL_NAME,                /* Protocol name */
        MQTT_311_PROTOCOL_VER,                 /* Protocol version */
        conn_flags,                              /* Connect flags */
        CLI_RSHIFT_BYTE(broker->alive),        /* MSB Keep alive */
        broker->alive & CLI_BYTE,              /* LSB Keep alive */
    };

    /***** Fixed header ****/
    /***********************/
    uint32_t remain_len = sizeof( var_header ) + payload_len;
    /* Add extra byte for remain len */
    if ( remain_len > ( MQTT_REMAIN_LEN_2ND_BYTE - 1 ) ) {
        fixed_header_len++;
        if ( remain_len > ( MQTT_REMAIN_LEN_3RD_BYTE - 1) ) {
            fixed_header_len++;
            if ( remain_len > ( MQTT_REMAIN_LEN_4TH_BYTE - 1) ) {
                fixed_header_len++;
            }
        }
    }
    uint8_t fixed_header[fixed_header_len];
    /* First Byte : Msg Type */
    fixed_header[ packet_offset++ ] = MQTT_CTRLPKT_CONNECT;
    /*  From 2nd to 5th Byte :  Remaining Len */
    do{
        remain_value = remain_len % MQTT_REMAIN_LEN;
        remain_len = remain_len / MQTT_REMAIN_LEN;
        if (remain_len > 0){
            remain_value |= MQTT_REMAIN_LEN;
        }
        fixed_header[ packet_offset++ ] = remain_value;
    }
    while( remain_len > 0 );

    /***********************/
    /*******  Packet *******/
    /***********************/
    packet_offset = 0;
    uint8_t qmtt_packet[ sizeof( fixed_header ) + sizeof( var_header ) + payload_len ];
    memset( qmtt_packet, 0, sizeof( qmtt_packet ) );
    /* Bulk Fixed Header */
    memcpy( qmtt_packet, fixed_header, sizeof( fixed_header ) );
    packet_offset += sizeof(fixed_header);
    /* Bulk Var Header */
    memcpy( qmtt_packet + packet_offset, var_header, sizeof( var_header ) );
    packet_offset += sizeof( var_header );
    /* Bulk Payload : Client ID */
    qmtt_packet[ packet_offset++ ] = CLI_RSHIFT_BYTE(clientid_len);
    qmtt_packet[ packet_offset++ ] = clientid_len & CLI_BYTE ;
    memcpy( qmtt_packet + packet_offset, broker->client_id, clientid_len );
    packet_offset += clientid_len;
    if(broker->will_flag) {
        /* Bulk Payload : Will Topic */
        if(will_topic_len) {
            qmtt_packet[ packet_offset++ ] = CLI_RSHIFT_BYTE(will_topic_len);
            qmtt_packet[ packet_offset++ ] = will_topic_len & CLI_BYTE ;
            memcpy( qmtt_packet + packet_offset, broker->will_topic, will_topic_len );
            packet_offset += will_topic_len;
        }
        /* Bulk Payload : Will Message*/
        if(will_msg_len) {
            qmtt_packet[packet_offset++] = CLI_RSHIFT_BYTE(will_msg_len);
            qmtt_packet[packet_offset++] = will_msg_len & CLI_BYTE;
            memcpy(qmtt_packet + packet_offset, broker->will_msg, will_msg_len);
            packet_offset += will_msg_len;
        }
    }
    /* Bulk Payload : Username */
    if(username_len) {
        qmtt_packet[ packet_offset++ ] = CLI_RSHIFT_BYTE(username_len);
        qmtt_packet[ packet_offset++ ] = username_len & CLI_BYTE ;
        memcpy( qmtt_packet + packet_offset, broker->username, username_len );
        packet_offset += username_len;
    }
    /* Bulk Payload : User password */
    if(passwd_len) {
        qmtt_packet[packet_offset++] = CLI_RSHIFT_BYTE(passwd_len);
        qmtt_packet[packet_offset++] = passwd_len & CLI_BYTE;
        memcpy(qmtt_packet + packet_offset, broker->password, passwd_len);
        packet_offset += passwd_len;
    }

    /* Send Conn packet */
    if( broker->send_data( broker->socketid, ( void * ) qmtt_packet,
        ( int ) sizeof( qmtt_packet ) ) < sizeof( qmtt_packet ) ) {
        return CLI_BRK_CON_ERROR;
    }

    return_code = eclimqtt_connack(broker, conf);

    return return_code;
}

/**********************************************************************/
/** Publish a message to topic
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_publish(ecli_broker_t *broker, ecli_conf_t *conf, uint8_t first_msg_flag){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char     buffer_str[CLI_BUF_SIZE] = {0};
    uint8_t  qos_flag         = MQTT_PUBLISH_QOS0_FLAG;
    uint8_t  qos_size         = 0;
    uint8_t  remain_value     = 0;
    uint8_t  return_code      = CLI_NO_ERROR;
    uint16_t packet_offset    = 0;
    uint16_t topiclen         = strlen(broker->topic);
    uint32_t fixed_header_len = MQTT_FIXED_HEADER_LEN;
    uint32_t msg_len          = 0;
    FILE     *fileptr         = NULL;

    if ( first_msg_flag ) {
        msg_len = strlen( broker->retain_msg );
    }
    else {
        /*Type of Message [ text msg | datafile msg ]*/
        if ( conf->msg_type == CLI_DATAFILE_MSG ) {
            long     filelen;
            /*  Open file */
            fileptr = fopen(conf->msg_txt, "rb");
            if ( fileptr != NULL ) {
                fseek(fileptr, 0, SEEK_END);
                filelen = ftell(fileptr);
                rewind(fileptr);
                msg_len = filelen;
            }
            else{
                perror( conf->msg_txt );
                return CLI_FILE_ERROR;
            }
        }
        else if ( conf->msg_type == CLI_TXT_MSG ) {
            msg_len = strlen( conf->msg_txt );
        }
    }
    /* Check max size */
    if ( msg_len > CLI_MAX_MSG_SIZE ){
        return CLI_PUBLISH_SIZE_ERROR;
    }
    uint8_t msg_buffer[msg_len];
    if ( first_msg_flag ) {
        memset( msg_buffer, 0, msg_len );
        memcpy( msg_buffer, broker->retain_msg, msg_len );
    }
    else {
        /*Type of Message [ text msg | datafile msg ]*/
        if ( conf->msg_type == CLI_DATAFILE_MSG ) {
            /*  Read in the entire file */
            fread(msg_buffer, msg_len, 1, fileptr);
            fclose(fileptr);
        }
        else if ( conf->msg_type == CLI_TXT_MSG ) {
            memset( msg_buffer, 0, msg_len );
            memcpy( msg_buffer, conf->msg_txt, msg_len );
        }
    }

    if(broker->qos == 1) {
        qos_size = 2; // 2 bytes for QoS
        qos_flag = MQTT_PUBLISH_QOS1_FLAG;
    }
    else if(broker->qos == 2) {
        qos_size = 2; // 2 bytes for QoS
        qos_flag = MQTT_PUBLISH_QOS2_FLAG;
    }

    /***** Var. header *****/
    /***********************/
    uint8_t var_header[ topiclen + 2 + qos_size ];
    memset( var_header, 0, sizeof( var_header ) );
    var_header[0] = CLI_RSHIFT_BYTE(topiclen);
    var_header[1] = topiclen & CLI_BYTE;
    memcpy( var_header + 2, broker->topic, topiclen );
    if( qos_size ) {
        var_header[ topiclen + 2 ] = CLI_RSHIFT_BYTE(broker->sequence);
        var_header[ topiclen + 3 ] = broker->sequence & CLI_BYTE;
        if(&broker->msg_id) {
            broker->msg_id = broker->sequence;
        }
        broker->sequence++;
    }

    /***** Fixed header ****/
    /***********************/
    uint32_t remain_len = sizeof( var_header ) + msg_len;
    /* Add extra byte for remain len */
    if ( remain_len > ( MQTT_REMAIN_LEN_2ND_BYTE - 1 ) ) {
        fixed_header_len++;
        if ( remain_len > ( MQTT_REMAIN_LEN_3RD_BYTE - 1) ) {
            fixed_header_len++;
            if ( remain_len > ( MQTT_REMAIN_LEN_4TH_BYTE - 1) ) {
                fixed_header_len++;
            }
        }
    }
    uint8_t fixed_header[fixed_header_len];
    /* First Byte : Msg Type */
    fixed_header[ packet_offset ] = MQTT_CTRLPKT_PUBLISH | qos_flag;
    if( broker->retain || first_msg_flag ) {
        fixed_header[ packet_offset ] |= MQTT_PUBLISH_RETAIN_FLAG;
    }
    packet_offset++;
    /*  From 2nd to 5th Byte :  Remaining Len */
    do{
        remain_value = remain_len % MQTT_REMAIN_LEN;
        remain_len = remain_len / MQTT_REMAIN_LEN;
        if (remain_len > 0){
            remain_value |= MQTT_REMAIN_LEN;
        }
        fixed_header[ packet_offset++ ] = remain_value;
    }
    while( remain_len > 0 );

    /***********************/
    /*******  Packet *******/
    /***********************/
    uint32_t packet_size = sizeof( fixed_header ) + sizeof( var_header ) + msg_len ;
    uint8_t mqtt_packet[packet_size];
    memset( mqtt_packet, 0, packet_size );
    memcpy( mqtt_packet, fixed_header, sizeof( fixed_header ) );
    memcpy( mqtt_packet + sizeof(fixed_header ), var_header, sizeof( var_header ) );
    memcpy( mqtt_packet + sizeof( fixed_header ) + sizeof( var_header ), msg_buffer, msg_len );

    /* Send Publish packet */
    eclilog_show(__FILE__, __func__, PUBLISH_MSG, LOG_DEBUG);
    sprintf(buffer_str, PUB_MSGLEN_MSG, msg_len);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
    sprintf(buffer_str, PUB_PKTLEN_MSG, packet_size);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
    if( ( broker->send_data( broker->socketid,
                           ( const void * ) mqtt_packet,
                           packet_size ) ) < packet_size ) {
        return CLI_PUBLISH_ERROR;
    }
    sprintf(buffer_str, PUBLISHED_MSG, packet_size);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);

    if( broker->qos == 1 ) {
        return_code = eclimqtt_puback(broker, conf);
    }
    else if( broker->qos == 2 ) {
        if ( ( return_code = eclimqtt_pubrec( broker, conf ) ) == CLI_NO_ERROR ) {
            if ( ( return_code = eclimqtt_pubrel( broker ) ) == CLI_NO_ERROR ) {
                return_code = eclimqtt_pubcomp(broker, conf);
            }
        }
    }

    return return_code;
}

/**********************************************************************/
/** Publish a message as byte array chunk
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 * @param msg_buffer: byte array or chunk with message.
 * @param msg_len: chunk len.
 *
 */
uint8_t eclimqtt_publish_chunk(ecli_broker_t *broker, ecli_conf_t *conf, const uint8_t *msg_buffer, uint32_t msg_len){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    char     buffer_str[CLI_BUF_SIZE] = {0};
    uint8_t  qos_flag         = MQTT_PUBLISH_QOS0_FLAG;
    uint8_t  qos_size         = 0;
    uint8_t  remain_value     = 0;
    uint8_t  return_code      = CLI_NO_ERROR;
    uint16_t packet_offset    = 0;
    uint16_t topiclen         = strlen(broker->topic);
    uint32_t fixed_header_len = MQTT_FIXED_HEADER_LEN;

    /* Check max size */
    if ( msg_len > MAX_CHUNK_SIZE ){
        return CLI_PUBLISH_SIZE_ERROR;
    }

    if(broker->qos == 1) {
        qos_size = 2; // 2 bytes for QoS
        qos_flag = MQTT_PUBLISH_QOS1_FLAG;
    }
    else if(broker->qos == 2) {
        qos_size = 2; // 2 bytes for QoS
        qos_flag = MQTT_PUBLISH_QOS2_FLAG;
    }

    /***** Var. header *****/
    /***********************/
    uint8_t var_header[ topiclen + 2 + qos_size ];
    memset( var_header, 0, sizeof( var_header ) );
    var_header[0] = CLI_RSHIFT_BYTE(topiclen);
    var_header[1] = topiclen & CLI_BYTE;
    memcpy( var_header + 2, broker->topic, topiclen );
    if( qos_size ) {
        var_header[ topiclen + 2 ] = CLI_RSHIFT_BYTE(broker->sequence);
        var_header[ topiclen + 3 ] = broker->sequence & CLI_BYTE;
        if(&broker->msg_id) {
            broker->msg_id = broker->sequence;
        }
        broker->sequence++;
    }

    /***** Fixed header ****/
    /***********************/
    uint32_t remain_len = sizeof( var_header ) + msg_len;
    /* Add extra byte for remain len */
    if ( remain_len > ( MQTT_REMAIN_LEN_2ND_BYTE - 1 ) ) {
        fixed_header_len++;
        if ( remain_len > ( MQTT_REMAIN_LEN_3RD_BYTE - 1) ) {
            fixed_header_len++;
            if ( remain_len > ( MQTT_REMAIN_LEN_4TH_BYTE - 1) ) {
                fixed_header_len++;
            }
        }
    }
    uint8_t fixed_header[fixed_header_len];
    /* First Byte : Msg Type */
    fixed_header[ packet_offset ] = MQTT_CTRLPKT_PUBLISH | qos_flag;
    if( broker->retain ) {
        fixed_header[ packet_offset ] |= MQTT_PUBLISH_RETAIN_FLAG;
    }
    packet_offset++;
    /*  From 2nd to 5th Byte :  Remaining Len */
    do{
        remain_value = remain_len % MQTT_REMAIN_LEN;
        remain_len = remain_len / MQTT_REMAIN_LEN;
        if (remain_len > 0){
            remain_value |= MQTT_REMAIN_LEN;
        }
        fixed_header[ packet_offset++ ] = remain_value;
    }
    while( remain_len > 0 );

    /***********************/
    /*******  Packet *******/
    /***********************/
    uint32_t packet_size = sizeof( fixed_header ) + sizeof( var_header ) + msg_len ;
    uint8_t mqtt_packet[packet_size];
    memset( mqtt_packet, 0, packet_size );
    memcpy( mqtt_packet, fixed_header, sizeof( fixed_header ) );
    memcpy( mqtt_packet + sizeof(fixed_header ), var_header, sizeof( var_header ) );
    memcpy( mqtt_packet + sizeof( fixed_header ) + sizeof( var_header ), msg_buffer, msg_len );

    /* Send Publish packet */
    eclilog_show(__FILE__, __func__, PUBLISH_MSG, LOG_DEBUG);
    sprintf(buffer_str, PUB_MSGLEN_MSG, msg_len);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
    sprintf(buffer_str, PUB_PKTLEN_MSG, packet_size);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);
    if( ( broker->send_data( broker->socketid,
                           ( const void * ) mqtt_packet,
                           packet_size ) ) < packet_size ) {
        return CLI_PUBLISH_ERROR;
    }
    sprintf(buffer_str, PUBLISHED_MSG, packet_size);
    eclilog_show(__FILE__, __func__, buffer_str, LOG_DEBUG);

    if( broker->qos == 1 ) {
        return_code = eclimqtt_puback(broker, conf);
    }
    else if( broker->qos == 2 ) {
        if ( ( return_code = eclimqtt_pubrec( broker, conf ) ) == CLI_NO_ERROR ) {
            if ( ( return_code = eclimqtt_pubrel( broker ) ) == CLI_NO_ERROR ) {
                return_code = eclimqtt_pubcomp(broker, conf);
            }
        }
    }

    return return_code;
}

/**********************************************************************/
/** Subscribe to topic.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_subscribe(ecli_broker_t *broker, ecli_conf_t *conf){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t  return_code = CLI_NO_ERROR;
    uint16_t topiclen    = strlen(broker->topic);

    /***** Var. header *****/
    /***********************/
    /*Message ID*/
    uint8_t var_header[2];
    memset( var_header, 0, sizeof( var_header ) );
    var_header[0] = CLI_RSHIFT_BYTE(broker->sequence);
    var_header[1] = broker->sequence & CLI_BYTE;
    if(&broker->msg_id) {
        broker->msg_id = broker->sequence;
    }
    broker->sequence++;

    /******** Topic ********/
    /***********************/
    uint8_t topic[ topiclen + 3 ];
    memset( topic, 0, sizeof( topic ) );
    topic[0] = CLI_RSHIFT_BYTE(topiclen);
    topic[1] = topiclen & CLI_BYTE;
    memcpy( topic + 2, broker->topic, topiclen);

    /***** Fixed header ****/
    /***********************/
    uint8_t fixed_header[] = {
        MQTT_CTRLPKT_SUBSCRIBE | MQTT_SUBSCRIBE_FLAG,
        sizeof( var_header ) + sizeof( topic )
    };

    /***********************/
    /******** Packet *******/
    /***********************/
    uint8_t mqtt_packet[ sizeof( var_header ) + sizeof( fixed_header ) + sizeof( topic ) ];
    memset( mqtt_packet, 0, sizeof( mqtt_packet ) );
    memcpy( mqtt_packet, fixed_header, sizeof( fixed_header ) );
    memcpy( mqtt_packet + sizeof( fixed_header ), var_header, sizeof( var_header ) );
    memcpy( mqtt_packet + sizeof( fixed_header ) + sizeof( var_header ), topic, sizeof( topic ) );

    /* Send Subs packet */
    if(broker->send_data(broker->socketid, mqtt_packet, sizeof( mqtt_packet ) ) < sizeof( mqtt_packet ) ) {
        return CLI_SUB_SEND_ERROR;
    }

    return_code = eclimqtt_suback(broker, conf);

    return return_code;
}

/**********************************************************************/
/** Send hearbeat to Broker.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
uint8_t eclimqtt_pingreq(ecli_broker_t *broker) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t mqtt_packet[] = { MQTT_CTRLPKT_PINGREQ, 0x00 };

    // Send the packet
    if(broker->send_data(broker->socketid, mqtt_packet, sizeof(mqtt_packet)) < sizeof(mqtt_packet)) {
        return CLI_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Send Disconnect message to Broker.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
uint8_t eclimqtt_disconnect(ecli_broker_t *broker){
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t mqtt_packet[] = { MQTT_CTRLPKT_DISCONNECT, 0x00 };

    if(broker->send_data(broker->socketid, mqtt_packet, sizeof(mqtt_packet)) < sizeof(mqtt_packet)) {
        return CLI_BRK_DISCONNECT_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/**********************************************************************/
/** Recv Connect CONNACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_connack(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    if( ecli_read_header( broker, conf ) == CLI_ERROR ){
        return CLI_BRK_CON_READ_ERROR;
    }
    if( MQTT_MSG_TYPE( conf->packet_buffer ) != MQTT_CTRLPKT_CONNACK ) {
      /* Session Present Flag. */
      switch (conf->packet_buffer[2]) {
      /*Session Present*/
        case 0x01:
          return CLI_CONN_SESS_PRE;
        break;
        default:
          return CLI_BRK_CON_EXP_ERROR;
        break;
        }
    }
    if( conf->packet_buffer[3] != MQTT_CONNACK_FLAG ) {
      /*Connection Refused, verify which cases*/
      switch (conf->packet_buffer[3]) {
        /*Unacceptable protocol version*/
        case 0x01:
          return CLI_UNACC_PRO_VER;
          break;
        /*Identifier rejected*/
        case 0x02:
          return CLI_IDEN_REJEC;
          break;
        /*Server unavailable*/
        case 0x03:
          return CLI_SERVER_UNAVAI;
          break;
        /*Bad user name or password*/
        case 0x04:
          return CLI_USER_PASS_BAD;
          break;
        /*Not authorized*/
        case 0x05:
          return CLI_NOT_AUTH;
          break;
        default:
          return CLI_BRK_CON_ACK_ERROR;
          break;
      }
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Recv Subscribe SUBACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_suback(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    if( ecli_read_header( broker, conf ) == CLI_ERROR ){
        return CLI_SUB_READ_ERROR;
    }
    if( MQTT_MSG_TYPE( conf->packet_buffer ) != MQTT_CTRLPKT_SUBACK ) {
        return CLI_SUB_ACK_ERROR;
    }
    uint16_t msg_id_rcv = ecli_get_msg_id(conf->packet_buffer);
    if(broker->msg_id != msg_id_rcv)
    {
        return CLI_SUB_MSGID_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Recv QOS1 PUBACK
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_puback(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    if( ecli_read_header( broker, conf ) == CLI_ERROR ){
        return CLI_PUB1_READ_ERROR;
    }
    if( MQTT_MSG_TYPE( conf->packet_buffer ) != MQTT_CTRLPKT_PUBACK ) {
        return CLI_PUB1_ACK_ERROR;
    }
    uint16_t msg_id_rcv = ecli_get_msg_id(conf->packet_buffer);
    if(broker->msg_id != msg_id_rcv)
    {
        return CLI_PUB1_MSGID_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Recv QOS2 PUBREC.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 */
static uint8_t eclimqtt_pubrec(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    if( ecli_read_header( broker, conf ) == CLI_ERROR ){
        return CLI_PUB2_REC_READ_ERROR;
    }
    if( MQTT_MSG_TYPE( conf->packet_buffer ) != MQTT_CTRLPKT_PUBREC ) {
        return CLI_PUB2_REC_ACK_ERROR;
    }
    uint16_t msg_id_rcv = ecli_get_msg_id(conf->packet_buffer);
    if(broker->msg_id != msg_id_rcv)
    {
        return CLI_PUB2_REC_MSGID_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Send QOS2 PUBREL.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
static uint8_t eclimqtt_pubrel(ecli_broker_t *broker) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    uint8_t mqtt_packet[] = {
        MQTT_CTRLPKT_PUBREL | MQTT_PUBREL_FLAG, 0x02,
        CLI_RSHIFT_BYTE( broker->msg_id ), broker->msg_id & CLI_BYTE
    };

    if(broker->send_data( broker->socketid, mqtt_packet, sizeof( mqtt_packet ) ) < sizeof( mqtt_packet ) ) {
        return CLI_ERROR;
    }

    return CLI_NO_ERROR;
}

/**********************************************************************/
/** Recv QOS2 PUBCOMP.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
static uint8_t eclimqtt_pubcomp(ecli_broker_t *broker, ecli_conf_t *conf) {
    eclilog_show(__FILE__, __func__, "", LOG_TRACE);

    if( ecli_read_header( broker, conf ) == CLI_ERROR ){
        return CLI_PUB2_COMP_READ_ERROR;
    }
    if( MQTT_MSG_TYPE( conf->packet_buffer ) != MQTT_CTRLPKT_PUBCOMP ) {
        return CLI_PUB2_COMP_ACK_ERROR;
    }
    uint16_t msg_id_rcv = ecli_get_msg_id(conf->packet_buffer);
    if(broker->msg_id != msg_id_rcv)
    {
        return CLI_PUB2_COMP_MSGID_ERROR;
    }

    return CLI_NO_ERROR;
}
