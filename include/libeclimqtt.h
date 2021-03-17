/***********************************************************************
* FILENAME    :   libeclimqtt.h
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Library functions to work with MQTT protocol.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <libeclimqttclient.h>

/**********************************************************************/

#ifndef LIBECLIMQTT_H_
#define LIBECLIMQTT_H_

/* CONN */
/* Protocol name: MQIsdp */
#define MQTT_31_PROTOCOL_NAME      0x00,0x06,0x4d,0x51,0x49,0x73,0x64,0x70
#define MQTT_31_PROTOCOL_VER       0x03       /* MQTT 3.1 */
/* Protocol name: MQTT */
#define MQTT_311_PROTOCOL_NAME     0x00,0x04,0x4d,0x51,0x54,0x54
#define MQTT_311_PROTOCOL_VER      0x04       /* MQTT 3.1.1 */

#define MQTT_FIXED_HEADER_LEN      2          /* bytes */
/* CONN FLAGS */
#define MQTT_CLEAN_SESSION         1<<1       /* 0000 0010 */
#define MQTT_WILL_FLAG             1<<2       /* 0000 0100 */
#define MQTT_WILL_QOS1             1<<3       /* 0000 1000 */
#define MQTT_WILL_QOS2             1<<4       /* 0001 0000 */
#define MQTT_WILL_RETAIN           1<<5       /* 0010 0000 */
#define MQTT_PASSWORD_FLAG         1<<6       /* 0100 0000 */
#define MQTT_USERNAME_FLAG         1<<7       /* 1000 0000 */
/********************************/
/**** FIXED HEADER - 2 BYTES ****/
/********************************/
/* FIXED HEADER CONTROL PACKET TYPES - FIRST BYTE */
#define MQTT_CTRLPKT_CONNECT       1<<4       /* 0001 0000 */
#define MQTT_CTRLPKT_CONNACK       2<<4       /* 0010 0000 */
#define MQTT_CTRLPKT_PUBLISH       3<<4       /* 0011 0000 */
#define MQTT_CTRLPKT_PUBACK        4<<4       /* 0100 0000 */
#define MQTT_CTRLPKT_PUBREC        5<<4       /* 0101 0000 */
#define MQTT_CTRLPKT_PUBREL        6<<4       /* 0110 0000 */
#define MQTT_CTRLPKT_PUBCOMP       7<<4       /* 0111 0000 */
#define MQTT_CTRLPKT_SUBSCRIBE     8<<4       /* 1000 0000 */
#define MQTT_CTRLPKT_SUBACK        9<<4       /* 1001 0000 */
#define MQTT_CTRLPKT_UNSUBSCRIBE  10<<4       /* 1010 0000 */
#define MQTT_CTRLPKT_UNSUBACK     11<<4       /* 1011 0000 */
#define MQTT_CTRLPKT_PINGREQ      12<<4       /* 1100 0000 */
#define MQTT_CTRLPKT_PINGRESP     13<<4       /* 1101 0000 */
#define MQTT_CTRLPKT_DISCONNECT   14<<4       /* 1110 0000 */
/* FIXED HEADER FLAGS TO CONTROL PACKET TYPES  - SECOND BYTE */
#define MQTT_CONNECT_FLAG             0       /* 0000 0000 */
#define MQTT_CONNACK_FLAG             0       /* 0000 0000 */
#define MQTT_PUBLISH_DUP_FLAG      1<<3       /* 0000 1000 */
#define MQTT_PUBLISH_QOS0_FLAG     0<<1       /* 0000 0000 */
#define MQTT_PUBLISH_QOS1_FLAG     1<<1       /* 0000 0010 */
#define MQTT_PUBLISH_QOS2_FLAG     2<<1       /* 0000 0100 */
#define MQTT_PUBLISH_RETAIN_FLAG      1       /* 0000 0001 */
#define MQTT_PUBACK_FLAG              0       /* 0000 0000 */
#define MQTT_PUBREC_FLAG              0       /* 0000 0000 */
#define MQTT_PUBREL_FLAG           1<<1       /* 0000 0010 */
#define MQTT_PUBCOMP_FLAG             0       /* 0000 0000 */
#define MQTT_SUBSCRIBE_FLAG        1<<1       /* 0000 0010 */
#define MQTT_SUBACK_FLAG              0       /* 0000 0000 */
#define MQTT_UNSUBSCRIBE_FLAG      1<<1       /* 0000 0010 */
#define MQTT_UNSUBACK_FLAG            0       /* 0000 0000 */
#define MQTT_PINGREQ_FLAG             0       /* 0000 0000 */
#define MQTT_PINGRESP_FLAG            0       /* 0000 0000 */
#define MQTT_DISCONNECT_FLAG          0       /* 0000 0000 */
/* Remaining Len */
#define MQTT_REMAIN_LEN               0x80    /* 128 */
#define MQTT_REMAIN_LEN_2ND_BYTE      128     /* Min Value to use 2nd Byte */
#define MQTT_REMAIN_LEN_3RD_BYTE      16384   /* Min Value to use 3rd Byte */
#define MQTT_REMAIN_LEN_4TH_BYTE      2097152 /* Min Value to use 4th Byte */
/* MQTT TYPES */
#define MQTT_MSG_TYPE( packet_buffer ) ( ( *packet_buffer & 0xF0 ) )
#define MQTT_QOS_TYPE( packet_buffer ) ( ( *packet_buffer & 0x06 ) >> 1 )

/**********************************************************************/
/** Set connection data & options
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_connect(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Publish a message to topic
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_publish(ecli_broker_t *broker, ecli_conf_t *conf, uint8_t first_msg_flag);

/**********************************************************************/
/** Publish a message as byte stream
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 * @param msg_buffer: byte array or chunk with message.
 * @param msg_len: chunk len.
 *
 */
uint8_t eclimqtt_publish_data(ecli_broker_t *broker, ecli_conf_t *conf, const uint8_t *msg_buffer, uint32_t msg_len);

/**********************************************************************/
/** Subscribe to topic.
 *
 * @param broker: structure that contains the client connection info with broker
 * @param conf: structure that contains the user config options for broker conn.
 *
 */
uint8_t eclimqtt_subscribe(ecli_broker_t *broker, ecli_conf_t *conf);

/**********************************************************************/
/** Send hearbeat to Broker.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
uint8_t eclimqtt_pingreq(ecli_broker_t* broker);

/**********************************************************************/
/** Send Disconnect message to Broker.
 *
 * @param broker: structure that contains the client connection info with broker
 *
 */
uint8_t eclimqtt_disconnect(ecli_broker_t *broker);

#endif
