/***********************************************************************
* FILENAME    :   ecli_mqtt_sub.c
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   MQTT protocol Subscriber client.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <signal.h>

/**********************************************************************/

#include <libeclimqtt.h>

/**********************************************************************/

ecli_broker_t broker;

/**********************************************************************/

void keep_alive(int signal)
{
    printf(PING_MSG);
    eclimqtt_pingreq(&broker);
    alarm(ALIVE_PING_DEFAULT);
}

/**********************************************************************/

void interrupt(int signal)
{
    printf(SIGINT_MSG, signal);
    eclimqtt_disconnect(&broker);
    ecli_close(&broker);
    exit(signal);
}

/**********************************************************************/

int main(int argc, char* argv[]){

    ecli_conf_t conf;
    uint8_t      return_code = 0;

    signal(SIGINT, interrupt);
    signal(SIGALRM, keep_alive);

    /*Get configuration*/
    ecli_get_conf(&broker, &conf, argc, argv);
    /*Associate client connection with Broker*/
    if ( ( return_code = ecli_init(&broker, &conf) ) != CLI_NO_ERROR ) {
        ecli_show_error(return_code);
        return return_code;
    }
    /*Connect with Broker*/
    if ( ( return_code = eclimqtt_connect(&broker, &conf) ) != CLI_NO_ERROR ){
        ecli_show_error(return_code);
        return return_code;
    }

    alarm(ALIVE_PING_DEFAULT);

    /* Subscribe */
    if ( ( return_code = eclimqtt_subscribe( &broker, &conf ) ) != CLI_NO_ERROR ){
        ecli_show_error(return_code);
        return return_code;
    }


    /* Read and get Payload */
    char     topic[CLI_TOPIC_LEN];
    //uint8_t  msg_buffer[CLI_MAX_MSG_SIZE];
    uint32_t buffer_len = MAX_TXT_MSG_SIZE;
    /* buffer size according to Type of Message [ text msg | datafile msg ]*/
    if ( conf.msg_type == CLI_DATAFILE_MSG ) {
        buffer_len = CLI_MAX_MSG_SIZE;
    }
    uint8_t  msg_buffer[buffer_len];
    uint32_t msg_len     = 0;
    do {
        if ( ( return_code = ecli_read_get_msg( &broker, &conf, topic, msg_buffer, &msg_len, 0 ) ) != CLI_NO_ERROR ){
            ecli_show_error(return_code);
            if ( conf.persist_conn_time ) {
                alarm(ALIVE_PING_DEFAULT);
                ecli_close(&broker);
                /*Connect with Broker*/
                if ( ( return_code = ecli_init(&broker, &conf) ) != CLI_NO_ERROR ) {
                    ecli_show_error(return_code);
                }
                else {
                    /*Authorize Broker*/
                    if ( ( return_code = eclimqtt_connect(&broker, &conf) ) != CLI_NO_ERROR ){
                        ecli_show_error(return_code);
                        return return_code;
                    }
                    /* Subscribe */
                    if ( ( return_code = eclimqtt_subscribe( &broker, &conf ) ) != CLI_NO_ERROR ){
                        ecli_show_error(return_code);
                        return return_code;
                    }
                }
            }
            else{
                return return_code;
            }
        }
        if ( msg_len > 0 ) {
            printf(TOPIC_MSG, topic);
            printf(MSG_LEN_MSG, msg_len);
            /*Type of Message [ text msg | datafile msg ]*/
            if ( conf.msg_type == CLI_DATAFILE_MSG ) {
                printf(DATA_MSG);
                FILE* recv_file = fopen( conf.datafile_path, "wb" );
                fwrite(msg_buffer, 1 , msg_len, recv_file);
                fclose(recv_file);
            }
            else if ( conf.msg_type == CLI_TXT_MSG ) {
                msg_buffer[msg_len] = '\0';
                printf(MESSAGE_MSG, msg_buffer);
            }
        }
    }
    while(conf.client_loop_flg);

    /* Close connections */
    /* Send Disconnect Msg to Broker */
    if ( ( return_code = eclimqtt_disconnect(&broker) ) != CLI_NO_ERROR ){
        ecli_show_error(return_code);
        return return_code;
    }
    /* Close socket */
    ecli_close(&broker);

    return CLI_NO_ERROR;
}
