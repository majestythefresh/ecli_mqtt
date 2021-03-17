#***********************************************************************
# FILENAME    :   Makefile
# AUTHOR      :   Arturo Plauchu (arturo.plauchu@gmail.com)
# DATE        :   November 2018
# DESCRIPTION :   Make a Build for MQTT protocol client.
# LICENSE     :   GPL v2.0
#
#***********************************************************************/
SRC=src
INC=include
LIB=lib
BIN=bin
CLIENT=client
CLIENT_SRC=$(SRC)/$(CLIENT)
CLIENT_LIB_SRC=$(SRC)/$(CLIENT)/$(LIB)
OUTPUT=output

CC=gcc
CCFLAGS=-I$(INC) -Wall -O
LDFLAGS=-L$(LIB) -leclimqtt -leclimqttclient -leclimqttlog
AR=ar

#********************** LOG **********************

ifeq (${LOG},DEBUG)
	DEFINE= -D LOG_DEBUG
endif
ifeq (${LOG},TRACE)
	DEFINE= -D LOG_TRACE
endif
ifeq (${LOG},BULK)
	DEFINE= -D LOG_BULK
endif
ifeq (${LOG},DEBUG_TRACE)
	DEFINE= -D LOG_DEBUG -D LOG_TRACE
endif
ifeq (${LOG},DEBUG_BULK)
	DEFINE= -D LOG_DEBUG -D LOG_BULK
endif
ifeq (${LOG},TRACE_BULK)
	DEFINE= -D LOG_TRACE -D LOG_BULK
endif
ifeq (${LOG},ALL)
	DEFINE= -D LOG_DEBUG -D LOG_DEBUG -D LOG_BULK
endif

#********************** X86 ARCH **********************
ifeq (${ARCH},x86)
	BIN=bin/x86
	LIB=lib/x86
	OUTPUT=output/x86
	CLIENT_LIB_SRC=$(SRC)/$(CLIENT)/lib
	AR=ar
endif

#********************** ARM ARCH **********************
ifeq (${ARCH},arm)
	BIN=bin/arm
	LIB=lib/arm
	OUTPUT=output/arm
	CLIENT_LIB_SRC=$(SRC)/$(CLIENT)/lib
	AR=ar
endif

#********************** NIOS2 ARCH **********************
#********************** UCLINUX **********************
ifeq (${ARCH},nios2-uclinux)
	BIN=bin/nios2-uclinux
	LIB=lib/nios2-uclinux
	OUTPUT=output/nios2-uclinux
	CLIENT_LIB_SRC=$(SRC)/$(CLIENT)/lib
	CC=nios2-linux-uclibc-gcc
	ELFFLAG=-elf2flt
	CCFLAGS=-I$(INC) -Wall -O $(ELFFLAG)
	AR=nios2-linux-uclibc-ar
endif
#********************** LINUX **********************
ifeq (${ARCH},nios2-linux)
	BIN=bin/nios2-linux
	LIB=lib/nios2-linux
	OUTPUT=output/nios2-linux
	CLIENT_LIB_SRC=$(SRC)/$(CLIENT)/lib
	CC=nios2-linux-gnu-gcc
	AR=nios2-linux-gnu-ar
endif

#***********************************************************************/
#*************************** Compile objects ***************************/
#***********************************************************************/

$(BIN)/ecli_mqtt_pub: $(LIB)/libeclimqtt.a $(LIB)/libeclimqttclient.a $(LIB)/libeclimqttlog.a $(OUTPUT)/ecli_mqtt_pub.o
	$(CC) $(DEFINE) $(OUTPUT)/ecli_mqtt_pub.o -o $(BIN)/ecli_mqtt_pub $(LDFLAGS) $(ELFFLAG)

$(OUTPUT)/ecli_mqtt_pub.o: $(CLIENT_SRC)/ecli_mqtt_pub.c $(INC)/libeclimqtt.h $(INC)/libeclimqttclient.h $(INC)/libeclimqttlog.h
	$(CC) $(DEFINE) $(CCFLAGS) -c $(CLIENT_SRC)/ecli_mqtt_pub.c -o $(OUTPUT)/ecli_mqtt_pub.o

$(BIN)/ecli_mqtt_sub: $(LIB)/libeclimqtt.a $(LIB)/libeclimqttclient.a $(LIB)/libeclimqttlog.a $(OUTPUT)/ecli_mqtt_sub.o
	$(CC) $(DEFINE) $(OUTPUT)/ecli_mqtt_sub.o -o $(BIN)/ecli_mqtt_sub $(LDFLAGS) $(ELFFLAG)

$(OUTPUT)/ecli_mqtt_sub.o: $(CLIENT_SRC)/ecli_mqtt_sub.c $(INC)/libeclimqtt.h $(INC)/libeclimqttclient.h $(INC)/libeclimqttlog.h
	$(CC) $(DEFINE) $(CCFLAGS) -c $(CLIENT_SRC)/ecli_mqtt_sub.c -o $(OUTPUT)/ecli_mqtt_sub.o

#***************************     Libraries    ***************************/

$(LIB)/libeclimqtt.a: $(OUTPUT)/libeclimqtt.o
	$(AR) rcs $(LIB)/libeclimqtt.a $(OUTPUT)/libeclimqtt.o

$(OUTPUT)/libeclimqtt.o: $(CLIENT_LIB_SRC)/libeclimqtt.c $(INC)/libeclimqtt.h
	$(CC) $(DEFINE) $(CCFLAGS) -c $(CLIENT_LIB_SRC)/libeclimqtt.c -o $(OUTPUT)/libeclimqtt.o

$(LIB)/libeclimqttclient.a: $(OUTPUT)/libeclimqttclient.o
	$(AR) rcs $(LIB)/libeclimqttclient.a $(OUTPUT)/libeclimqttclient.o

$(OUTPUT)/libeclimqttclient.o: $(CLIENT_LIB_SRC)/libeclimqttclient.c $(INC)/libeclimqttclient.h
	$(CC) $(DEFINE) $(CCFLAGS) -c $(CLIENT_LIB_SRC)/libeclimqttclient.c -o $(OUTPUT)/libeclimqttclient.o

$(LIB)/libeclimqttlog.a: $(OUTPUT)/libeclimqttlog.o
	$(AR) rcs $(LIB)/libeclimqttlog.a $(OUTPUT)/libeclimqttlog.o

$(OUTPUT)/libeclimqttlog.o: $(CLIENT_LIB_SRC)/libeclimqttlog.c $(INC)/libeclimqttlog.h
	$(CC) $(DEFINE) $(CCFLAGS) -c $(CLIENT_LIB_SRC)/libeclimqttlog.c -o $(OUTPUT)/libeclimqttlog.o


set_properties:
ifeq (${ARCH},nios2-uclinux)
	nios2-linux-uclibc-flthdr -s 102400  $(BIN)/ecli_mqtt_pub
	nios2-linux-uclibc-flthdr -s 102400  $(BIN)/ecli_mqtt_sub
endif
