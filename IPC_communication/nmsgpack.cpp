#include "nmsgpack.h"

/* class creation
*   designed for sender-receiver mode, communication between 2 apps on same pc
*   for other polling modes read nanomsg documentation
*   std::string localurl (default): tcp://127.0.0.1:5000
*   bool nodemode: Sender mode (default) => flase, Receiver mode => true
*
*/
nmsgpack::nmsgpack(QObject *parent, bool nodemode, std::string localurl) : QObject(parent)
{
    const char* url=localurl.c_str();
    socket=nm_init_socket(url,nodemode);   // initialize the socket

}

/* fillDataBuffer
 *  this is to fill the databuffer and to compress (packe) it into a local msgpack buffer
 *  that is used when sending data out. This msgpack buffer can be send with nm_send(sock,sbuf)
 */
void nmsgpack::fillDataBuffer(uint64_t element1, bool element2, std::string element3)
{
    DataPatern dp;
    dp.element1=element1;
    dp.element2=element2;
    dp.element3=element3;

    size_t buflen=0;
    dataBuffer = prepare_buffer(dp, buflen);
}

/*  sendBuffer
 *   this is the method that sends the buffer out
 */
int nmsgpack::sendBuffer(void)
{
    int r=nm_send(socket, dataBuffer);
    return r;
}

/* reciveBuffer
 *  this is the buffer receiving method
 *  it prints the data within unpacking
 */
DataPatern nmsgpack::receiveBuffer(void)
{
    msgpack_object depacked;
    DataPatern dp; //final data
    dp = nm_recv(socket, depacked);

    if (depacked.type == MSGPACK_OBJECT_ARRAY){
        printf("number of elements = %d \n", depacked.via.array.size); //3
        printf("element 1 = %llu \n", dp.element1);
        printf("element 2 = %d \n", dp.element2); //element 2 correct -> false
        printf("element 3 = %s \n", dp.element3.c_str()); //element 3 correct -> example
    }


    return dp;
}

/*  destroyBuffer
 *   this function must be called at the end
 */
void nmsgpack::destroyBuffer(void)
{
    msgpack_sbuffer_destroy(&dataBuffer);
}




/* Local Functions ********************************/
/* -----------------------------------------------*/


// function to convert decimal to hexadecimal
void nmsgpack::decToHexa(int n, char *hexa)
{
    // char array to store hexadecimal number
    //char hexaDeciNum[100];
    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexa[i] =  temp + 48;
            i++;
        }
        else
        {
            hexa[i] = temp + 55;
            i++;
        }

        n = n/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        std::cout << hexa[j];
}


// Function to convert hexadecimal to decimal
int nmsgpack::hexaToDecimal(unsigned char hexVal[], int len)
{
    // Initializing base value to 1, i.e 16^0
    int base = 1;

    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            dec_val += (hexVal[i] - 48)*base;

            // incrementing base by power
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (hexVal[i]>='A' && hexVal[i]<='F')
        {
            dec_val += (hexVal[i] - 55)*base;

            // incrementing base by power
            base = base*16;
        }
    }

    return dec_val;
}


/* print: This function prints hex values starting at point buf,
 * with length of len. It is used intensively in testing.
 */
void nmsgpack::print(const char *buf, unsigned int len)
{
    size_t i = 0;
    for(; i < len ; ++i)
        printf("%02x ", 0xff & buf[i]);
    printf("\n");
}


/* prints nanomsg fatal error
 */
void nmsgpack::fatal(const char *func)
{
    fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
    exit(1);
}


/* this function takes a msgpack buffer and serializes it
 * input: msgpack buffer, length of the array
 * output: serializedbuffer: pointer to first element in the membuf array
 *         after being serialized
 * note: length of the array may be used to find pointer starting for additional elements in future
 *       supressed the warning for now, but len may be useful in future
 */
