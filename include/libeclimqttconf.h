/***********************************************************************
* FILENAME    :   libeclimqttconf.h
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   Constants to avoid hardcoding values in config.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#ifndef LIBECLIMQTTCONST_H_
#define LIBECLIMQTTCONST_H_

/**********************************************************************/
#define FALSE_FLAG            0
#define TRUE_FLAG             1
#define EQUAL_STR_CMP         0
/*Default Conf. Values*/
#define CONFIGFILE_DEFAULT    "conf/mqtt.conf"
#define BROKER_IP_DEFAULT     "127.0.0.1"
#define USERNAME_DEFAULT      "usertest"
#define PASSWORD_DEFAULT      "passwdtest"
#define TOPIC_DEFAULT         "mqtt/test"
#define CLIENTID_DEFAULT      "mqtt"
#define TXT_MSG_DEFAULT       "Hello - MQTT Protocol!"
#define OUT_FILE_DEFAULT      "output/recv_file"
#define WILL_TOPIC_DEFAULT    TOPIC_DEFAULT
#define WILL_MSG_DEFAULT      "pub:offline"
#define WILL_MSG_CONN_DEFAULT "pub:online"
#define QOS_DEFAULT           0
#define RETAIN_DEFAULT        FALSE_FLAG
#define WILL_QOS_DEFAULT      0
#define WILL_RETAIN_DEFAULT   FALSE_FLAG
#define WILL_FLAG_DEFAULT     FALSE_FLAG
#define ALIVE_CON_DEFAULT     300
#define ALIVE_PING_DEFAULT    30
#define CLEAN_SESSION_DEFAULT FALSE_FLAG
#define SEQUENCE_DEFAULT      1
#define FILE_TRANS_DEFAULT    FALSE_FLAG
#define READ_LOOP_DEFAULT     FALSE_FLAG
#define PUBONLINE_FLG_DEFAULT FALSE_FLAG
#define CFG_FILE_FLAG_DEFAULT FALSE_FLAG
#define BROKER_PORT_DEFAULT   1883
#define PERSIST_CON_DEFAULT   0
/* bytes (MQTT support up to 256Mb)*/
//#define MAX_MSG_SIZE          268435456  /* 256MB for File messages */
#define MAX_MSG_SIZE          4194304   /* 4MB for File messages */
#define MAX_TXT_MSG_SIZE      1024      /* 1KB for Text messages */
#define MAX_CHUNK_SIZE        102400    /* 100KB for chunk to transfer a file*/
/*Config File IDs*/
#define BROKER_IP_ID          "broker_ip"
#define BROKER_PORT_ID        "broker_port"
#define BROKER_USER_ID        "broker_user"
#define BROKER_PASSWD_ID      "broker_passwd"
#define CLIENT_ID             "client_id"
#define TOPIC_ID              "topic"
#define QOS_ID                "qos"
#define RETAIN_ID             "retain"
#define KEEP_ALIVE_ID         "alive"
#define WILL_FLAG_ID          "will_flag"
#define WILL_QOS_ID           "will_qos"
#define WILL_RETAIN_ID        "will_retain"
#define CLEAN_SESSION_ID      "clean_session"
#define WILL_TOPIC_ID         "will_topic"
#define WILL_MSG_ID           "will_msg"
#define SEQUENCE_ID           "sequence"
#define OUTPUT_FILE_ID        "output_file"
#define INPUT_FILE_ID         "input_file"
#define CLIENT_LOOP_ID        "client_loop"
#define ONLINE_MSG_ID         "publish_first_online"
#define PERSIST_CON_ID        "persist_conn_time"
#define FILE_TRANS_ID         "file_trans"
/* Messages */
#define CONN_TRY_MSG          "-- Trying to connect to broker server %s:%d..."
#define CONNECTED_MSG          "Connected with broker %s:%d."
#define CFG_FILE_MSG          "Using Config File [%s]..."
#define TOPIC_MSG             "Topic: [%s] - "
#define MSG_LEN_MSG           "Message Len: [%d] - "
#define DATA_MSG              "Data: [ File ]"
#define MESSAGE_MSG           "Message: [%s]"
#define PUBLISH_MSG           "Publishing..."
#define PUB_PKTLEN_MSG        "Packet Len [%d] bytes"
#define PUB_MSGLEN_MSG        "Message Len [%d] bytes"
#define PUBLISHED_MSG         "Published: Packet Len [%d] bytes"
#define PING_MSG              "Sending Ping..."
#define SIGINT_MSG            "Closed by SIGNAl %d"
/* Error Msg */
#define UNKNOW_OPT_ERROR      "Unknown option character %c"
#define CONN_BRR_ERROR        "Broken connection with Broker"
#define SOCK_ERROR            "Socket creation error: %s"
#define SET_SOCK_OPTS_ERROR   "Socket options error: %s"
#define CON_ERROR             "Server connection error: %s"
#define BRK_CON_ERROR         "Broker connection error: %s"
#define BRK_CON_READ_ERROR    "Error - Reading packet from Broker connection: %s"
#define BRK_CON_EXP_ERROR     "Error - Expected ACK reading packet from Broker connection: %s"
#define BRK_CON_ACK_ERROR     "Error - Connection ACK from Broker failed : %s"
#define BRK_DISCONNECT_ERROR  "Disconnect from Broker - Error : %s"
#define PUBLISH_ERROR         "Error - Publishing to Broker : %s"
#define PUBLISH_SIZE_ERROR    "Error - Publishing a message bigger than limit"
#define SUB_SEND_ERROR        "Error sending packet - Subscribing to Broker : %s"
#define PUB1_READ_ERROR       "Error PUBACK reading packet - Publishing QOS1 to Broker : %s"
#define PUB1_ACK_ERROR        "Error PUBACK reading ACK - Publishing QOS1 to Broker : %s"
#define PUB1_MSGID_ERROR      "Error PUBACK reading Msg ID - Publishing QOS1 to Broker : %s"
#define PUB2_REC_READ_ERROR   "Error PUBREC reading packet - Publishing QOS2 to Broker : %s"
#define PUB2_REC_ACK_ERROR    "Error PUBREC reading ACK - Publishing QOS2 to Broker : %s"
#define PUB2_REC_MSGID_ERROR  "Error PUBREC reading Msg ID - Publishing QOS2 to Broker : %s"
#define PUB2_COMP_READ_ERROR  "Error PUBCOMP reading packet - Publishing QOS2 to Broker : %s"
#define PUB2_COMP_ACK_ERROR   "Error PUBCOMP reading ACK - Publishing QOS2 to Broker : %s"
#define PUB2_COMP_MSGID_ERROR "Error PUBCOMP reading Msg ID - Publishing QOS2 to Broker : %s"
#define SUB_READ_ERROR        "Error SUBACK reading packet - Subscribing to Broker : %s"
#define SUB_ACK_ERROR         "Error SUBACK reading ACK - Subscribing to Broker : %s"
#define SUB_MSGID_ERROR       "Error SUBACK reading Msg ID - Subscribing to Broker : %s"
#define READ_SIZE_ERROR       "Error - Reading a message bigger than limit"
#define UNKNOW_ERROR          "Unknown error: %s"
#define OPEN_FILE_ERROR       "Error - Opening file"
#define READ_TIMEOUT_ERROR    "Reading message Timeout..."
#define CONN_SESS_PRE         "Warning - Session present CONNACK"    /*Connection shall be established*/
#define UKNOW_FLAG_CONN       "Error - Unknown case CONNACK"
#define UNACC_PRO_VER         "Error - Unacceptable protocol version CONACK"
#define IDEN_REJEC            "Error - Identifier rejected CONNACK"
#define SERVER_UNAVAI         "Error - Server unavailable CONNACK"
#define USER_PASS_BAD         "Error - Bad user name or password CONNACK"
#define NOT_AUTH              "Error - Not authorized CONNACK"

