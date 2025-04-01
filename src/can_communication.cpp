#include "can_communication.h"

using namespace std;

can_communication::can_communication()
{

}  

can_communication::~can_communication()
{

} 

int can_communication::client_connect(const char * IP, const int port)
{
    num=VCI_FindUsbDevice2(pInfo1);
    if (num==0)
    {
        cerr << "Error: no USBCAN device found!" << endl;
        return -2;
    }
    else
    {
        if (communication::is_print)
        {
            printf(">>USBCAN DEVICE NUM:");printf("%d", num);printf(" PCS");printf("\n");
        }
    }

    if(VCI_OpenDevice(VCI_USBCAN2,0,0)==1)//打开设备
    {
        if (communication::is_print)
        {
            printf(">>open deivce success!\n");//打开设备成功
        }
    }else
    {
        printf(">>open deivce error!\n");
        return -1;
    }

    VCI_INIT_CONFIG config;
    config.AccCode=0;
    config.AccMask=0xFFFFFFFF;
    config.Filter=1;//接收所有帧

    unsigned char baudrate_char[2];
    int_to_unsigned_char(baudrate_map[port], baudrate_char[0], baudrate_char[1]);
    if (communication::is_print)
    {
        cout << ">>波特率设置字节" << hex << (int)baudrate_char[0] << ", " << (int)baudrate_char[1] << dec << endl;
    }
    config.Timing0=baudrate_char[0];/*波特率1000 Kbps  0x00  0x14*/
    config.Timing1=baudrate_char[1];

    config.Mode=0;//正常模式

    if(VCI_InitCAN(VCI_USBCAN2,0,0,&config)!=1)
    {
        printf(">>Init CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);
        return -1;
    }

    if(VCI_StartCAN(VCI_USBCAN2,0,0)!=1)
    {
        printf(">>Start CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);
        return -1;
    }

    if(VCI_InitCAN(VCI_USBCAN2,0,1,&config)!=1)
    {
        printf(">>Init can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);
        return -1;

    }
    if(VCI_StartCAN(VCI_USBCAN2,0,1)!=1)
    {
        printf(">>Start can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2,0);
        return -1;
    }

    return 0;
}

int can_communication::client_disconnect(const int socket)
{
    VCI_ResetCAN(VCI_USBCAN2, 0, 0);//复位CAN1通道。
    usleep(100000);
    VCI_ResetCAN(VCI_USBCAN2, 0, 1);//复位CAN2通道。
    usleep(100000);
    VCI_CloseDevice(VCI_USBCAN2,0);//关闭设备。
    return 0;
}

int can_communication::client_send_data(const int socket, const int id, const unsigned char * cMsg, int sizeof_cMsg)
{
    VCI_CAN_OBJ send[1];
    send[0].ID=0x140+id;
    send[0].SendType=0;
    send[0].RemoteFlag=0;
    send[0].ExternFlag=0;
    send[0].DataLen=sizeof_cMsg;
    for(int i = 0; i < send[0].DataLen; i++)
    {
        send[0].Data[i] = cMsg[i];
    }

    if(VCI_Transmit(VCI_USBCAN2, 0, socket, send, 1) == 1)
    {
        if (communication::is_print)
        {
            printf("CAN1 TX ID:0x%08X",send[0].ID);
            if(send[0].ExternFlag==0) printf(" Standard ");
            if(send[0].ExternFlag==1) printf(" Extend   ");
            if(send[0].RemoteFlag==0) printf(" Data   ");
            if(send[0].RemoteFlag==1) printf(" Remote ");
            printf("DLC:0x%02X",send[0].DataLen);
            printf(" data:0x");

            for(int i=0;i<send[0].DataLen;i++)
            {
                printf(" %02X",send[0].Data[i]);
            }

            printf("\n");
        }
        else
        {
//            usleep(send_sleep_time);
        }
        //send[0].ID+=1;
    }
    else
    {
        cerr << "Error: vci_transmit failed!" << endl;
        return -1;
    }
    
    return 0;
}

int can_communication::client_send_data_with_feedback(const int socket, const unsigned char *cMsg, int sizeof_cMsg, const unsigned char *sMsg, int sizeof_sMsg)
{
//   if (!client_send_data(socket, cMsg, sizeof_cMsg))
//   {
//       return 0;
//   }

//   unsigned char buf[1024] = {0};

//   if (!client_receive_data(socket, buf, sizeof(buf)))
//   {
//       return 0;
//   }

//   if (!data_check(buf, sMsg, sizeof(buf), sizeof_sMsg))
//   {
//       return 0;
//   }
   return 0;
}

int can_communication::client_clear_old_data(const int socket)
{
    if (VCI_ClearBuffer(VCI_USBCAN2, 0, socket) == 1)
    {

    }
    else
    {
        cerr << " Error: clear buff failed!" << endl;
        return -1;
    }

    return 0;
}

int can_communication::client_receive_data(const int socket, unsigned char * sMsg, int sizeof_sMsg)
{
    int reclen = 0;
    VCI_CAN_OBJ rec[3000];
    int ind = socket;
    if((reclen=VCI_Receive(VCI_USBCAN2,0,ind,rec,3000,100))>0)//调用接收函数，如果有数据，进行数据处理显示。
    {
        for(int j=0;j<reclen;j++)
        {
            for(int i = 0; i < rec[j].DataLen; i++)
            {
                sMsg[i] = rec[j].Data[i];
            }
            if (communication::is_print)
            {
                printf("CAN%d RX ID:0x%08X", ind+1, rec[j].ID);//ID
                if(rec[j].ExternFlag==0) printf(" Standard ");//帧格式：标准帧
                if(rec[j].ExternFlag==1) printf(" Extend ");//帧格式：扩展帧
                if(rec[j].RemoteFlag==0) printf(" Data ");//帧类型：数据帧
                if(rec[j].RemoteFlag==1) printf(" Remote ");//帧类型：远程帧
                printf("DLC:0x%02X",rec[j].DataLen);//帧长度
                printf(" data:0x");	//数据
                for(int i = 0; i < rec[j].DataLen; i++)
                {
                    printf(" %02X", rec[j].Data[i]);  
                }
                printf(" TimeStamp:0x%08X",rec[j].TimeStamp);//时间标识。
                printf("\n");
            }
            else
            {
//                usleep(send_sleep_time);
            }
        }
    }

    return 0;
}