void nmsgpack::serialize(msgpack_sbuffer buf, char *serializedbuffer, uint len)
{
    char *data;
    data=buf.data;
    size_t alloc=buf.alloc;
    size_t size=buf.size;
    memcpy (serializedbuffer,&alloc,sizeof(size_t));                         //take first element which is integer (see DataPatern struct)
    memcpy (&serializedbuffer[0]+sizeof(size_t),data,size);                  //take 2nd element which is boolean by its pointer
    memcpy (&serializedbuffer[0]+sizeof(size_t)+size,&size,sizeof(size_t));  //take 3rd element which is string by its starting pointer
    Q_UNUSED(len);
}


/* prepare_buffer prepares the msgpack buffer from given datapatern structure for send purposes
 * input: DataPatern structure (see above)
 * output: msgpack_packer buffer
 *         buflen: bufferlength
 */
msgpack_sbuffer nmsgpack::prepare_buffer(DataPatern dp, size_t &buflen)
{
    //prepare the packaged msgpack structure
     msgpack_sbuffer sbuf;
     msgpack_packer pk;
     /* msgpack::sbuffer is a simple buffer implementation. */
     msgpack_sbuffer_init(&sbuf);
//---------------------------------- Used in Example --------------
     /* serialize values into the buffer using msgpack_sbuffer_write callback function. */
     msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
     msgpack_pack_array(&pk, 3); //adjust number of elements in a package
     msgpack_pack_int(&pk, dp.element1);   //first element is int with value 1
     if(dp.element2)
     {
         msgpack_pack_true(&pk);     //second element is boolean with value true
     }
     else {
         msgpack_pack_false(&pk);     //second element is boolean with value true
     }
     //prepare the string count
     size_t c=dp.element3.length(); // 7
     const char* element3pt=dp.element3.c_str();
     msgpack_pack_str(&pk, c);   //third element is a string with 7 characters
     msgpack_pack_str_body(&pk, element3pt, c); //fill the element string
//-----------------------------------------------------------------

     /* serialize values into the buffer using msgpack_sbuffer_write callback function. */
     TelegramPatern dp1;
     msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
     msgpack_pack_array(&pk, 7);     //adjust number of elements in a package
     msgpack_pack_char(&pk,0x02);    //STX: 1 Byte
     msgpack_pack_str(&pk,2);        //NA : 2 Byte
     msgpack_pack_str_body(&pk,&dp1.NA[0],2);

     msgpack_pack_str(&pk,4);        //Auftrag: 4 Byte
     msgpack_pack_str_body(&pk,&dp1.Auftrag[0],4);

     msgpack_pack_str(&pk,4);        //data length
     msgpack_pack_str_body(&pk,&dp1.Fieldlen[0],4);

     //convert hex to decimal to know the length of the adress
     int dataln=hexaToDecimal(dp1.Fieldlen,4);
     char * dd = (char *) malloc(dataln);

     // prtint this message to be sure
     print(sbuf.data, static_cast<uint>(sbuf.size));

     buflen=sbuf.size+2*(sizeof(size_t));//2* because of alloc and size
     return sbuf;

}


/* deserialize: this funciton deserializes (converts data from nanomsg to msgpack format) data
 * input: *buf: received buffer from nano msg
 *        datalen: this is length of data in the package received from nanomsg
 * output: rbuf: byref filled msgpack buffer
 */
void nmsgpack::deserialize(char *buf, msgpack_sbuffer &rbuf, size_t datalen)
{
    //deserialize the data back from nanomsg to the msgpack structure
    //first paramater that filled was alloc, then data, then size
    size_t tmp;
    memcpy(&tmp,buf,sizeof(size_t)); //copy the size first
    rbuf.size=tmp;
    //char data[rbuf.size]; //to supress variable size of array
    char *data = static_cast<char *>(malloc(rbuf.size));
    memcpy(&data[0],buf+sizeof(rbuf.size),static_cast<size_t>(datalen)); //copy the data second
    //printf("filled data \n");
    //print(&data[0],rbuf.size);
    rbuf.data=&data[0];
    memcpy(&tmp,buf+sizeof(size_t)+rbuf.size,sizeof(size_t)); //copy the alloc last
    rbuf.alloc=tmp;
    //end deserialization
}


/* unpacking: this function unpacks data given in a msgpack structure into
 * msgpack_object that holds the elements. Aditionally it stores them
 * into a local structure representing transmission data pattern */
