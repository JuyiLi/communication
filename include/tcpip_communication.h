#ifndef TCPIP_COMMUNICATION_H
#define TCPIP_COMMUNICATION_H

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

class tcpip_communication: public communication
{
public:	
	//构造和析构函数	
	tcpip_communication();
	~tcpip_communication(); 

    bool is_print = false;

    /**
     * @brief server test 程序
     * @return
     */
	int server();
	
	int client();

    /**
     * @brief 打开server
     * @param IP 服务器ip
     * @param port 服务器接口号
     * @param socket_id socket_id
     * @param listen_id listen_id
     * @return 成功返回1,失败返回0
     */
    int server_on(const char * IP, const int port, int & socket_id, int & listen_id);

    /**
     * @brief 关闭server
     * @param socket_id socket_id
     * @param listen_id listen_id
     * @return
     */
    int server_off(const int socket_id, const int listen_id);

    /**
     * @brief 服务器获取数据
     * @param socket socket_id
     * @param sMsg 接受的数据
     * @param sizeof_sMsg 接受数据的长度
     * @return 成功返回1,失败返回0
     */
    int server_receive_data(const int socket, char * sMsg, int sizeof_sMsg);

    /**
     * @brief 服务器发送数据
     * @param socket socket_id
     * @param cMsg 发送的数据
     * @param sizeof_cMsg 发送数据的长度
     * @return 成功返回1,失败返回0
     */
    int server_send_data(const int socket, const char * cMsg, int sizeof_cMsg);
	
    /**
     * @brief client_connect 客户端连接服务器
     * @param IP 服务器ip
     * @param port 服务器接口号
     * @return socket_id
     */
	int client_connect(const char * IP, const int port);
	
    /**
     * @brief client_send_data 客户端发送数据
     * @param socket socketid
     * @param cMsg 发送的数据
     * @param sizeof_cMsg 发送数据的长度
     * @return 成功返回1,失败返回0
     */
	int client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg);

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

    /**
     * @brief client_receive_data 服务器接受数据
     * @param socket socketid
     * @param sMsg 接收到的数据会存储在这里
     * @param sizeof_sMsg 接受数据的长度
     * @return 成功返回1,失败返回0
     */
    int client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg);

    int client_receive_data_with_end(const int socket, unsigned char * sMsg, unsigned char * endMsg, int sizeof_endMsg);

    /**
     * @brief valid_data_search_crc 在字符串sMsg寻找有效字符串（头为search），并将有效字符串赋给sMsg，有效字符串需要符合crc check
     * @param sMsg 原始字符串
     * @param search 有效字符串的头
     * @param sizeof_sMsg 原始字符串大小
     * @param sizeof_search 头大小
     * @param sizeof_result 有效字符串大小
     * @return 成功返回1,失败返回0
     */
    int valid_data_search_crc(unsigned char * sMsg, const unsigned char * search, int sizeof_sMsg, int sizeof_search, int sizeof_result);

    /**
     * @brief data_check 检查字符串是否包含与check字符串相同的字符串
     * @param sMsg 需要检查的字符串
     * @param check 检查依据的字符串
     * @param sizeof_sMsg 字符串长度
     * @param sizeof_check 检查依据的字符串长度
     * @return 成功返回1,失败返回0
     */
    int data_check(const unsigned char * sMsg, const unsigned char * check, int sizeof_sMsg, int sizeof_check);
	
    /**
     * @brief 断开与server的通讯
     * @param socket socketid
     * @return 成功返回1,失败返回0
     */
	int client_disconnect(const int socket);
	
    /**
     * @brief 设置非阻塞模式
     * @param socket socketid
     * @return 成功返回1,失败返回0
     */
	int setnonblock(const int socket);

    /**
     * @brief 根据crc16校验字符串
     * @param bufData 需要校验的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 校验成功返回1,失败返回0
     */
    int crc16_check(const unsigned char * crc16_check_bufData, int sizeof_bufData);

    /**
     * @brief 为字符串增加crc16校验位，bufData最后两位需要预留，例如bufData为unsigned char[16]，sizeof_bufData=8,其第7、8位将变为第1-6位算出来的crc16校验值
     * @param bufData 需要增加校验位的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 成功返回1,失败返回0
     */
    int crc16_add(unsigned char * bufData, int sizeof_bufData);

    /**
     * @brief 把两位的十六进制转化为整数(unsigned)
     * @param high 高八位字节
     * @param low 第八位字节
     * @param data 转化完的整数
     * @return 成功返回1,失败返回0
     */
    int unsigned_char_to_int(const unsigned char high, const unsigned char low, int &data);

    /**
     * @brief 把两位的十六进制转化为有符号的整数
     * @param high 高八位字节
     * @param low 第八位字节
     * @param data 转化完的整数
     * @return 成功返回1,失败返回0
     */
    int unsigned_char_to_signed_int(const unsigned char high, const unsigned char low, int &data);

    /**
     * @brief 把整数数据转化为两位的十六进制形式
     * @param data 需要转换的整数，-32768到32767
     * @param high 高八位字节
     * @param low 第八位字节
     * @return 成功返回1,失败返回0
     */
    int int_to_unsigned_char(const int data, unsigned char &high, unsigned char &low);
	
protected:

    int get_crc16(const unsigned char * bufData, const int buflen, unsigned char * pcrc);


private:
	//private members here;
};

#endif
