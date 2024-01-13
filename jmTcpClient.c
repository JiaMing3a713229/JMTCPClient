// tcp_server.c
#include "jmTcpClient.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include <string.h>

static void tcpTask(void *pvParameters);

SERVER_INFO* initServer(const char* host_address, int host_port) {

    SERVER_INFO *server = (SERVER_INFO*)malloc(sizeof(SERVER_INFO));

    if (server == NULL) {
        // 处理内存分配失败的情况
        return NULL;
    }
    server -> port = host_port;
    server -> ip_address = strdup(host_address);
    server -> sock = -1;
    printf("Connect server IP:%s, Port: %d, \r\n", server -> ip_address, server -> port);
    return server;
}

void clean(SERVER_INFO *server) {
    if (server != NULL) {
        // 释放动态分配的内存
        free((void*)server->ip_address);
        free(server);
    }
}

void start(SERVER_INFO *server) {
    // 启动 TCP 服务器任务
    xTaskCreate(tcpTask, "tcpTask", 1024 * 4, (void *)server, 1, NULL);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
/* pvParamaters is point to struct server */
static void tcpTask(void *pvParameters){
    
    SERVER_INFO *server = (SERVER_INFO*)pvParameters;

    struct sockaddr_in dest_addr;

    while(1){
        
        if(server != NULL){

            server -> sock = socket(AF_INET, SOCK_STREAM, 0);
            printf("%d\r\n", server->sock);
            // server -> socket = socket(AF_INET, SOCK_STREAM, 0);
            
            if(server->sock < 0){
                // LOGln("Socket unable to connect");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                continue;
            }

            dest_addr.sin_addr.s_addr = inet_addr((server -> ip_address));
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_port = htons((server -> port));

            //connect to server
            if(connect(server->sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) != 0){
                // LOGln("Socket unable to connect");
                printf("failed connnect\n");
                close(server -> sock);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                continue;
            }
            printf("connect server\n");

            // Wait for 3 minutes
            vTaskDelay(180000 / portTICK_PERIOD_MS);

            close(server->sock);


        }
    
    }
}

int writeTCP(SERVER_INFO *server, const char* msg) {
    int sock = server -> sock;
    if ((strcmp(msg, "") != 0) && sock > 0) {

        // Send the data immediately
        if (write(sock, msg, strlen(msg)) < 0) {
            return -1;
        } else {
            return 0;
        }
    }
}

JMTCPClient TCPClient = {
    .initServer = initServer,
    .start = start,
    .writeTCP = writeTCP
};

