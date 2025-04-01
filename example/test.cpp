#include "communication.h"
#include <unistd.h>
#include <iostream>
#include <string>			//string lib
#include "Eigen/Eigen"
#include <cmath>

using namespace std;
using namespace Eigen;

void test(MatrixXd t)
{
    cout << t << endl;
}

int main(int argc, char *argv[])
{
    /*****************蓝点力传感器测试***********************/
//    const char* IP = "192.168.1.20"; // 串口设备文件
//    int port = 49152; // 波特率

//    communication * rs;
//    rs = new udp_communication();
////    rs->print_on();
//    rs->print_off();

//    int fd = rs->client_connect(IP, port);
//    sleep(1);

//    unsigned char stop_stream[50] = {0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//    unsigned char request_state[8] = {0x12, 0x34, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01};

//    unsigned char state_answer[36] = {0x00};

//    rs->client_send_data(fd, stop_stream, sizeof(stop_stream));
//    sleep(1);

//    int time_step_us=10000;
//    for (int i = 0; i<10000000; i=i+time_step_us)
//    {

//        rs->client_send_data(fd, request_state, sizeof(request_state));
//    //    sleep(1);
//        rs->client_receive_data(fd, state_answer, sizeof(state_answer));

//        int32_t f[6] = {0};
//        double f_N[6] ={0.0};
//        for (int i=0; i<6; i++)
//        {
//            rs->unsigned_char_to_int32(state_answer[15+4*i], state_answer[14+4*i], state_answer[13+4*i], state_answer[12+4*i], f[i]);
//            f_N[i] = f[i]/1000000.0;
//        }

//        cout << "f: " << f_N[0] << " " << f_N[1] << " " << f_N[2] << " " << f_N[3] << " " << f_N[4] << " " << f_N[5] << " " << endl;
//        usleep(time_step_us);
//    }

//    rs->client_disconnect(fd);
    /***************************************************************/

    /*****************NBIT力传感器测试***********************/
//    const char* IP = "192.168.1.1"; // 串口设备文件
//    int port = 1024; // 波特率

//    communication * rs;
//    rs = new communication();
//    rs->is_print = true;
////    rs->print_off();

////    int fd = rs->client_connect(IP, port);
////    sleep(1);

//    unsigned char stop_stream[50] = {0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//    unsigned char data_request[60] = {0x55, 0x52, 0xff, 0xff, 0x1," " 0x0, 0x0, 0x0, 0xe3, 0xa, 0x93, 0x55, 0x53, 0x59, 0x80, 0x52, 0x0, 0xf3, 0x7, 0x12, 0x49, 0x28, 0x55, 0x54, 0x3, 0x3, 0x4c, 0xf7, 0xd0, 0x18, 0xe3, 0xa, 0xc7, 0x55, 0x51, 0xf0, 0xff, 0xf1, 0xff, 0xa, 0xf8, 0xe3, 0xa, 0x77, 0x55, 0x52, 0xff, 0xff, 0x1, 0x0, 0x0, 0x0, 0xe3, 0xa, 0x93, 0x55, 0x53, 0x59, 0x80, 0x51};

//    unsigned char head[2] = {0x55, 0x51};
//    rs->valid_data_search_crc(data_request, head, 60,2, 11, 3);
//    cout << hex << (int)data_request[0] << " " << (int)data_request[1] << " " << (int)data_request[2] << " " << (int)data_request[3] << dec << endl;
    /***************************************************************/

    communication rs;
    int data = 0;
//    unsigned char m[3] = {0xf6, 0x11, 0x82};
    unsigned char m[3] = {0x8c, 0x26, 0x81};
    rs.unsigned_char_3_to_int(m[0], m[1], m[2], data);
    cout << "data: " << data << endl;

    return 1;
}
