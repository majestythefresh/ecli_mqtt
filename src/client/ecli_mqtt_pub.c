/***********************************************************************
* FILENAME    :   ecli_mqtt_pub.c
* AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
* DATE        :   November 2018
* DESCRIPTION :   MQTT protocol Publisher client.
* LICENSE     :   GPL v2.0
*
***********************************************************************/

#include <libeclimqtt.h>

/**********************************************************************/

int main(int argc, char* argv[]){

    ecli_conf_t conf;
    ecli_broker_t broker;
    uint8_t return_code;

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

    /* Publish first online retain message */
    if( conf.publish_online_flg ) {
        if ( ( return_code = eclimqtt_publish( &broker, &conf, 1 ) ) != CLI_NO_ERROR ){
            ecli_show_error(return_code);
            return return_code;
        }
        sleep(1);
    }

    do {
        /* Publish normal message*/
        if ( ( return_code = eclimqtt_publish( &broker, &conf, 0 ) ) != CLI_NO_ERROR ){
            ecli_show_error(return_code);
            return return_code;
        }
    }
    while( conf.client_loop_flg );

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