void nmsgpack::unpacking(msgpack_sbuffer rbuf, msgpack_object &depacked, DataPatern &dp)
{

    /* deserialize the buffer into msgpack_object instance. */
    msgpack_zone mempool;

    /* deserialized object is valid during the msgpack_zone instance alive. */
    msgpack_zone_init(&mempool, 2048);
    msgpack_unpack(rbuf.data, rbuf.size, nullptr, &mempool, &depacked);
    /* print the deserialized object. */
    msgpack_object_print(stdout, depacked);
    puts(""); //we have object array in the struct
    if (depacked.type == MSGPACK_OBJECT_ARRAY){
//        printf("number of elements = %d \n", depacked.via.array.size); //3

        uint64_t element1= depacked.via.array.ptr[0].via.u64;
//        printf("element 1 = %llu \n", element1);
        dp.element1=element1;

        bool element2 = depacked.via.array.ptr[1].via.boolean;
//        printf("element 2 = %d \n", element2); //element 2 correct -> false
        dp.element2 = element2;

        msgpack_object_str element3 = depacked.via.array.ptr[2].via.str;
//        print(element3.ptr,element3.size); //element 3 correct -> example
        std::string e3(element3.size,'0');
        memcpy(&e3[0], element3.ptr, element3.size);
        dp.element3 = e3;

    }

    msgpack_zone_destroy(&mempool);
}


/* nm_send: This function takes a msgpack_packer bufer that holds data should be send
 * but in msgpack format. It then serializes the data into a serial buffer
 * and sends it out through nanomsg
 * input: socket: integer representing socket number
 *        msgpack_sbuffer sbuf: buffer holding exchange data pattern
 * output: int r: number of packets being sent
 */
int nmsgpack::nm_send(int socket, msgpack_sbuffer sbuf)
{

    //serialize data and deliver to nanomsg
    //use serialize function
    unsigned int len=uint(sbuf.size+2*(sizeof(size_t)));//2* because of alloc and size

    //char membuf[len]; //supress variable length array
    char *membuf = static_cast<char *>(malloc(len));
    serialize(sbuf,&membuf[0],len);
    print(membuf,len);
    int r=nn_send(socket, &membuf[0], len, 0);
    return r;
}


/* nm_recv: This function is the package interface, it uses the above code
 * to receive data from nanomsg and convert it into msgpack_object;
 * input: socket value
 * output: msgpack_object: by reference incase there is a need to access object data
 *         datapattern : original sent data structure
 */
DataPatern nmsgpack::nm_recv(int sock, msgpack_object &depacked)
{
    //initialize msgpack reception object
    DataPatern dp;

    //receive data from nanomsg
    char *buf = nullptr;
    int msglen = nn_recv(sock, &buf, NN_MSG, 0); //nanomessage length
    //print(buf,msglen);

    //deserialize the data back from nanomsg to the msgpack structure
    msgpack_sbuffer rbuf;
    if (msglen>0)
    {
        deserialize(buf,rbuf, static_cast<size_t>(msglen));
        unpacking(rbuf,depacked,dp);
    }

    return dp;
}


/* nm_init_socket: This function initializes the nanomsg socket
 * and returns the socket number.
 * input: url: nanomsg binding url ex tcp://127.0.0.1:5000
 * mode: 0 or false= Node0: Main node, it binds to the address
 *       1 or true = Node1: Auxiliary node, it connects to the address
 * ourput: socket value
 *
 */
int nmsgpack::nm_init_socket(const char *url, bool recv_mode)
{
    int sock;
    if ((sock = nn_socket(AF_SP, NN_PAIR)) < 0) {
            fatal("nn_socket");
    }
    if (recv_mode) //Receiver (Node 1, Auxiliary node)
    {
        if (nn_connect(sock, url) < 0) {
                fatal("nn_connect");
        }
    }
    else //Sender (node 0, server, main node)
    {
        if (nn_bind(sock, url) < 0) {
               fatal("nn_bind");
       }

    }

    int to = 100;
    if (nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to,sizeof (to)) < 0) {
            fatal("nn_setsockopt");
    }

    return sock;
}







