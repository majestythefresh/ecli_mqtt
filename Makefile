#***********************************************************************
# FILENAME    :   Makefile
# AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
# DATE        :   November 2018
# DESCRIPTION :   Make a Build for MQTT protocol client.
# LICENSE     :   GPL v2.0
#
#***********************************************************************/

include config.mk

#***********************************************************************/
#*************************** Compile targets ***************************/
#***********************************************************************/

mqttclient: $(BIN)/ecli_mqtt_pub $(BIN)/ecli_mqtt_sub set_properties

all: mqttclient

clean: clientclean

cleanall:
	rm -f lib/lib* bin/ecli_mqtt_* output/*.o
	rm -f lib/nios2-linux/* output/nios2-linux/* bin/nios2-linux/*
	rm -f lib/nios2-uclinux/* output/nios2-uclinux/* bin/nios2-uclinux/*
	rm -f lib/arm/* output/arm/* bin/arm/*
	rm -f lib/x86/* output/x86/* bin/x86/*

cleanclientobjects:
	rm -f $(LIB)/lib*
	rm -f $(OUTPUT)/*.o

cleanclientbin:
	rm -f $(BIN)/ecli_mqtt_*

clientclean: cleanclientbin cleanclientobjects

packet:
	tar -cvzf ../ecli_mqtt.tar.gz ../ecli_mqtt
