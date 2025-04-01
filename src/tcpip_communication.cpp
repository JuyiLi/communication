#include "tcpip_communication.h" 

using namespace std;

#define PORT 8888
#define MAX_LINE 20
#define MSG_SIZE 8
#define DEFAULT_SERVER "127.0.0.3"

#define SUCCESS             1

#define SERVER_SOCKET_ERR   1000
#define SERVER_BIND_ERR     1001
#define SERVER_LISTEN_ERR   1002
#define SERVER_ACCEPT_ERR   1003 
#define SERVER_RECV_ERR     1004
#define SERVER_SEND_ERR     1005

#define CLIENT_INETPTON_ERR 1010
#define CLIENT_SOCKET_ERR   1011
#define CLIENT_CONNECT_ERR  1012
#define CLIENT_SEND_ERR     1013
#define CLIENT_RECV_ERR     1014

tcpip_communication::tcpip_communication()
{
	//构造函数
}  

tcpip_communication::~tcpip_communication()
{
	//析构函数
} 

int tcpip_communication::server()
{   
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);// in socket code,it must be AF_INET(protocol)
    if(listenfd==-1)
    {
        printf("socket create fail\n");
        return -1;
    }
    //second step bind ->server's ip&port for communication to socket created in fist step
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("192.168.30.128");
    //serveraddr.sin_addr.s_addr=atoi(argv[1]);// specify ip address
    serveraddr.sin_port=htons(8005);//specify port
    //printf("%s %s\n",argv[1],argv[2]);
    if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))!=0)
    {
        printf("bind failed \n");
        return -1;
    }
    cout << "server started!" << endl;
    //Third step ->Set socket to listening mode
    /*
    The listen function changes the active connection socket interface into the connected socket interface,
    so that a process can accept the requests of other processes and become a server process.
    In TCP server programming, the listen function changes the process into a server and specifies that the corresponding socket becomes a passive connection.
    */
    if(listen(listenfd,3)!=0)
    {
        printf("Listen failed\n");
        close(listenfd);
        return -1;
    }
    // 4th step -> receive client's request
    int clintfd;//socket for client
    int socklen=sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    cout << "waiting request" << endl;
    clintfd=accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t *)&socklen);
    if(clintfd==-1)
        printf("connect failed\n");
    else
        printf("client %s has connnected\n",inet_ntoa(client_addr.sin_addr));

    // 5th step ->connect with client,receive data and reply OK
    char buffer[1024];
    while (1)
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        cout << "waiting data" << endl;
        iret=recv(clintfd,buffer,sizeof(buffer),0);
        if (iret<=0)
        {
           printf("iret=%d\n",iret); break;
        }
        printf("receive :%s\n",buffer);
//        cout << buffer[0] << endl;

        if (!strcmp(buffer, "exit"))
        {
            strcpy(buffer,"exit");
            if ( (iret=send(clintfd,buffer,strlen(buffer),0))<=0)
            {
                perror("send");
                break;
            }
            printf("send :%s\n",buffer);
            break;
        }
        else
        {
            strcpy(buffer,"ok");//reply cilent with "ok"
            if ( (iret=send(clintfd,buffer,strlen(buffer),0))<=0)
            {
                perror("send");
                break;
            }
            printf("send :%s\n",buffer);
        }
    }
    // 6th close socket
    close(listenfd); close(clintfd);

    return 0;
}

