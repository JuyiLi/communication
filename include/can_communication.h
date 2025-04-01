#ifndef CAN_COMMUNICATION_H
#define CAN_COMMUNICATION_H

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctime>
#include <cstdlib>
#include <map>
#include "communication.h"
#include "controlcan.h"

class can_communication: public communication
{
public:

    /**
     * @brief 基于controlcan的can通讯库，测试硬件为创芯canalyst-II linux版
     */
    can_communication();
    ~can_communication();

    /**
     * @brief canalyst连接
     * @param IP 服务器ip
     * @param port 波特率
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
    int client_send_data(const int socket, const int id, const unsigned char * cMsg, int sizeof_cMsg);

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

    VCI_BOARD_INFO pInfo1[50];
    int num=VCI_FindUsbDevice2(pInfo1);

    std::map<int, int> baudrate_map=
    {
        {10     , 0x311c},
        {20     , 0x181c},
        {40     , 0x87ff},
        {50     , 0x091c},
        {80     , 0x83ff},
        {100    , 0x041c},
        {125    , 0x031c},
        {200    , 0x81fa},
        {250    , 0x011c},
        {400    , 0x80fa},
        {500    , 0x001c},
        {666    , 0x80b6},
        {800    , 0x0016},
        {1000   , 0x0014}
//        {33.33  , 0x096f}
//        {66.66  , 0x046f}
//        {83.33  , 0x036f}
    };

    int send_sleep_time = 10000;
private:
	//private members here;
};

#endif
