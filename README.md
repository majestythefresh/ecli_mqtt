# MQTT Client library for embedded systems.

### Developer:
    Arturo Plauchu: arturo.plauchu@gmail.com

#### License: GPL V2.0
     - Date of development - November 2018

## How to Compile:

    ### Use following make targets to compile...
      - all : compile MQTT Client (MQTT library, Pub and Sub).
      - ARCH=x86: compile for x86 arch

    ### Use following make targets to clean compiled objects and binaries...
      - clean : clean MQTT Client generated files
      - ARCH=[ x86 | nios2-linux | nios2-uclinux | arm ] clientclean : clean MQTT Client generated files for specific supported arch.

    ### Output:
      - bin folder: broker and pub/sub client binaries to use for host arch.
      - output folder: pub and sub client objects generated for host arch.
      - lib folder: client library objects for host arch

## Features:

    ### Client:
      - Support MQTT 3.1 and 3.1.1 version
      - Support text messages and file messages
      - Support transfer of messages/files from 0 to 256 MB
      - Support of publish message with QOS 0, 1 and 2
      - Support of Will Flag (Last Will Message, Will Topic, Will Retain) in Connection.
      - Support of publish retain flag to erase Will Message with an empty payload.
      - Support of N seconds (or unlimited) persistence to connect with broker

## How to use it:

    ### Tranfer size:
        To allow a maximum size for text messages and file messages you can modify the following values at include/libeclimqttconf.h
        #define ECLI_MAX_MSG_SIZE          4192988   /* 4MB for File messages */
        #define ECLI_MAX_TXT_MSG_SIZE      1024      /* 1KB for Text messages */

    ### Client:
      - ecli_mqtt_pub -h to display options and flags to set and default values. Publisher.
      - ecli_mqtt_sub -h to display options and flags to set and default values. Subscriber.

    ### Examples:

        #### Subscriber:
            - Subscribe to topic in broker with default values, to receive text messages.
              $ ecli_mqtt_sub -i client-id-1 -t devices/ID/sensor1
            - Subscribe to topic in a specific broker, to receive text messages.
              $ ecli_mqtt_sub -b 192.168.125.11 -p 1883 -i client-id-2 -t devices/ID/sensor1
            - Subscribe to topic in a specific broker with credentials, to receive text messages.
              $ ecli_mqtt_sub -b 192.168.125.11 -p 1883 -u usertest -k passwdtest -i client-id-3 -t devices/ID/sensor1
            - Subscribe to topic, to receive text messages and set flag to receive in a loop.
              $ ecli_mqtt_sub -i client-id-4 -t devices/ID/sensor1 -l
            - Subscribe to topic, to receive text messages using a configuration file.
              $ ecli_mqtt_sub -i client-id-5 -t devices/ID/sensor1 -c conf/client_mqtt.conf
            - Subscribe to topic, to receive file messages.
              $ ecli_mqtt_sub -i client-id-6 -t devices/ID/camera -f
            - Subscribe to topic, to receive file messages and set path to receive it.
              $ ecli_mqtt_sub -i client-id-7 -t devices/ID/camera -f -o -tmp/recv_image.jpg
            - Subscribe to topic, to receive file messages and set path to receive it in a loop.
              $ ecli_mqtt_sub -i client-id-8 -t devices/ID/camera -f -o -tmp/recv_image.jpg -l

        #### Publisher:
            - Publish text message to topic in broker with default values.
              $ ecli_mqtt_pub -i client-id-9 -t devices/ID/sensor1 -m "Temperature: 30 C"
            - Publish text message to topic in broker with default values and set flag to receive in a loop.
              $ ecli_mqtt_pub -i client-id-10 -t devices/ID/sensor1 -m "Temperature: current C" -l
            - Publish text message to topic in a specific broker, to receive text messages.
              $ ecli_mqtt_pub -b 192.168.125.11 -p 1883 -i client-id-11 -t devices/ID/sensor1 -m "Temperature: 30 C"
            - Publish text message to topic in a specific broker with credentials, to receive text messages.
              $ ecli_mqtt_pub -b 192.168.125.11 -p 1883 -u usertest -k passwdtest -i client-id-12 -t devices/ID/sensor1 -m "Temperature: 30 C"
            - Publish text message to topic, using a configuration file.
              $ ecli_mqtt_pub -i client-id-13 -t devices/ID/sensor1 -m "Temperature: 30 C" -c conf/client_mqtt.conf
            - Publish file message to topic.
              $ ecli_mqtt_pub -i client-id-14 -t devices/ID/camera -f -m "/mnt/v4l/camera/img-001.jpg"
            - Publish file message to topic, to receive file messages and set path to receive it in a loop.
              $ ecli_mqtt_pub -i client-id-15 -t devices/ID/camera -f -m "/mnt/v4l/camera/img-last.jpg" -l
            - Publish text message to topic, set Will Flag, Will Message, Will Topic as connection problem retain message.
              $ ecli_mqtt_pub -i client-id-16 -t devices/ID/sensor1 -m "Temperature: 30 C" -W -T server/ID/sensor1/status -M "sensor1:offline"
            - Publish text message to topic, set Publish retain Flag to send first connection online retain message.
              $ ecli_mqtt_pub -i client-id-17 -t devices/ID/sensor1 -m "Temperature: 30 C" -O
            - Delete any retained message in topic.
              $ ecli_mqtt_pub -i client-id-18 -t devices/ID/sensor1 -r -m ""
            - Publish text message to topic in broker with default values. with 30 seconds time to wait for broker connection
              $ ecli_mqtt_pub -i client-id-19 -t devices/ID/sensor1 -m "Temperature: 30 C" -P 30
            - Publish text message to topic in broker with default values. with infinite time to wait for broker connection
              $ ecli_mqtt_pub -i client-id-20 -t devices/ID/sensor1 -m "Temperature: 30 C" -P -1


### Notes...

#### For nios2-uclinux ARCH:
     - Change stack limit with toolchain tools
     - Example stack to 4MB...
       [plauchu@localhost ecli_mqtt]$ nios2-linux-uclibc-flthdr -s 4192988 bin/nios2-uclinux/ecli_mqtt_pub
       [plauchu@localhost ecli_mqtt]$ nios2-linux-uclibc-flthdr -p bin/nios2-uclinux/ecli_mqtt_pub
        bin/nios2-uclinux/ecli_mqtt_pub
        Magic:        bFLT
        Rev:          4
        Build Date:   Sat November 17 09:23:10 2018
        Entry:        0x44
        Data Start:   0xaa80
        Data End:     0xe170
        BSS End:      0x10230
        Stack Size:   0x3ffadc
        Reloc Start:  0xe170
        Reloc Count:  0x419
        Flags:        0x1 ( Load-to-Ram )