int tcpip_communication::client()
{
	char cMsg[MSG_SIZE], sMsg[MSG_SIZE];
	memset(cMsg, 0, sizeof(cMsg));
    memset(sMsg, 0, sizeof(sMsg));

    struct sockaddr_in serverAddr; // 一个将来与套接字绑定的结构体
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_port = htons(PORT); // 从客户端的PORT端口接收服务器信息
    serverAddr.sin_family = AF_INET; // 协议簇，AF_INET表示TCP/IP协议   
    serverAddr.sin_addr.s_addr = inet_addr(DEFAULT_SERVER);

    int fd_skt = socket(AF_INET, SOCK_STREAM, 0); // （1）socket函数新建套接字fd_skt
    if (fd_skt < 0) 
    {
        perror("client socket err");
        return 0;
    }
    
    if (connect(fd_skt, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    { //（2）connect向服务器发起连接请求
        perror("client connect err");       
        return 0;
    }

    if (communication::is_print)
    {
        cout << "Connect Success! \nBegin to chat..." << endl;
    }

    while(1) 
    {
        memset(cMsg, 0, sizeof(cMsg));
        memset(sMsg, 0, sizeof(sMsg));

        //cin.ignore(1024,'\n'); // 去除上一个cin残留在缓冲区的\n
        if (communication::is_print)
        {
            cout << "pls enter massage: " << endl;
        }
        cin.getline(cMsg, MSG_SIZE); // 不用cin，因为不能含空格

        if (communication::is_print)
        {
            cout << "get input massage!" << sizeof(cMsg) << endl;
        }
        if(strcmp(cMsg, "quit\n") == 0)
        {
            break;
        }
        if(send(fd_skt, cMsg, strlen(cMsg), 0) < 0) 
        { // （3）send，客户端向服务端发消息
            perror("client send err");
            return 0;
        }

        if (communication::is_print)
        {
            cout << "Send message to <" << serverAddr.sin_addr.s_addr << ">: " << cMsg << endl;
        }
    }
    close(fd_skt);

    return 1;
}

int tcpip_communication::server_on(const char *IP, const int port, int &socket_id, int &listen_id)
{
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);// in socket code,it must be AF_INET(protocol)
    if(listenfd==-1)
    {
        printf("socket create fail\n");
        return 0;
    }
    //second step bind ->server's ip&port for communication to socket created in fist step
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(IP);
    //serveraddr.sin_addr.s_addr=atoi(argv[1]);// specify ip address
    serveraddr.sin_port=htons(port);//specify port
    //printf("%s %s\n",argv[1],argv[2]);
    if(bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))!=0)
    {
        printf("bind failed \n");
        return 0;
    }
    cout << "server started!" << endl;
    //Third step ->Set socket to listening mode
    /*
    The listen function changes the active connection socket interface into the connected socket interface,
    so that a process can accept the requests of other processes and become a server process.
    In TCP server programming, the listen function changes the process into a server and specifies that the corresponding socket becomes a passive connection.
    */
    if(listen(listenfd,3)!=0)
    {
        printf("Listen failed\n");
        close(listenfd);
        return 0;
    }
    // 4th step -> receive client's request
    int clintfd;//socket for client
    int socklen=sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    cout << "waiting request" << endl;
    clintfd=accept(listenfd,(struct sockaddr*)&client_addr,(socklen_t *)&socklen);
    if(clintfd==-1)
    {
        printf("connect failed\n");
        return 0;
    }
    else
        printf("client %s has connnected\n",inet_ntoa(client_addr.sin_addr));

    socket_id = clintfd;
    listen_id = listenfd;

    return 1;
}

int tcpip_communication::server_off(const int socket_id, const int listen_id)
{
    close(listen_id);
    close(socket_id);

    return 0;
}

int tcpip_communication::server_receive_data(const int socket, char *sMsg, int sizeof_sMsg)
{
    int iret;
    memset(sMsg,0,sizeof_sMsg);
    cout << "waiting data" << endl;

    iret=recv(socket,sMsg,sizeof_sMsg,0);
    if (iret<=0)
    {
       printf("iret=%d\n",iret);
       return 1;
    }
    printf("receive :%s\n",sMsg);

    return 0;
}

int tcpip_communication::server_send_data(const int socket, const char *cMsg, int sizeof_cMsg)
{
    int iret;
    if ( (iret=send(socket,cMsg,sizeof_cMsg,0))<=0)
    {
        perror("send");
        return 1;
    }
    printf("send :%s\n",cMsg);
    return 0;
}

