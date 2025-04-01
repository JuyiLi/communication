#include "udp_communication.h"
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
    const char* IP = "192.168.1.1"; // 串口设备文件
    int port = 1024; // 波特率

    communication * rs;
    rs = new udp_communication();
//    rs->print_on();
    rs->print_off();

    int fd = rs->client_connect(IP, port);
    sleep(1);

    unsigned char stop_stream[50] = {0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char data_request[7] = {0x01, 0x03, 0x02, 0x00, 0x12, 0x38, 0x49};

    unsigned char state_answer[23] = {0x00};

    rs->client_send_data(fd, stop_stream, sizeof(stop_stream));
    sleep(1);

    int time_step_us=5000;
    double data_list[6] = {0.0};
    for (int i = 0; i<10000000; i=i+time_step_us)
    {

        rs->client_send_data(fd, data_request, sizeof(data_request));
    //    sleep(1);
        rs->client_receive_data(fd, state_answer, sizeof(state_answer));

        int data_int[6] = {0};
        for (int i=0; i<6; i++)
        {
            rs->unsigned_char_3_to_int(state_answer[3*i+5], state_answer[3*i+4], state_answer[3*i+3], data_int[i]);
        }

        {
//            int a = 0;
//            a = com->crc16_check(str,answer_length);

//            if(!a)
//            {
//                cout << "Error: crc check failed!" << endl;
//                return -4;
//            }
        }

        {
            data_list[0] = data_int[0]/1000.0;
            data_list[1] = data_int[1]/1000.0;
            data_list[2] = data_int[2]/1000.0;
            data_list[3] = data_int[3]/1000.0;
            data_list[4] = data_int[4]/1000.0;
            data_list[5] = data_int[5]/1000.0;
        }

        cout << "f: " << data_list[0] << " " << data_list[1] << " " << data_list[2] << " " << data_list[3] << " " << data_list[4] << " " << data_list[5] << " " << endl;
        usleep(time_step_us);
    }

    rs->client_disconnect(fd);
    /***************************************************************/

    return 1;
}