/*PUB & SUB command options*/
#define HELP_TXT              "\n \
 Publisher Usage: \n\n \
       ecli_mqtt_pub -option value -flag\n\n\
            Options:\n\n\
              -b : Broker IP (default %s)\n\
              -p : Broker Port (default %d)\n\
              -u : Broker Username (default %s)\n\
              -k : Broker Password (default %s)\n\
              -i : Client ID (default %s)\n\
              -t : Topic to publish (default %s)\n\
              -m : Message to tranfer [ msg text | path to file with -f only ] (default %s)\n\
              -q : Quality of Service to publish (default QOS %d)\n\
              -a : Keep Alive (default %d)\n\
              -c : Use Configuration File\n\
              -Q : Will Quality of Service (default %d)\n\
              -T : Will Topic (default %s)\n\
              -M : Will Message (default %s)\n\
              -P : Time in seconds to wait connect to broker (default %d secs)\n\
              -h : Show help\n\n\
            Flags:\n\n\
              -l : flag to publish messages in loop (default no loop)\n\
              -f : Select file transfer flag (default no file transfer)\n\
              -r : Retain publish flag (default no retain)\n\
              -R : Retain connection flag (default no retain)\n\
              -W : Will connection flag (default no flag)\n\
              -C : Clean Session connection flag (default no clean session)\n\
              -O : First Online Message flag (default no no online retain message)\n\
 \n\n\
 Subscriber Usage: \n\n \
       ecli_mqtt_sub -option value -flag \n\n\
            Options:\n\n\
              -b : Broker IP (default %s)\n\
              -p : Broker Port (default %d)\n\
              -u : Broker Username (default %s)\n\
              -k : Broker Password (default %s)\n\
              -i : Client ID (default %s)\n\
              -t : Topic to subscribe (default %s)\n\
              -o : Output file with -f only (default %s)\n\
              -a : Keep Alive (default %d)\n\
              -c : Use Configuration File \n\
              -Q : Will Quality of Service (default %d)\n\
              -T : Will Topic (default %s)\n\
              -M : Will Message (default %s)\n\
              -P : Time in seconds to wait connect to broker (default %d secs)\n\
              -h : Show help\n\n\
            Flags:\n\n\
              -l : flag to read messages in loop (default no loop)\n\
              -f : Select file receive flag (default no file receive)\n\
              -R : Retain connection flag (default no retain)\n\
              -W : Will connection flag (default no flag)\n\
              -C : Clean Session connection flag (default no clean session)\n\
              -O : First Online Message flag (default no no online retain message)\n\
  \n\n\
