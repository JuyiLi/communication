#include "rs485_communication.h" 
#include <exception>

using namespace std;

rs485_communication::rs485_communication()
{
	//构造函数
}  

rs485_communication::~rs485_communication()
{
	//析构函数
} 

//int rs485_communication::client_connect(const char * _device, const int _baud_rate, int flow_ctrl, int databits, int stopbits, int parity)
int rs485_communication::client_connect(const char * _device, const int _baud_rate)
{
    int baud_rate = _baud_rate; // 波特率
//    int fd = open(_device, O_RDWR | O_NOCTTY | O_NDELAY); // 打开串口设备
    int fd = open(_device, O_RDWR); // 打开串口设备
    /*
     * O_RDWR ： 可读可写
     * O_NOCTTY ：该参数不会使打开的文件成为该进程的控制终端。如果没有指定这个标志，那么任何一个 输入都将会影响用户的进程。
     * O_NDELAY ：这个程序不关心DCD信号线所处的状态,端口的另一端是否激活或者停止。如果用户不指定了这个标志，则进程将会一直处在睡眠状态，直到DCD信号线被激活。
     */

    if (fd == -1) {
        std::cerr << "Failed to open serial device" << errno << std::endl;
        return 0;
    }

    struct serial_rs485 rs485conf;
    rs485conf.flags |= SER_RS485_ENABLED;
//    rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
    rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);

    rs485conf.flags |= SER_RS485_RTS_ON_SEND;

//    rs485conf.flags |= SER_RS485_RX_DURING_TX;
    ioctl (fd, TIOCSRS485, &rs485conf);

    termios options;
    bzero(&options,sizeof(struct termios));
    tcgetattr(fd, &options);

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;

    options.c_cflag &= ~PARENB;
//    options.c_cflag |= PARENB;
//    options.c_cflag &= ~PARODD;

    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_oflag &= ~OPOST;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    options.c_iflag &= ~(IXON|IXOFF);

    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 0;  /* 读取字符的最少个数为1 */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

//    fcntl(fd, F_SETFL,FNDELAY);

    return fd;
}  

int rs485_communication::client_disconnect(const int socket)
{
    if (!device_is_open(socket))
        return -1;
    close(socket);

    return 0;
}

int rs485_communication::client_send_data(const int socket, const unsigned char *cMsg, int sizeof_cMsg)
{
    if (communication::is_print)
    {
        cout << "Send message : ";
        for (int i=0; i<sizeof_cMsg; i++)
        {
            cout << hex << (int)cMsg[i] << " ";
        }
        cout << dec << endl;
    }
    if(write(socket, cMsg, sizeof_cMsg) < 0)
    { // （3）send，客户端向服务端发消息
        perror("client send err");
        return 0;
    }

    return 1;
}

int rs485_communication::client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg)
{
    if (communication::is_print)
    {
        cout << "data receiving ..." << endl;
    }

    int total_receive = 0;
    while (total_receive < sizeof_sMsg)
    {
        unsigned char _sMsg[1000] = {0};
        int res = read(socket, _sMsg, 1000);
        if (communication::is_print)
        {
            cout << "received " << res << " data" << endl;
        }

        if(res <= 0)
        {
            perror("client recv err");
            return 0;
        }

        for (int i=0; i<res; i++)
        {
            sMsg[total_receive+i] = _sMsg[i];
        }

        total_receive = total_receive + res;
    }

    if (communication::is_print)
    {
        cout << "Receive message :";
        for (int i=0; i<sizeof_sMsg; i++)
        {
            cout << hex << (int)sMsg[i] << " ";
        }
        cout << dec << endl;
    }

    return 1;
}

int rs485_communication::client_send_data_with_feedback(const int socket, const unsigned char *cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg)
{
    if (!client_send_data(socket, cMsg, sizeof_cMsg))
    {
        return 0;
    }

    unsigned char buf[1024] = {0};

    if (!client_receive_data(socket, buf, sizeof(buf)))
    {
        return 0;
    }

    if (!data_check(buf, sMsg, sizeof(buf), sizeof_sMsg))
    {
        return 0;
    }
    return 1;
}

int rs485_communication::client_clear_old_data(const int socket)
{
    tcflush(socket, TCIOFLUSH);
    return 1;
}

bool rs485_communication::device_is_open(const int socket)
{
    return socket < 0 ? false : true;
}
