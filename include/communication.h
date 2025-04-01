#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
//#include other .h 

class communication
{
public:	
    virtual ~communication(){};

    virtual int server(){};

    virtual int client(){};

    virtual int server_on(const char * IP, const int port, int & socket_id, int & listen_id){};

    virtual int server_off(const int socket_id, const int listen_id){};

    virtual int server_receive_data(const int socket, char * sMsg, int sizeof_sMsg){};

    virtual int server_send_data(const int socket, const char * cMsg, int sizeof_cMsg){};

    virtual int client_connect(const char * IP, const int port){};

    virtual int client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg){};

    virtual int client_send_data(const int socket, const int id, const unsigned char * cMsg, int sizeof_cMsg){};

    virtual int client_send_data_with_feedback(const int socket, const unsigned char * cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg){};

    virtual int client_clear_old_data(const int socket){};

    virtual int client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg){};
    virtual int client_receive_data_with_end(const int socket, unsigned char * sMsg, unsigned char * endMsg, int sizeof_endMsg){};

    virtual int client_disconnect(const int socket){};

    virtual int setnonblock(const int socket){};

public:
    /**
     * @brief valid_data_search_crc 在字符串sMsg寻找有效字符串（头为search），并将有效字符串赋给sMsg，有效字符串需要符合指定的校验
     * @param sMsg 原始字符串
     * @param search 有效字符串的头
     * @param sizeof_sMsg 原始字符串大小
     * @param sizeof_search 头大小
     * @param sizeof_result 有效字符串大小
     * @param check_type 校验种类,0:不校验，1:crc16,2:crc8,3:sum8
     * @return 成功返回1,失败返回0
     */
    int valid_data_search_crc(unsigned char * sMsg, const unsigned char * search, int sizeof_sMsg, int sizeof_search, int sizeof_result, int check_type = 0);

    /**
     * @brief data_check 检查字符串是否包含与check字符串相同的字符串
     * @param sMsg 需要检查的字符串
     * @param check 检查依据的字符串
     * @param sizeof_sMsg 字符串长度
     * @param sizeof_check 检查依据的字符串长度
     * @return 成功返回1,失败返回0
     */
    int data_check(const unsigned char * sMsg, const unsigned char * check, int sizeof_sMsg, int sizeof_check);

    int get_crc16(const unsigned char * bufData, const int buflen, unsigned char * pcrc);

    /**
     * @brief 根据crc16校验字符串
     * @param bufData 需要校验的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 校验成功返回1,失败返回0
     */
    int crc16_check(const unsigned char * crc16_check_bufData, int sizeof_bufData);

    /**
     * @brief 得到CRC8校验字符
     * @param bufData 需要校验的字符串，不包含校验位
     * @param buflen 需要校验的字符串长度，不包含校验位
     * @param pcrc crc8字符
     * @return 校验成功返回1,失败返回0
     */
    int get_crc8(const unsigned char * bufData, const int buflen, unsigned char &pcrc);

    /**
     * @brief 根据crc8校验字符串
     * @param crc8_check_bufData 需要校验的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 校验成功返回1,失败返回0
     */
    int crc8_check(const unsigned char * crc8_check_bufData, int sizeof_bufData);

    /**
     * @brief 得到sum8校验字符
     * @param bufData 需要校验的字符串，不包含校验位
     * @param headlen 字符串头长度，0表示包含头计算，1表示头长度为1,sum计算不包括头
     * @param contextlen 需要校验的字符串长度，不包含校验位
     * @param psum sum8字符
     * @return 校验成功返回1,失败返回0
     */
    int get_sum8(const unsigned char * bufData, const int headlen, const int contextlen, unsigned char &psum);

    /**
     * @brief 根据sum8校验字符串
     * @param sum8_check_bufData 需要校验的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 校验成功返回1,失败返回0
     */
    int sum8_check(const unsigned char * sum8_check_bufData, int sizeof_bufData);

    /**
     * @brief 为字符串增加crc16校验位，bufData最后两位需要预留，例如bufData为unsigned char[16]，sizeof_bufData=8,其第7、8位将变为第1-6位算出来的crc16校验值
     * @param bufData 需要增加校验位的字符串
     * @param sizeof_bufData 需要校验的字符串长度
     * @return 成功返回1,失败返回0
     */
    int crc16_add(unsigned char * bufData, int sizeof_bufData);

    /**
     * @brief 把两位的十六进制转化为整数
     * @param high 高八位字节
     * @param low 第八位字节
     * @param data 转化完的整数
     * @return 成功返回1,失败返回0
     */
    int unsigned_char_to_int(const unsigned char high, const unsigned char low, int &data);

    /**
     * @brief 把整数数据转化为两位的十六进制形式
     * @param data 需要转换的整数，-32768到32767
     * @param high 高八位字节
     * @param low 第八位字节
     * @return 成功返回1,失败返回0
     */
    int int_to_unsigned_char(const int data, unsigned char &high, unsigned char &low);

    int int32_to_unsigned_char(const int data, unsigned char &high1, unsigned char &high2, unsigned char &high3, unsigned char &high4);

    int float_to_unsigned_char(const float data, unsigned char &high1, unsigned char &high2, unsigned char &high3, unsigned char &high4);

    int unsigned_char_to_signed_int(const unsigned char high, const unsigned char low, int &data);

    int unsigned_char_to_float(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, const unsigned char high_4, float &data);

    int unsigned_char_3_to_int(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, int &data);

    int unsigned_char_to_int32(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, const unsigned char high_4, int32_t &data);

    int unsigned_char_to_int64(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, const unsigned char high_4, const unsigned char high_5, const unsigned char high_6, const unsigned char high_7, const unsigned char high_8, int &data);

    int unsigned_char_array_to_double_list(const unsigned char * ucharArray, double * double_list, int start_pos, int end_pos, char delimiter = ',');

    int double_list_to_unsigned_char_array(double * double_list, int double_list_length, unsigned char ** ucharArray, unsigned char * start_char, unsigned char * end_char, char delimiter = ',');

    void print_on();

    void print_off();

    bool is_print = true;

};

#endif
