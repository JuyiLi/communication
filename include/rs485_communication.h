#ifndef RS485_COMMUNICATION_H
#define RS485_COMMUNICATION_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include "communication.h"

class rs485_communication: public communication
{
public:	

    rs485_communication();

    ~rs485_communication();

    /**
     * @brief 设备连接
     * @param _device 如"/dev/ttyUSB0"
     * @param _baud_rate 波特率，19200对应B19200
     * @return socket_id
     */
//    int client_connect(const char * _device, const int _baud_rate, int flow_ctrl = 0, int databits = 8, int stopbits = 1, int parity = 'n');
    int client_connect(const char * _device, const int _baud_rate);

    /**
     * @brief 断开通讯
     * @param socket socketid
     * @return 成功返回1,失败返回0
     */
    int client_disconnect(const int socket);

    /**
     * @brief 发送数据
     * @param socket socketid
     * @param cMsg 发送的数据
     * @param sizeof_cMsg 发送数据的长度
     * @return 成功返回1,失败返回0
     */
    int client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg);

    /**
     * @brief 接受数据
     * @param socket socketid
     * @param sMsg 接收到的数据会存储在这里
     * @param sizeof_sMsg 接受数据的长度
     * @return 成功返回1,失败返回0
     */
    int client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg);

    /**
     * @brief 发送数据后，会检查返回的数据是否是规定返回的数据
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

private:
	//private members here;
};

#endif