Examples:\n\
\n\n\
Subscriber:\n\
    - Subscribe to topic in broker with default values, to receive text messages.\n\
      $ ecli_mqtt_sub -i client-id-1 -t devices/ID/sensor1\n\
    - Subscribe to topic in a specific broker, to receive text messages.\n\
      $ ecli_mqtt_sub -b 192.168.125.11 -p 1883 -i client-id-2 -t devices/ID/sensor1\n\
    - Subscribe to topic in a specific broker with credentials, to receive text messages.\n\
      $ ecli_mqtt_sub -b 192.168.125.11 -p 1883 -u usertest -k passwdtest -i client-id-3 -t devices/ID/sensor1\n\
    - Subscribe to topic, to receive text messages and set flag to receive in a loop.\n\
      $ ecli_mqtt_sub -i client-id-4 -t devices/ID/sensor1 -l\n\
    - Subscribe to topic, to receive text messages using a configuration file.\n\
      $ ecli_mqtt_sub -i client-id-5 -t devices/ID/sensor1 -c conf/client_mqtt.conf\n\
    - Subscribe to topic, to receive file messages.\n\
      $ ecli_mqtt_sub -i client-id-6 -t devices/ID/camera -f\n\
    - Subscribe to topic, to receive file messages and set path to receive it.\n\
      $ ecli_mqtt_sub -i client-id-7 -t devices/ID/camera -f -o -tmp/recv_image.jpg\n\
    - Subscribe to topic, to receive file messages and set path to receive it in a loop.\n\
      $ ecli_mqtt_sub -i client-id-8 -t devices/ID/camera -f -o -tmp/recv_image.jpg -l\n\
