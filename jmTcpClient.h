#ifndef JMTCPCLIENT_H
#define JMTCPCLIENT_H

#ifdef LOG_ENABLE
#define LOGln(msg) Serial.println(msg)
#define LOG(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
#define LOG(msg)
#endif

#define BUFFER_LENGTH_MAX 1028

typedef void (*MessageCallback)(const char* msg);

typedef struct{

    int port;
    const char* ip_address;
    int sock;

}SERVER_INFO;

typedef struct{

    /* function pointer */
    SERVER_INFO* (*initServer)(const char* host_address, int host_port);
    void (*start)(SERVER_INFO *server);      //開始與TCP SERVER 對接
    void (*clean)(SERVER_INFO *server);    //釋放JMClient的記憶體配置
    int (*writeTCP)(SERVER_INFO *server, const char* msg);      //將const char* msg的資料傳給TCPTask

    /* Callback function for message reception */
    MessageCallback messageCallback;

}JMTCPClient;

extern JMTCPClient TCPClient;

SERVER_INFO* initServer(const char* host_address, int host_port);
void start(SERVER_INFO *server);
void clean(SERVER_INFO *server);
int writeTCP(SERVER_INFO *server, const char* msg);
#endif // JMTCPSERVER_H