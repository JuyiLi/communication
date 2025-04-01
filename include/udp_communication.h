#ifndef UDP_COMMUNICATION_H
#define UDP_COMMUNICATION_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include "communication.h"

class udp_communication: public communication
{
public:
    //构造和析构函数
    udp_communication();
    ~udp_communication();

    /**
     * @brief client_connect 客户端连接服务器
     * @param IP 服务器ip
     * @param port 服务器接口号
     * @return socket_id
     */
    int client_connect(const char * IP, const int port);

    /**
     * @brief 断开与server的通讯
     * @param socket socketid
     * @return 成功返回1,失败返回0
     */
    int client_disconnect(const int socket);

    /**
     * @brief client_send_data 客户端发送数据
     * @param socket socketid
     * @param cMsg 发送的数据
     * @param sizeof_cMsg 发送数据的长度
     * @return 成功返回1,失败返回0
     */
    int client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg);

    /**
     * @brief client_receive_data 服务器接受数据
     * @param socket socketid
     * @param sMsg 接收到的数据会存储在这里
     * @param sizeof_sMsg 接受数据的长度
     * @return 成功返回1,失败返回0
     */
    int client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg);

    /**
     * @brief client_send_data_with_feedback 发送数据后，会检查返回的数据是否是规定返回的数据
     * @param socket socketid
     * @param cMsg 发送的数据
     * @param sizeof_cMsg 发送数据的长度
     * @param sMsg 规定接受的数据
     * @param sizeof_sMsg 接受的数据长度
     * @return 数据发送成功且接受到规定数据，返回1,反之返回0
     */
    int client_send_data_with_feedback(const int socket, const unsigned char * cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg);

    /**
     * @brief client_clear_old_data
     * @param socket
     * @return
     */
    int client_clear_old_data(const int socket);

protected:

    bool device_is_open(const int socket);

    struct sockaddr_in ser_addr;
    struct sockaddr_in src;


private:
    //private members here;
};

#endif