\n\n\
Publisher:\n\
    - Publish text message to topic in broker with default values.\n\
      $ ecli_mqtt_pub -i client-id-9 -t devices/ID/sensor1 -m \"Temperature: 30 C\"\n\
    - Publish text message to topic in broker with default values and set flag to receive in a loop.\n\
      $ ecli_mqtt_pub -i client-id-10 -t devices/ID/sensor1 -m \"Temperature: current C\" -l\n\
    - Publish text message to topic in a specific broker, to receive text messages.\n\
      $ ecli_mqtt_pub -b 192.168.125.11 -p 1883 -i client-id-11 -t devices/ID/sensor1 -m \"Temperature: 30 C\"\n\
    - Publish text message to topic in a specific broker with credentials, to receive text messages.\n\
      $ ecli_mqtt_pub -b 192.168.125.11 -p 1883 -u usertest -k passwdtest -i client-id-12 -t devices/ID/sensor1 -m \"Temperature: 30 C\"\n\
    - Publish text message to topic, using a configuration file.\n\
      $ ecli_mqtt_pub -i client-id-13 -t devices/ID/sensor1 -m \"Temperature: 30 C\" -c conf/client_mqtt.conf\n\
    - Publish file message to topic.\n\
      $ ecli_mqtt_pub -i client-id-14 -t devices/ID/camera -f -m \"/mnt/v4l/camera/img-001.jpg\"\n\
    - Publish file message to topic, to receive file messages and set path to receive it in a loop.\n\
      $ ecli_mqtt_pub -i client-id-15 -t devices/ID/camera -f -m \"/mnt/v4l/camera/img-last.jpg\" -l\n\
    - Publish text message to topic, set Will Flag, Will Message, Will Topic as connection problem retain message.\n\
      $ ecli_mqtt_pub -i client-id-16 -t devices/ID/sensor1 -m \"Temperature: 30 C\" -W -T server/ID/sensor1/status -M \"sensor1:offline\"\n\
    - Publish text message to topic, set Publish retain Flag to send first connection online retain message.\n\
      $ ecli_mqtt_pub -i client-id-17 -t devices/ID/sensor1 -m \"Temperature: 30 C\" -O\n\
    - Delete any retained message in topic.\n\
      $ ecli_mqtt_pub -i client-id-18 -t devices/ID/sensor1 -r -m ""\n\
    - Publish text message to topic in broker with default values. with 30 seconds time to wait for broker connection\n\
      $ ecli_mqtt_pub -i client-id-19 -t devices/ID/sensor1 -m \"Temperature: 30 C\" -P 30\n\
    - Publish text message to topic in broker with default values. with infinite time to wait for broker connection\n\
      $ ecli_mqtt_pub -i client-id-20 -t devices/ID/sensor1 -m \"Temperature: 30 C\" -P -1\n\
\n\n\
 ", BROKER_IP_DEFAULT, BROKER_PORT_DEFAULT, USERNAME_DEFAULT, \
 PASSWORD_DEFAULT, CLIENTID_DEFAULT, TOPIC_DEFAULT, TXT_MSG_DEFAULT,\
 QOS_DEFAULT, ALIVE_CON_DEFAULT, WILL_QOS_DEFAULT, WILL_TOPIC_DEFAULT, WILL_MSG_DEFAULT,\
 PERSIST_CON_DEFAULT, BROKER_IP_DEFAULT, BROKER_PORT_DEFAULT, USERNAME_DEFAULT,\
 PASSWORD_DEFAULT, CLIENTID_DEFAULT, TOPIC_DEFAULT, OUT_FILE_DEFAULT,\
 ALIVE_CON_DEFAULT, WILL_QOS_DEFAULT, WILL_TOPIC_DEFAULT, WILL_MSG_DEFAULT, PERSIST_CON_DEFAULT
#endif
