#ifndef NMSGPACK_H
#define NMSGPACK_H

#include <QObject>

#include <nanomsg/nn.h>
#include <nanomsg/pair.h>
#include <msgpack.h>
#define UNPACKED_BUFFER_SIZE 2048


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>

/* DataPatern
 *  this is the data pattern that is filled and transmitted
 *  note changing this pattern needs to adjust fillDataBuffer as well
 */
typedef struct __attribute__((__packed__)) {
    uint64_t    element1;         //element 1
    bool        element2;         //boolean
    std::string element3;         //string
}DataPatern;

/* TelegramPatern is a message pack internal buffer
 */
typedef struct __attribute__((__packed__)) {
    unsigned char      STX;         //element 1
    unsigned char      NA[2];       //Network Adresse
    unsigned char      Auftrag[4];
    unsigned char      Fieldlen[4]; //data field length
    char *dataptr;
    //std::string        Data;          //string
    unsigned char      CRC[4];
    unsigned char      ETX;
}TelegramPatern;


class nmsgpack : public QObject
{
    Q_OBJECT
public:
    explicit nmsgpack(QObject *parent = nullptr, bool nodemode=false, std::string localurl= "tcp://127.0.0.1:5000");

    msgpack_sbuffer dataBuffer;
    void fillDataBuffer(uint64_t element1, bool element2, std::string element3);
    int sendBuffer(void);
    DataPatern receiveBuffer();
    void destroyBuffer(void);
    int socket;


private:
    int nm_send(int socket, msgpack_sbuffer sbuf);
    void decToHexa(int n, char* hexa);
    int hexaToDecimal(unsigned char hexVal[], int len);
    void print(char const* buf, unsigned int len);
    [[ noreturn ]] void fatal(const char *func);
    void serialize (msgpack_sbuffer buf, char *serializedbuffer, uint __attribute__ ((unused)) len);
    msgpack_sbuffer prepare_buffer(DataPatern dp, size_t &buflen);
    void deserialize(char *buf,msgpack_sbuffer &rbuf,size_t datalen);
    void unpacking(msgpack_sbuffer rbuf, msgpack_object &depacked, DataPatern &dp);
    DataPatern nm_recv(int sock, msgpack_object &depacked);
    int nm_init_socket(const char* url, bool recv_mode);

signals:

};

#endif // NMSGPACK_H
