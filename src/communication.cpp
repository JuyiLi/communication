#include <iostream>
#include "communication.h" 

using namespace std;

void communication::print_on()
{
    is_print = true;
}

void communication::print_off()
{
    is_print = false;
}

int communication::valid_data_search_crc(unsigned char *sMsg, const unsigned char *search, int sizeof_sMsg, int sizeof_search, int sizeof_result, int check_type)
{
    if (sizeof_result > sizeof_sMsg)
    {
        cerr << "Error: the size of message is not enough!" << endl;
        return 0;
    }

    if (sizeof_search > sizeof_result)
    {

        cerr << "Error: size of head is larger than valid message" << endl;
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
                    if (is_print)
                    {
                        cout << hex << (int)sMsg[i+j] << dec << "is different from " << hex << (int)search[j] << dec << endl;
                    }
                    break;
                }

                if (j == sizeof_search-1)
                {
                    for (int k=0; k<sizeof_result; k++)
                    {
                        res[k] = sMsg[i+k];
                    }

                    int check_res = 0;
                    if (check_type == 0)
                    {
                        check_res = 1;
                    }
                    else if (check_type == 1)
                    {
                        check_res = crc16_check(res, sizeof_result);
                    }
                    else if (check_type == 2)
                    {
                        check_res = crc8_check(res, sizeof_result);
                    }
                    else if (check_type == 3)
                    {
                        check_res = sum8_check(res, sizeof_result);
                    }

                    if (!check_res)
                    {
                        break;
                    }
                    else
                    {
                        for (int k=0; k<sizeof_result; k++)
                        {
                            sMsg[k] = res[k];
                        }
                        if(is_print)
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

    cout << "origin message :";
    for(int i=0; i<sizeof_sMsg; i++)
    {
        cout << dec << "0x" << hex << (int)sMsg[i] << dec << "," << " ";
    }
    cout << dec << endl;
    cout << "Error: cannot find valid massage!" << endl;
    return 0;
}

int communication::data_check(const unsigned char *sMsg, const unsigned char *check, int sizeof_sMsg, int sizeof_check)
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
        if (is_print)
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
                    if (is_print)
                    {
                        cout << "check passes!" << endl;
                    }
                    return 1;
                }
            }
        }
    }

    if (is_print)
    {
        cout << "Error: check does not pass!" << endl;
    }
    return 0;
}

int communication::get_crc16(const unsigned char * bufData, const int buflen, unsigned char *pcrc)
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

    if (is_print)
    {
        printf ("CRC=%X\n", CRC);
    }
    //低位在前，高位在后
    pcrc[0] = (unsigned char)(CRC & 0x00ff);
    pcrc[1] = (unsigned char)(CRC >> 8);

    return 1;
}

int communication::get_crc8(const unsigned char *bufData, const int buflen, unsigned char &pcrc)
{
    unsigned short CRC = 0xff;
    unsigned short POLYNOMIAL = 0x07;
    int i, j;

    //指令为空
    if(bufData == NULL)
    {
        cerr << "Error: no bufdata input!" << endl;
        return 0;
    }

    //校验计算的长度为0
    if (buflen == 0)
    {
        cerr << "Error: wrong bufdata length!" << endl;
        return 0;
    }

    for (i = 0; i < buflen; i++)
    {
        CRC ^= bufData[i];

        //总共八次右移操作
        for (j = 0; j < 8; j++)
        {
            if (CRC & 0x80)
            {
                CRC = (CRC << 1) ^ POLYNOMIAL;
            }
            else
            {
                CRC <<= 1;
            }

        }
    }

    if (is_print)
    {
        printf ("CRC=%X\n", CRC);
    }
    //低位在前，高位在后
    pcrc = (unsigned char)CRC;

    return 1;
}

int communication::get_sum8(const unsigned char *bufData, const int headlen, const int contextlen, unsigned char &psum)
{
    int checksum = 0;
    for (int i = headlen; i<contextlen+headlen; i++)
    {
        checksum += (int)bufData[i];
    }

    psum = (unsigned char)(checksum & 0x00ff);

    if(is_print)
    {
        cout << "checksum:" << hex << (int)psum << dec << endl;
    }

    return 1;
}