int tcpip_communication::client_connect(const char * IP, const int port)
{
    struct sockaddr_in serverAddr; // 一个将来与套接字绑定的结构体
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_port = htons(port); // 从客户端的PORT端口接收服务器信息
    serverAddr.sin_family = AF_INET; // 协议簇，AF_INET表示TCP/IP协议
    serverAddr.sin_addr.s_addr = inet_addr(IP);   
    
    if(inet_pton(AF_INET, IP, (void *)&serverAddr.sin_addr) <= 0) 
    {
        perror("client inet_pton err");
        return 0;
    }

    int fd_skt = socket(AF_INET, SOCK_STREAM, 0); // （1）socket函数新建套接字fd_skt
    if (fd_skt < 0) 
    {
        perror("client socket err");
        return 0;
    }
    
    if (connect(fd_skt, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    { //（2）connect向服务器发起连接请求
        perror("client connect err");       
        return 0;
    }

    if (communication::is_print)
    {
        cout << "Connect Success! \nBegin to chat..." << endl;
    }
    
    return fd_skt;
}

int tcpip_communication::client_send_data(const int socket, const unsigned char * cMsg, int sizeof_cMsg)
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
    if(send(socket, cMsg, sizeof_cMsg, 0) < 0) 
    { // （3）send，客户端向服务端发消息
        perror("client send err");
        return 0;
    }  
    
    return 1;
}

int tcpip_communication::client_send_data_with_feedback(const int socket, const unsigned char *cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg)
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

int tcpip_communication::client_clear_old_data(const int socket)
{
    unsigned char large_buff[1024] = {0};
    recv(socket, large_buff, sizeof(large_buff), MSG_DONTWAIT);
    if (communication::is_print)
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

int tcpip_communication::client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg)
{
    if (communication::is_print)
    {
        cout << "data receiving ..." << endl;
    }

    int received_len = 0;
    if (communication::is_print)
    {
        cout << "Receive message : " << endl;
    }

    int receive_count = 0;
    while (received_len < sizeof_sMsg)
    {
//        unsigned char rec_this[1023];
//        int rec_len_this = recv(socket, rec_this, sizeof (rec_this), MSG_DONTWAIT);
        unsigned char rec_this[1];
        int rec_len_this = recv(socket, rec_this, sizeof (rec_this), MSG_WAITALL);

        if(rec_len_this <= 0)
        {
            receive_count++;
//            cout << receive_count << endl;
            if (receive_count > 1000)
            {
                perror("client recv err");
                return 0;
            }
            else
            {
                usleep(100);
                continue;
            }
        }

        for (int i= 0; i<rec_len_this; i++)
        {
            sMsg[received_len+i] = rec_this[i];
            if (communication::is_print)
            {
                cout << hex << (int)sMsg[received_len+i] << " ";
            }
        }
        if (communication::is_print)
        {
            cout << endl;
        }

        received_len = received_len + rec_len_this;
    }

    if (communication::is_print)
    {
        cout << dec << endl;
    }

	return 1;
}

int tcpip_communication::client_receive_data_with_end(const int socket, unsigned char * sMsg, unsigned char * endMsg, int sizeof_endMsg)
{
    if (communication::is_print)
    {
        cout << "data receiving ..." << endl;
    }

    int received_len = 0;
    if (communication::is_print)
    {
        cout << "Receive message : " << endl;
    }

    int right_endMsg_count = 0;
    int receive_count = 0;
    while (right_endMsg_count<3)
    {
//        unsigned char rec_this[1023];
//        int rec_len_this = recv(socket, rec_this, sizeof (rec_this), MSG_DONTWAIT);
        cout << "len:" << received_len << endl;
        unsigned char rec_this[1];
        int rec_len_this = recv(socket, rec_this, sizeof (rec_this), MSG_WAITALL);
        cout << rec_len_this << endl;

        if(rec_len_this <= 0)
        {
            receive_count++;
//            cout << receive_count << endl;
            if (receive_count > 1000)
            {
                perror("client recv err");
                return 0;
            }
            else
            {
                usleep(100);
                continue;
            }
        }
        else
        {
            receive_count=0;
        }

        for (int i= 0; i<rec_len_this; i++)
        {
            sMsg[received_len+i] = rec_this[i];
            if (communication::is_print)
            {
                cout << hex << (int)sMsg[received_len+i] << " ";
            }
        }
        if (communication::is_print)
        {
            cout << endl;
        }

        received_len = received_len + rec_len_this;

        right_endMsg_count=0;
        if (received_len>sizeof_endMsg)
        {
            for (int i=0; i<sizeof_endMsg; i++)
            {
                if (communication::is_print)
                {
                    cout << "sMsg:" << sMsg[received_len-1-i] << " vs " << "endMsg:" << endMsg[sizeof_endMsg-1-i] << endl;
                }
                if (sMsg[received_len-1-i] == endMsg[sizeof_endMsg-1-i])
                {
                    right_endMsg_count++;
                }
                if (communication::is_print)
                {
                    cout << "right endMsg count:" << right_endMsg_count << endl;
                }
            }
        }

    }

    if (communication::is_print)
    {
        cout << dec << endl;
    }

    return 1;
}

int tcpip_communication::valid_data_search_crc(unsigned char *sMsg, const unsigned char *search, int sizeof_sMsg, int sizeof_search, int sizeof_result)
{
    if (sizeof_result > sizeof_sMsg)
    {
        if (communication::is_print)
        {
            cout << "Error: the size of message is not enough!" << endl;
        }
        return 0;
    }

    if (sizeof_search > sizeof_result)
    {
        if (communication::is_print)
        {
            cout << "Error: size of head is larger than valid message" << endl;
        }
        return 0;
    }

    unsigned char res[sizeof_result];

    for (int i=sizeof_sMsg-sizeof_result; i>-1; i--)
    {
        if (sMsg[i] == search[0])
        {
            for (int j=0; j<sizeof_search; j++)
            {
                if (sMsg[i+j] != search[j])
                {
                    break;
                }

                if (j == sizeof_search-1)
                {
                    for (int k=0; k<sizeof_result; k++)
                    {
                        res[k] = sMsg[i+k];
                    }

                    if (!crc16_check(res, sizeof_result))
                    {
                        break;
                    }
                    else
                    {
                        for (int k=0; k<sizeof_result; k++)
                        {
                            sMsg[k] = res[k];
                        }
                        if(communication::is_print)
                        {
                            cout << "valid message :";
                            for (int k=0; k<sizeof_result; k++)
                            {
                                cout << hex << (int)sMsg[k] << " ";
                            }
                            cout << dec << endl;
                        }
                        return 1;
                    }
                }
            }
        }
    }

    cout << "Error: cannot find valid massage!" << endl;
    return 0;
}

int tcpip_communication::data_check(const unsigned char *sMsg, const unsigned char *check, int sizeof_sMsg, int sizeof_check)
{
//    if (sizeof_sMsg > sizeof(sMsg))
//    {
//        cout << "Error: the size of msg is not enough!" << endl;
//        return 0;
//    }
//    if (sizeof_sMsg > sizeof(check))
//    {
//        cout << "Error: the size of check msg is not enough!" << endl;
//        return 0;
//    }

    if (sizeof_check > sizeof_sMsg)
    {
        if (communication::is_print)
        {
            cout << "Error: the size of message is not enough!" << endl;
        }
        return 0;
    }

    for (int i=sizeof_sMsg-sizeof_check; i>-1; i--)
    {
        if (sMsg[i] == check[0])
        {
            for (int j=0; j<sizeof_check; j++)
            {
                if (sMsg[i+j] != check[j])
                {
                    break;
                }

                if (j == sizeof_check-1)
                {
                    if (communication::is_print)
                    {
                        cout << "check passes!" << endl;
                    }
                    return 1;
                }
            }
        }
    }

    if (communication::is_print)
    {
        cout << "Error: check does not pass!" << endl;
    }
    return 0;
}

int tcpip_communication::client_disconnect(const int socket)
{
	close(socket);
	sleep(1);
	
	return 1;
}

int tcpip_communication::setnonblock(const int socket)
{
	int iFlags;
 
    iFlags = fcntl(socket, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(socket, F_SETFL, iFlags);
    return ret;
}

int tcpip_communication::get_crc16(const unsigned char * bufData, const int buflen, unsigned char *pcrc)
{
	unsigned short CRC = 0xffff;
	unsigned short POLYNOMIAL = 0xa001;
	int i, j;
 
	//指令为空
	if(bufData == NULL || pcrc == NULL)
	{
        return 0;
	}
 
	//校验计算的长度为0
	if (buflen == 0)
	{
        return 1;
	}
 
	for (i = 0; i < buflen; i++)
	{
		CRC ^= bufData[i];
 
		//总共八次右移操作
		for (j = 0; j < 8; j++)
		{
			if ((CRC & 0x0001) != 0)
			{
				//右移的移出位为1
				CRC >>= 1;
				CRC ^= POLYNOMIAL;
			}
			else
			{
                //右移的移出位为0
				CRC >>= 1;
			}
		}
	}
 
    if (communication::is_print)
    {
        printf ("CRC=%X\n", CRC);
    }
    //低位在前，高位在后
	pcrc[0] = (unsigned char)(CRC & 0x00ff);
	pcrc[1] = (unsigned char)(CRC >> 8);
 
    return 1;
}

int tcpip_communication::crc16_check(const unsigned char *crc16_check_bufData, int sizeof_bufData)
{
    if (communication::is_print)
    {
        cout << "bufdata is" << hex;
        for (int i=0; i<sizeof_bufData; i++)
        {
            cout << int(crc16_check_bufData[i]) << " ";
        }
        cout << dec << endl;
    }
    int length = sizeof_bufData;
//    if (length > sizeof(crc16_check_bufData))
//    {
//        cout << "Error: the length of data is not enough!" << sizeof(crc16_check_bufData) << "/" << length << endl;
//        return 0;
//    }

    unsigned char buf[length-2];
    for (int i=0; i<length-2; i++)
    {
        buf[i] = crc16_check_bufData[i];
    }

    unsigned char crc_calculated[2];
    if (!get_crc16(buf,length-2,crc_calculated))
    {
        cout << "Error: crc16 calculation occurs error!" << endl;
        return 0;
    }

    if (crc16_check_bufData[length-2] == crc_calculated[0] and crc16_check_bufData[length-1] == crc_calculated[1])
    {
        if (communication::is_print)
        {
            cout << "crc16 check passed!" << endl;
        }
        return 1;
    }
    else if (crc16_check_bufData[length-2] == crc_calculated[1] and crc16_check_bufData[length-1] == crc_calculated[0])
    {
        cout << "Error: crc16 results has wrong order!" << endl;
        return 0;
    }
    else
    {
        cout << "Error: crc16 check does not pass!" << endl;
        return 0;
    }
}

int tcpip_communication::crc16_add(unsigned char *bufData, int sizeof_bufData)
{
    int length = sizeof_bufData;

//    if (length > sizeof(bufData))
//    {
//        cout << "Error: the length of data is not enough!" << endl;
//        return 0;
//    }

    unsigned char buf[length-2];
    for (int i=0; i<length-2; i++)
    {
        buf[i] = bufData[i];
    }

    unsigned char crc_calculated[2];
    if (!get_crc16(buf,length-2,crc_calculated))
    {
        cout << "Error: crc16 calculation occurs error!" << endl;
        return 0;
    }

    bufData[length-2] = crc_calculated[0];
    bufData[length-1] = crc_calculated[1];
    if (communication::is_print)
    {
        cout << "data with crc: " << hex;
        for (int i=0; i<length; i++)
        {
            cout << int(bufData[i]) << " ";
        }
        cout << dec << endl;
    }
    return 1;
}

int tcpip_communication::unsigned_char_to_int(const unsigned char high, const unsigned char low, int &data)
{
    data = (((short)high << 8) & 0xff00) | (short(low) & 0x00ff);

    return 1;
}

int tcpip_communication::unsigned_char_to_signed_int(const unsigned char high, const unsigned char low, int &data)
{
    int data_unsigned = 0;

    if (!unsigned_char_to_int(high, low, data_unsigned))
        return 0;

    if (data_unsigned < 256*256/2)
    {
        data = data_unsigned;
    }
    else
    {
        data = data_unsigned-256*256;
    }

    return 1;
}

int tcpip_communication::int_to_unsigned_char(const int data, unsigned char &high, unsigned char &low)
{
    high = data/256;
    low = data%256;
    if (communication::is_print)
    {
        cout << "convert " << data << " to " << hex << int(high) << " " << int(low) << dec << endl;
    }

    return 1;
}
