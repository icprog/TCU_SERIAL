#if 1

#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>
#include <unistd.h>     /*Unix标准函数定义*/
#include <sys/types.h>  /**/
#include <sys/stat.h>   /**/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include "card_api.h"

char rulerinfo[] = 
	{	
	"//=========================================================================\n"
	"//==15H	功能命令类别(NAK)	接收数据错误（DKQ）   10H	查看版本命令代码\n"
	"//==30H	功能命令类别  01H	设置串口通讯波特率\n"
	"//===31H	功能命令类别  11H	查看读卡器版本信息\n"
	"//============================13H	蜂鸣器控制命令代码\n"
	"//============================14H	设置 LED 灯状态命令代码\n"
	"//============================90H	打开射频\n"
   	"//============================91H	关闭射频\n"
	"//============================26H	功能指令：进入低功耗\n"
	"//===32H	功能命令类别  22H	卡上电命令代码\n"
	"//============================23H	卡下电命令代码\n"
	"//============================42H	弹卡命令代码\n"
	"//============================41H	接触式卡弹卡命令代码\n"
	"//============================24H	激活非接触卡命令代码\n"
	"//============================26H	操作卡片数据命令代码   应用层传输\n"
	"//============================93H	操作卡片数据命令代码   非接触卡直通命令\n"
	"//==========================================================================\n"

	};



#define TITLE "CPU_CARD TEST"
#define FALSE 1
#define TRUE 0

#define TEST_BUFF_SIZE 1024

//UINT8 outbuf[512] = {0};/* Recvice data buffer */
//UINT8 message[7]={0x02,0x00,0x02,0x31,0x11,0x20,0x03};
//UINT8 sendBuf[100]={0};

void debug_card_switch(int show)
{
    g_need_pack_2016 = show;
}

/*****************************************************************************
 函 数 名  : void unpack_card_frame_new(int outlength, char *outBuffer)
 功能描述  : 解析645协议帧
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年07月14日
    作    者   : andy
    修改内容   : 新生成函数

*****************************************************************************/

void unpack_card_frame_new(int outlength, char *outBuffer)
{		
	int i,j = 0;
	int ret = 0;
	char buf[512] = {0};
	int len = outlength;
    S_CARD_UNPACK stUnPack = {0};

	//printf("buffer = ");
	#if 0
	for(i = 2; i < argc; i++)
	{
		printf("argv %s  ", argv[i]);
		buf[j++] = strtol(argv[i], NULL,16);		
	}
	len = j;
#endif
	
	
	#if 1
	printf("len===%d\n",len);
	memset (buf, '\0', sizeof(buf));
	memcpy(buf,outBuffer,len);
	for(j=0;j<len;j++){
		printf("111buf[%d]==0x%2x\n", j,buf[j]);
	}
	#endif
	
    debug_card_switch(CARD_ON);
    ret = unpack_card_frame(buf, len, &stUnPack);
    debug_card_switch(CARD_OFF);
    if(ret != E_CARD_OK)
	{
		printf("\n  解析出错! (error = %d : ",ret);
		switch(ret)
		{
            case E_CARD_ERRO_FRAME_UNCOMP:
				printf("不完整的帧数据)\n");
				break;

            case E_CARD_ERRO_FRAME_0x68:
				printf("起始符 0x68 的位置不对)\n");
				break;
				
            case E_CARD_ERRO_FRAME_CHECK_SUM:
				printf("测试帧检验和不对)\n");
				break;
				
            case E_CARD_ERRO_FRAME_END_0x16:
				printf("测试帧结束符 0x16 不对)\n");
				break;
			default:
				break;
		}
		
		printf("\n");
		return;
	}
	
	// 显示结果
	printf("/------------------unpack result-------------------\\\n");
	printf("\n<1> - ruler info:\n");
	show_ruler_info(stUnPack.ruler_id);
	printf("\n<2> - frame info:\n");
	printf(" ctrl_code\t=  0x%02X\n", stUnPack.ctrl_c);
	printf(" 0xFE_num\t=  %d\n", stUnPack.lead_num);
	printf(" data_len\t=  %d\n", stUnPack.data_len);
	printf(" frame_len\t=  %d\n", stUnPack.frame_len);
	//stUnPack.address[12] = 0;
	printf(" address\t=  %s\n", stUnPack.address);
	printf("\n<3> - data fields \n");
    if(stUnPack.flag == E_CARD_UNPD_FLG_OK)
	{
		printf("正确的回答:\n");
	}
    else if(stUnPack.flag == E_CARD_UNPD_FLG_ERROR_OK)
	{
		printf("异常的回答:\n");
	}
    printf("%s\n", g_out_data_2016);

	printf("\n<4> - remark:\n\n\t");
    if(stUnPack.ctrl_s.direct == E_CARD_CTRL_DIR_M2S)
	{
		printf("主站到从站的");
        if(E_CARD_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("读数据");
		}
        else if(E_CARD_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("写数据");
		}
		printf("请求\n\n");
	}
	else
	{		
		printf("从站到主站的");
        if(E_CARD_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对读数据请求的响应");
		}
        else if(E_CARD_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对写数据成功的响应");
		}
		printf("\n\n");
	}



	printf("\\------------------unpack result-------------------/\n");
}


/*********************************************************************************************
 函 数 名  : void pack_card_frame(unsigned long inRulerID,char *sendBuffer,int *sendlength)
 功能描述  : 组合cpu_card协议帧
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史	   :
  1.日	  期   : 2016年07月14日
	作	  者   : andy
	修改内容   : 新生成函数

**********************************************************************************************/

void pack_card_frame(unsigned long inRulerID,char *sendBuffer,int *sendlength)
{
	int num = 1;

	printf("0x%02X\n", inRulerID);
	
    S_CARD_PACK_FRAME pack_frame = {0};
    S_CARD_RULER_INFO info = {0};
	
	int length = 0;
	char buffer[256] = {0};
	
    int ret = get_card_ruler_info(inRulerID, &pack_frame, &info);
    if(ret != E_CARD_OK)
	{
		printf("不存在的刷卡命令\n\n");
		return ;
    }

	pack_frame.ruler_id = inRulerID;


	ret = pack_card_frame_by_data(&pack_frame, buffer, &length);
    if(ret != E_CARD_OK)
	{
		printf("\npack frame failed %d\n", ret);
		return;
	}
    memcpy(sendBuffer,buffer,length);//copy 到输出 outfile
	
	printf("\n%d - ruler infor:\n\n", num++);
	//show_ruler_info(inRulerID);

	
	//显示帧内容
	printf("\n%d - frame:\n\n\t", num++);
	show_packet(length, buffer);
	printf("\n\n-------------------------------------------------------------------\n");
	memcpy(sendBuffer,buffer,length);//copy 到输出 outfile
	*sendlength = length;
	
}

//字节流转换为十六进制字符串
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}

//十六进制字符串转换为字节流
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(source[i]);
        lowByte  = toupper(source[i + 1]);

        if (highByte > 0x39)
            highByte -= 0x37;
        else
            highByte -= 0x30;

        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;

        dest[i / 2] = (highByte << 4) | lowByte;
    }
    return ;
}


#endif