int communication::crc16_check(const unsigned char *crc16_check_bufData, int sizeof_bufData)
{
    if (is_print)
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
        if (is_print)
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

int communication::crc8_check(const unsigned char *crc8_check_bufData, int sizeof_bufData)
{
    if (is_print)
    {
        cout << "bufdata is" << hex;
        for (int i=0; i<sizeof_bufData; i++)
        {
            cout << int(crc8_check_bufData[i]) << " ";
        }
        cout << dec << endl;
    }
    int length = sizeof_bufData;
//    if (length > sizeof(crc16_check_bufData))
//    {
//        cout << "Error: the length of data is not enough!" << sizeof(crc16_check_bufData) << "/" << length << endl;
//        return 0;
//    }

    unsigned char buf[length-1];
    for (int i=0; i<length-1; i++)
    {
        buf[i] = crc8_check_bufData[i];
    }

    unsigned char crc_calculated;
    if (!get_crc8(buf,length-1,crc_calculated))
    {
        cout << "Error: sum8 calculation occurs error!" << endl;
        return 0;
    }

    if (crc8_check_bufData[length-1] == crc_calculated)
    {
        if (is_print)
        {
            cout << "crc8 check passed!" << endl;
        }
        return 1;
    }
    else
    {
        cout << "Error: crc8 check does not pass!" << endl;
        return 0;
    }
}

int communication::sum8_check(const unsigned char *sum8_check_bufData, int sizeof_bufData)
{
    if (is_print)
    {
        cout << "bufdata is" << hex;
        for (int i=0; i<sizeof_bufData; i++)
        {
            cout << int(sum8_check_bufData[i]) << " ";
        }
        cout << dec << endl;
    }
    int length = sizeof_bufData;
//    if (length > sizeof(crc16_check_bufData))
//    {
//        cout << "Error: the length of data is not enough!" << sizeof(crc16_check_bufData) << "/" << length << endl;
//        return 0;
//    }

    unsigned char buf[length-1];
    for (int i=0; i<length-1; i++)
    {
        buf[i] = sum8_check_bufData[i];
    }

    unsigned char sum_calculated;
    if (!get_sum8(buf,0,length-1,sum_calculated))
    {
        cout << "Error: sum8 calculation occurs error!" << endl;
        return 0;
    }

    if (sum8_check_bufData[length-1] == sum_calculated)
    {
        if (is_print)
        {
            cout << "sum8 check passed!" << endl;
        }
        return 1;
    }
    else
    {
        cout << "Error: sum8 check does not pass!" << endl;
        return 0;
    }
}

int communication::crc16_add(unsigned char *bufData, int sizeof_bufData)
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
    if (is_print)
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

int communication::unsigned_char_to_int(const unsigned char high, const unsigned char low, int &data)
{
    int _data = (((short)high << 8) & 0xff00) | (short(low) & 0x00ff);
    data = _data;

    return 1;
}

int communication::int_to_unsigned_char(const int data, unsigned char &high, unsigned char &low)
{
    if (data >=0)
    {
        high = data/256;
        low = data%256;
    }
    else
    {
        int positive_data = -data+1;
        high = 0xff - positive_data/256;
        low = 0xff - positive_data%256;
    }
    if (is_print)
    {
        cout << "convert " << data;
        cout << " to " << hex << int(high) << " " << int(low) << dec << endl;
    }

    return 1;
}

int communication::int32_to_unsigned_char(const int data, unsigned char &high1, unsigned char &high2, unsigned char &high3, unsigned char &high4)
{
    if (data >=0)
    {
        high1 = data/16777216;
        int buf = data%16777216;
        high2 = buf/65536;
        buf = buf%65536;
        high3 = buf/256;
        high4 = buf%256;
    }
    else
    {
        int positive_data = -data+1;
        high1 = 0xff - positive_data/16777216;
        int buf = positive_data%16777216;
        high2 = 0xff - buf/65536;
        buf = buf%65536;
        high3 = 0xff - buf/256;
        high4 = 0xff - buf%256;
    }

    if (is_print)
    {
        cout << "convert " << data;
        cout << " to " << hex << int(high1) << " " << int(high2) << " " << int(high3) << " " << int(high4) << dec << endl;
    }

    return 1;
}

int communication::float_to_unsigned_char(const float data, unsigned char &high1, unsigned char &high2, unsigned char &high3, unsigned char &high4)
{
    int data_int = (int &)data;
    high1 = (unsigned char)((data_int & 0xff000000)>>24);
    high2 = (unsigned char)((data_int & 0x00ff0000)>>16);
    high3 = (unsigned char)((data_int & 0x0000ff00)>>8);
    high4 = (unsigned char)((data_int & 0x000000ff));

    return 1;
}

int communication::unsigned_char_to_signed_int(const unsigned char high, const unsigned char low, int &data)
{
    unsigned_char_to_int(high, low, data);
    if (data > 32767)
    {
        data = data-65535;
    }

    return 1;
}

int communication::unsigned_char_to_float(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, const unsigned char high_4, float &data)
{
    unsigned char c[4] = {high_4, high_3, high_2, high_1};
    memcpy(&data, c, 4);

    return 1;
}

int communication::unsigned_char_3_to_int(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, int &data)
{
    int symbol = 1;
    unsigned char high_3_unsigned = 0x00;
    if (high_3<0x80)
    {
        high_3_unsigned = high_3;
        symbol = 1;
    }
    else
    {
        high_3_unsigned = high_3-0x80;
        symbol = -1;
    }

//    unsigned char c[3] = {high_1, high_2, high_3_unsigned};
//    memcpy(&data, c, 4);
    data = (int)high_1+(int)high_2*256+(int)high_3_unsigned*256*256;
    data = data*symbol;

    return 1;
}

int communication::unsigned_char_to_int32(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, unsigned char high_4, int32_t &data)
{
    data = (uint32_t)high_1 << 24 |
          (uint32_t)high_2 << 16 |
          (uint32_t)high_3 << 8  |
          (uint32_t)high_4;

    return 1;
}

int communication::unsigned_char_to_int64(const unsigned char high_1, const unsigned char high_2, const unsigned char high_3, const unsigned char high_4, const unsigned char high_5, const unsigned char high_6, const unsigned char high_7, const unsigned char high_8, int &data)
{
    data =  (uint64_t)high_1 << 56 |
            (uint64_t)high_2 << 48 |
            (uint64_t)high_3 << 40  |
            (uint64_t)high_4 << 32  |
            (uint64_t)high_5 << 24 |
            (uint64_t)high_6 << 16 |
            (uint64_t)high_7 << 8  |
            (uint64_t)high_8;

    return 1;
}

int communication::unsigned_char_array_to_double_list(const unsigned char * ucharArray, double * double_list, int start_pos, int end_pos, char delimiter)
{
    string str((char*)ucharArray);
    stringstream ss(str);
    string token;
    vector<double> double_vector;

    while(std::getline(ss, token, delimiter))
    {
        double value = 0;
        try{
             value = std::stod(token);
        }
        catch(const invalid_argument& ia){
            continue;
        }
        double_vector.push_back(value);
        if (is_print)
            {
            cout << "value: " << value << endl;
        }
    }

    for (int i=0; i<end_pos-start_pos+1; i++)
        {
        double_list[i] = double_vector[start_pos+i];
    }

    return 0;
}

int communication::double_list_to_unsigned_char_array(double * double_list, int double_list_length, unsigned char ** ucharArray, unsigned char * start_char, unsigned char * end_char, char delimiter)
{
    stringstream oss;
    oss << start_char;
    for (int i=0; i<double_list_length; i++)
    {
        oss << double_list[i];
        if (i<double_list_length-1)
        {
            oss<<delimiter;
        }
    }
    oss << end_char;
    if (is_print)
    {
        cout << "double list to unsigned char array ostringstream:" << oss.str() << endl;
    }

    string str = oss.str();
    cout << "size of str" << str << endl;
    *ucharArray = new unsigned char[str.size()+1];
    strcpy(reinterpret_cast<char*>(*ucharArray), str.c_str());

    if (is_print)
    {
        cout << "double list to unsigned char array result:" << *ucharArray << endl;
    }

    return str.length();
}
