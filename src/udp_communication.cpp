#include "udp_communication.h" 

using namespace std;

udp_communication::udp_communication()
{
    //构造函数
}

udp_communication::~udp_communication()
{
    //析构函数
}

int udp_communication::client_connect(const char * IP, const int port)
{
    int client_fd;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
//        perror("client socket err");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(IP);
//    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
    ser_addr.sin_port = htons(port);  //注意网络序转换

    return client_fd;
}

int udp_communication::client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg)
{
    if (is_print)
    {
        cout << "Send message : ";
        for (int i=0; i<sizeof_cMsg; i++)
        {
            cout << hex << (int)cMsg[i] << " ";
        }
        cout << dec << endl;
    }
    if(sendto(socket, cMsg, sizeof_cMsg, 0, (const struct sockaddr *) &ser_addr, (int)sizeof (ser_addr)) < 0)
    { // （3）send，客户端向服务端发消息
        perror("client send err");
        return 0;
    }

    return 1;
}

int udp_communication::client_send_data_with_feedback(const int socket, const unsigned char *cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg)
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

int udp_communication::client_clear_old_data(const int socket)
{
    unsigned char large_buff[1024] = {0};

    socklen_t len_src = sizeof (src);
    recvfrom(socket, large_buff, sizeof(large_buff), MSG_DONTWAIT, (struct sockaddr *)&src, &len_src);
    if (is_print)
    {
        cout << "old message :";
        for (int i=0; i<sizeof(large_buff); i++)
        {
            cout << hex << (int)large_buff[i] << " ";
        }
        cout << dec << endl;
    }

    return 1;
}

int udp_communication::client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg)
{
    if (is_print)
    {
        cout << "data receiving ..." << endl;
    }

    socklen_t len_src = sizeof (src);

    if(recvfrom(socket, sMsg, sizeof_sMsg, MSG_DONTWAIT, (struct sockaddr *)&src, &len_src) <= 0)
    {
//        cout <<
//        perror("client recv err");
//        return 0;
    }

    if (is_print)
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

int udp_communication::client_disconnect(const int socket)
{
    close(socket);
    sleep(1);

    return 0;
}
