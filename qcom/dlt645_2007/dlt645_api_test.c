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
#include "dlt645_api_07.h"

int g_num = 0;

void debug_switch(int show)
{
    g_need_pack_07 = show;
}
	
#define TITLE "DLT645_2007 TEST"
//#define FALSE 1
//#define TRUE 0
#define RS485_SEND "echo 1 > /sys/class/misc/io_ctl/gpio_state" 
#define RS485_RECEIVE "echo 0 > /sys/class/misc/io_ctl/gpio_state"

#define CTL_485_FLAGS_YES   1
#define CTL_485_FLAGS_NO    0
#define TEST_BUFF_SIZE 1024


//FILE *outfile;
//void print_usage();
//UINT8 outbuf[512] = {0};/* Recvice data buffer */

//UINT8 sendBuf[100]={0};

//int  fd = 0;
//static int  hex = 1;
//int ctl_485_flags;


void shownInformation(void)
{
        printf("*********************************************\n");
        printf("\t %s \t\n", TITLE);
        printf("*********************************************\n\n");
}

int show_all_valid_ruler_info()
{
	int rulerCount = 0;
	int i = 0;
	int ret = 0;
	unsigned long ulRulerID = 0;
	for(i = 0; i < 0x06100000; i++)
	{
		ulRulerID = i;
		ret = show_ruler_info(ulRulerID);	
		if(ret == E_D07_OK)
		{	
			
			rulerCount++;
			printf("%d:\n",rulerCount);
		}
	}

	return rulerCount;
}

void unpack_d07_frame_test(int argc, char*argv[])
{
	printf("argc = %d \n",argc);
	
	int i,j = 0;
	int ret = 0;
	char buf[512] = {0};
	int len = 0;
    S_D07_UNPACK stUnPack = {0};

	//printf("buffer = ");
	for(i = 2; i < argc; i++)
	{
		printf("argv %s  ", argv[i]);
		buf[j++] = strtol(argv[i], NULL,16);		
	}

	len = j;
	
	#if 1
	for(j=0;j<len;j++){
		printf("111buf[%d]==0x%x\n", j,buf[j]);
	}
	#endif
	
    debug_switch(D07_ON);
    ret = unpack_d07_frame(buf, len, &stUnPack);
    debug_switch(D07_OFF);
	if(ret != E_D07_OK)
	{
		printf("\n  解析出错! (error = %d : ",ret);
		switch(ret)
		{
			case E_D07_ERRO_FRAME_UNCOMP:
				printf("不完整的帧数据)\n");
				break;

			case E_D07_ERRO_FRAME_0x68:
				printf("起始符 0x68 的位置不对)\n");
				break;
				
			case E_D07_ERRO_FRAME_CHECK_SUM:
				printf("测试帧检验和不对)\n");
				break;
				
			case E_D07_ERRO_FRAME_END_0x16:
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
	if(stUnPack.flag == E_D07_UNPD_FLG_OK)
	{
		printf("正确的回答:\n");
	}
	else if(stUnPack.flag == E_D07_UNPD_FLG_ERROR_OK)
	{
		printf("异常的回答:\n");
	}
    printf("%s\n", g_out_data_07);

	printf("\n<4> - remark:\n\n\t");
	if(stUnPack.ctrl_s.direct == E_D07_CTRL_DIR_M2S)
	{
		printf("主站到从站的");
		if(E_D07_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("读数据");
		}
		else if(E_D07_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("写数据");
		}
		printf("请求\n\n");
	}
	else
	{		
		printf("从站到主站的");
		if(E_D07_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对读数据请求的响应");
		}
		else if(E_D07_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对写数据成功的响应");
		}
		printf("\n\n");
	}



	printf("\\------------------unpack result-------------------/\n");
}

/*****************************************************************************
 函 数 名  : void unpack_d07_frame_new(int outlength, char *outBuffer)
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
void unpack_d07_frame_new(int outlength, char *outBuffer)
{		
    int j = 0;
	int ret = 0;
	char buf[512] = {0};
	int len = outlength;
    S_D07_UNPACK stUnPack = {0};

	//printf("buffer = ");
	
	#if 1
	printf("len===%d\n",len);
	memset (buf, '\0', sizeof(buf));
	memcpy(buf,outBuffer,len);
	for(j=0;j<len;j++){
        printf("111buf[%d]==0x%02X\n", j,buf[j]&0xFF);
	}
	#endif
	
    debug_switch(D07_ON);
	ret = unpack_d07_frame(buf, len, &stUnPack);
    debug_switch(D07_OFF);
	if(ret != E_D07_OK)
	{
		printf("\n  解析出错! (error = %d : ",ret);
		switch(ret)
		{
			case E_D07_ERRO_FRAME_UNCOMP:
				printf("不完整的帧数据)\n");
				break;

			case E_D07_ERRO_FRAME_0x68:
				printf("起始符 0x68 的位置不对)\n");
				break;
				
			case E_D07_ERRO_FRAME_CHECK_SUM:
				printf("测试帧检验和不对)\n");
				break;
				
			case E_D07_ERRO_FRAME_END_0x16:
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
	if(stUnPack.flag == E_D07_UNPD_FLG_OK)
	{
		printf("正确的回答:\n");
	}
	else if(stUnPack.flag == E_D07_UNPD_FLG_ERROR_OK)
	{
		printf("异常的回答:\n");
	}
    printf("%s\n", g_out_data_07);

	printf("\n<4> - remark:\n\n\t");
	if(stUnPack.ctrl_s.direct == E_D07_CTRL_DIR_M2S)
	{
		printf("主站到从站的");
		if(E_D07_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("读数据");
		}
		else if(E_D07_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("写数据");
		}
		printf("请求\n\n");
	}
	else
	{		
		printf("从站到主站的");
		if(E_D07_CTRL_READ_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对读数据请求的响应");
		}
		else if(E_D07_CTRL_WRITE_DATA == stUnPack.ctrl_s.funcode)
		{
			printf("对写数据成功的响应");
		}
		printf("\n\n");
	}






	printf("\\------------------unpack result-------------------/\n");
}
void unpack_d07_frame_all(int outlength, char *outBuffer )
{
    int j = 0;
    int ret = 0;
    char buf[512] = {0};
    int len = outlength;
    //S_D07_UNPACK stUnPack = {0};

    //printf("buffer = ");

    #if 1
    printf("len===%d\n",len);
    memset (buf, '\0', sizeof(buf));
    memcpy(buf,outBuffer,len);
    for(j=0;j<len;j++){
        printf("111buf[%d]==0x%02X\n", j,buf[j]&0xFF);
    }
    #endif

    debug_switch(D07_ON);
    ret = unpack_d07_frame(buf, len, &my_stUnPack);
    debug_switch(D07_OFF);
    if(ret != E_D07_OK)
    {
        printf("\n  解析出错! (error = %d : ",ret);
        switch(ret)
        {
            case E_D07_ERRO_FRAME_UNCOMP:
                printf("不完整的帧数据)\n");
                break;

            case E_D07_ERRO_FRAME_0x68:
                printf("起始符 0x68 的位置不对)\n");
                break;

            case E_D07_ERRO_FRAME_CHECK_SUM:
                printf("测试帧检验和不对)\n");
                break;

            case E_D07_ERRO_FRAME_END_0x16:
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
    show_ruler_info_all(my_stUnPack.ruler_id);
    printf("\n<2> - frame info:\n");
    printf(" ctrl_code\t=  0x%02X\n", my_stUnPack.ctrl_c);
    printf(" 0xFE_num\t=  %d\n", my_stUnPack.lead_num);
    printf(" data_len\t=  %d\n", my_stUnPack.data_len);
    printf(" frame_len\t=  %d\n", my_stUnPack.frame_len);
    //my_stUnPack.address[12] = 0;
    printf(" address\t=  %s\n", my_stUnPack.address);
    printf("\n<3> - data fields \n");
    if(my_stUnPack.flag == E_D07_UNPD_FLG_OK)
    {
        printf("正确的回答:\n");
    }
    else if(my_stUnPack.flag == E_D07_UNPD_FLG_ERROR_OK)
    {
        printf("异常的回答:\n");
    }
    printf("%s\n", g_out_data_07);

    printf("\n<4> - remark:\n\n\t");
    if(my_stUnPack.ctrl_s.direct == E_D07_CTRL_DIR_M2S)
    {
        printf("主站到从站的");
        if(E_D07_CTRL_READ_DATA == my_stUnPack.ctrl_s.funcode)
        {
            printf("读数据");
        }
        else if(E_D07_CTRL_WRITE_DATA == my_stUnPack.ctrl_s.funcode)
        {
            printf("写数据");
        }
        printf("请求\n\n");
    }
    else
    {
        printf("从站到主站的");
        if(E_D07_CTRL_READ_DATA == my_stUnPack.ctrl_s.funcode)
        {
            printf("对读数据请求的响应");
        }
        else if(E_D07_CTRL_WRITE_DATA == my_stUnPack.ctrl_s.funcode)
        {
            printf("对写数据成功的响应");
        }
        printf("\n\n");
    }
    printf("\\------------------unpack result-------------------/\n");
}

void pack_d07_frame(unsigned long inRulerID)
{
	int num = 1;

    printf("0x%08X\n", inRulerID);
	S_D07_RULER_INFO info = {0};
	int ret = get_d07_ruler_info(inRulerID, &info);
	if(ret != E_D07_OK)	
	{
		printf("不存在的规规约类型\n\n");
		return ;
	}

	unsigned char ucCtrl = 0;
	S_D07_CTRL_CODE stCtrl = {0};
	int dir = 0;
	
	char addr[64] = {0}; // 地址
	S_D07_PACK_FRAME pack_frame = {0};
	int length = 0;
	char buffer[256] = {0};
	char data[256] = {0};
	char user[256] = {0};
	F_D07_RULER_TRANS func = NULL;
	printf("\n-------------------------------------------------------------------");
	printf("\n%d - 输入地址(小于等于12位):\n\n\taddress <<  ", num++);
	scanf("%s",addr);

	// 方向
	printf("\n%d - 传输方向:   主站到从站(0)  从站到主站(1)\n\n\tdirect << ", num++);
	scanf("%d", &dir);
	if(dir != 0 && dir != 1)
	{
		printf("\n输入错误\n");
		return;
	}

	stCtrl.direct = (E_D07_CTRL_DIR)dir;

	// 主站到从站
	if(E_D07_CTRL_DIR_M2S == dir)
	{
		if(info.rdwr == E_D07_RDWR_READ_ONLY)
		{
			stCtrl.funcode = E_D07_CTRL_READ_DATA;
			pack_frame.data_len = 4;
			pack_frame.data = NULL;
		}
		else if(info.rdwr == E_D07_RDWR_READ_WRITE)
		{
			//功能码
			printf("\n%d - 功能码:  读数据(0) 写数据(1)\n operate << ",num++);
			scanf("%d", &dir);
			if(dir == 0)
			{
				stCtrl.funcode = E_D07_CTRL_READ_DATA;
				pack_frame.data_len = 4;
				pack_frame.data = NULL;
			}
			else if(1 == dir)
			{
                debug_switch(D07_ON);
				stCtrl.funcode = E_D07_CTRL_WRITE_DATA;
				pack_frame.data_len = info.len + 4;
				func = info.func;
				func(E_D07_TRANS_U2F,user, data);
				pack_frame.data = data;
                debug_switch(D07_OFF);
			}
			else
			{
				printf("\n输入错误\n");
				return;
			}
		}
	}
	else	// 从站到主站
	{
		printf("\n%d - 数据域内容:\n\n",num++);
		// 正确的应答
		func = info.func;
        debug_switch(D07_ON);
		func(E_D07_TRANS_U2F, user, data);
        debug_switch(D07_OFF);
		stCtrl.funcode = E_D07_CTRL_READ_DATA;
		pack_frame.data = data;
		pack_frame.data_len = info.len + 4;
		// 异常的应答
	}

	ret = trans_d07ctrl_struct2char(&ucCtrl, &stCtrl); //将结构封装成字节
	
	if(ret != E_D07_OK)
	{
		printf("\ntrans_d07ctrl_struct2char failed %d\n", ret);
		return;
	}
	
	pack_frame.ruler_id = inRulerID;
	pack_frame.ctrl_code = ucCtrl;
    memcpy(pack_frame.address,addr, 13 );

	ret = pack_d07_frame_by_data(&pack_frame, buffer, &length);
	if(ret != E_D07_OK)
	{
		printf("\npack frame failed %d\n", ret);
		return;
	}

	printf("\n%d - ruler infor:\n\n", num++);
	show_ruler_info(inRulerID);
	printf(" ctrl\t=  0x%02X\n", ucCtrl);
	printf(" addr\t=  %s\n", addr);

	
	//显示帧内容
	printf("\n%d - frame:\n\n\t", num++);
	show_packet(length, buffer);
	printf("\n\n-------------------------------------------------------------------\n");
	
}

/*********************************************************************************************
 函 数 名  : void pack_d07_frame_new(unsigned long inRulerID,char *sendBuffer,int *sendlength)
 功能描述  : 组合645协议帧
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
void pack_d07_frame_new(unsigned long inRulerID,char *sendBuffer,int *sendlength)
{
	int num = 1;

    printf("0x%08X\n", inRulerID);
	S_D07_RULER_INFO info = {0};
	int ret = get_d07_ruler_info(inRulerID, &info);
	if(ret != E_D07_OK)	
	{
		printf("不存在的规规约类型\n\n");
		return ;
	}

	unsigned char ucCtrl = 0;
	S_D07_CTRL_CODE stCtrl = {0};
	int dir = 0;	
	char addr[64] = {0}; // 地址
	S_D07_PACK_FRAME pack_frame = {0};
	int length = 0;
	char buffer[256] = {0};
	char data[256] = {0};
	char user[256] = {0};
    F_D07_RULER_TRANS func = NULL;
	printf("\n-------------------------------------------------------------------");
	printf("\n%d - 输入地址(小于等于12位):\n\n\taddress <<  ", num++);
    scanf("%s",addr);

	// 方向
	printf("\n%d - 传输方向:   主站到从站(0)  从站到主站(1)\n\n\tdirect << ", num++);
	scanf("%d", &dir);
	if(dir != 0 && dir != 1)
	{
		printf("\n输入错误\n");
		return;
	}

	stCtrl.direct = (E_D07_CTRL_DIR)dir;

	// 主站到从站
	if(E_D07_CTRL_DIR_M2S == dir)
	{
		if(info.rdwr == E_D07_RDWR_READ_ONLY)
		{
			stCtrl.funcode = E_D07_CTRL_READ_DATA;
			pack_frame.data_len = 4;
			pack_frame.data = NULL;
		}
		else if(info.rdwr == E_D07_RDWR_READ_WRITE)
		{
			//功能码
			printf("\n%d - 功能码:  读数据(0) 写数据(1)\n operate << ",num++);
			scanf("%d", &dir);
			if(dir == 0)
			{
				stCtrl.funcode = E_D07_CTRL_READ_DATA;
				pack_frame.data_len = 4;
				pack_frame.data = NULL;
			}
			else if(1 == dir)
			{
                debug_switch(D07_ON);
				stCtrl.funcode = E_D07_CTRL_WRITE_DATA;
				pack_frame.data_len = info.len + 4;
				func = info.func;
				func(E_D07_TRANS_U2F,user, data);
				pack_frame.data = data;
                debug_switch(D07_OFF);
			}
			else
			{
				printf("\n输入错误\n");
				return;
			}
		}
	}
	else	// 从站到主站
	{
		printf("\n%d - 数据域内容:\n\n",num++);
		// 正确的应答
		func = info.func;
        debug_switch(D07_ON);
		func(E_D07_TRANS_U2F, user, data);
        debug_switch(D07_OFF);
		stCtrl.funcode = E_D07_CTRL_READ_DATA;
		pack_frame.data = data;
		pack_frame.data_len = info.len + 4;
		// 异常的应答
	}

	ret = trans_d07ctrl_struct2char(&ucCtrl, &stCtrl); //将结构封装成字节
	
	if(ret != E_D07_OK)
	{
		printf("\ntrans_d07ctrl_struct2char failed %d\n", ret);
		return;
	}
	
	pack_frame.ruler_id = inRulerID;
	pack_frame.ctrl_code = ucCtrl;
    memcpy(pack_frame.address,addr, 13 );

	ret = pack_d07_frame_by_data(&pack_frame, buffer, &length);
	if(ret != E_D07_OK)
	{
		printf("\npack frame failed %d\n", ret);
		return;
	}
	//memcpy(sendBuffer,buffer,length);//copy 到输出 outfile
	
	printf("\n%d - ruler infor:\n\n", num++);
	show_ruler_info(inRulerID);
	printf(" ctrl\t=  0x%02X\n", ucCtrl);
	printf(" addr\t=  %s\n", addr);

	
	//显示帧内容
	printf("\n%d - frame:\n\n\t", num++);
	show_packet(length, buffer);
	printf("\n\n-------------------------------------------------------------------\n");
	memcpy(sendBuffer,buffer,length);//copy 到输出 outfile
	*sendlength = length;
	
}

/*********************************************************************************************
 函 数 名  : void pack_d07_frame_all(unsigned long inRulerID,char *sendBuffer,int *sendlength,char *addr,int dir)
 功能描述  : 组合645协议帧
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史	   :
  1.日	  期   : 2016年08月29日
    作	  者   : andy
    修改内容   : 新生成函数

**********************************************************************************************/
void pack_d07_frame_all(unsigned long inRulerID,char *sendBuffer,int *sendlength,char *addr,int dir)
{
    int num = 1;

    printf("0x%08X\n", inRulerID);
    S_D07_RULER_INFO info = {0};
    int ret = get_d07_ruler_info(inRulerID, &info);
    if(ret != E_D07_OK)
    {
        printf("不存在的规规约类型\n\n");
        return ;
    }

    unsigned char ucCtrl = 0;
    S_D07_CTRL_CODE stCtrl = {0};
    //int dir = 0;
    //char addr[64] = {0}; // 地址
    S_D07_PACK_FRAME pack_frame = {0};
    int length = 0;
    char buffer[256] = {0};
    char data[256] = {0};
    char user[256] = {0};
    F_D07_RULER_TRANS func = NULL;
#if 0
    printf("\n-------------------------------------------------------------------");
    printf("\n%d - 输入地址(小于等于12位):\n\n\taddress <<  ", num++);
    scanf("%s",addr);

    // 方向
    printf("\n%d - 传输方向:   主站到从站(0)  从站到主站(1)\n\n\tdirect << ", num++);
    scanf("%d", &dir);
    if(dir != 0 && dir != 1)
    {
        printf("\n输入错误\n");
        return;
    }
#else

#endif
    stCtrl.direct = (E_D07_CTRL_DIR)dir;

    // 主站到从站
    if(E_D07_CTRL_DIR_M2S == dir)
    {
        if(info.rdwr == E_D07_RDWR_READ_ONLY)
        {
            stCtrl.funcode = E_D07_CTRL_READ_DATA;
            pack_frame.data_len = 4;
            pack_frame.data = NULL;
        }
        else if(info.rdwr == E_D07_RDWR_READ_WRITE)
        {
            //功能码
            printf("\n%d - 功能码:  读数据(0) 写数据(1)\n operate << ",num++);
         #if 0
            scanf("%d", &dir);
         #endif
            if(dir == 0)
            {
                stCtrl.funcode = E_D07_CTRL_READ_DATA;
                pack_frame.data_len = 4;
                pack_frame.data = NULL;
            }
            else if(1 == dir)
            {
                debug_switch(D07_ON);
                stCtrl.funcode = E_D07_CTRL_WRITE_DATA;
                pack_frame.data_len = info.len + 4;
                func = info.func;
                func(E_D07_TRANS_U2F,user, data);
                pack_frame.data = data;
                debug_switch(D07_OFF);
            }
            else
            {
                printf("\n输入错误\n");
                return;
            }
        }
    }
    else	// 从站到主站
    {
        printf("\n%d - 数据域内容:\n\n",num++);
        // 正确的应答
        func = info.func;
        debug_switch(D07_ON);
        func(E_D07_TRANS_U2F, user, data);
        debug_switch(D07_OFF);
        stCtrl.funcode = E_D07_CTRL_READ_DATA;
        pack_frame.data = data;
        pack_frame.data_len = info.len + 4;
        // 异常的应答
    }

    ret = trans_d07ctrl_struct2char(&ucCtrl, &stCtrl); //将结构封装成字节

    if(ret != E_D07_OK)
    {
        printf("\ntrans_d07ctrl_struct2char failed %d\n", ret);
        return;
    }

    pack_frame.ruler_id = inRulerID;
    pack_frame.ctrl_code = ucCtrl;
    memcpy(pack_frame.address,addr, 13 );

    ret = pack_d07_frame_by_data(&pack_frame, buffer, &length);
    if(ret != E_D07_OK)
    {
        printf("\npack frame failed %d\n", ret);
        return;
    }
    //memcpy(sendBuffer,buffer,length);//copy 到输出 outfile

    printf("\n%d - ruler infor:\n\n", num++);
    show_ruler_info(inRulerID);
    printf(" ctrl\t=  0x%02X\n", ucCtrl);
    printf(" addr\t=  %s\n", addr);


    //显示帧内容
    printf("\n%d - frame:\n\n\t", num++);
    show_packet(length, buffer);
    printf("\n\n-------------------------------------------------------------------\n");
    memcpy(sendBuffer,buffer,length);//copy 到输出 outfile
    *sendlength = length;

}


void test_get_ruler_info()
{
    show_ruler_info(0);
    unsigned long ulRulerID = 0;

    for(ulRulerID = 0; ulRulerID <= 0x00003F00; ulRulerID++)
    {
        show_ruler_info(ulRulerID);
    }

}

typedef struct
{
    F_D07_RULER_TRANS func;
    char * funcName;

}funcInfo;

int show_ruler_info(unsigned long inRulerID)
{
    unsigned char ucDi3 = 0;
    unsigned char ucDi2 = 0;
    unsigned char ucDi1 = 0;
    unsigned char ucDi0 = 0;

    ucDi0 = inRulerID & 0xFF;
    ucDi1 = (inRulerID >> 8) & 0xFF;
    ucDi2 = (inRulerID >> 16) & 0xFF;
    ucDi3 = (inRulerID >> 24) & 0xFF;
    S_D07_RULER_INFO info = {0};

    char* formats[] = { "E_D07_FMT_UNKOWN",
                        "E_D07_FMT_XXXX",
                        "E_D07_FMT_XXXXXX",
                        "E_D07_FMT_XX_4",
                        "E_D07_FMT_XX_6",
                        "E_D07_FMT_XX_8",
                        "E_D07_FMT_XX_10",
                        "E_D07_FMT_XX_16",
                        "E_D07_FMT_X_XXX",
                        "E_D07_FMT_XX_XX",
                        "E_D07_FMT_XX_XXXX",
                        "E_D07_FMT_XX_XXXX_8",
                        "E_D07_FMT_XXX_X",
                        "E_D07_FMT_XXX_XXX",
                        "E_D07_FMT_XXXXXX_XX",
                        "E_D07_FMT_XXXXXXXX",
                        "E_D07_FMT_XXXXXX_2",
                        "E_D07_FMT_XXXXXX_6",
                        "E_D07_FMT_NN",
                        "E_D07_FMT_NNNN",
                        "E_D07_FMT_NN_4",
                        "E_D07_FMT_NN_6",
                        "E_D07_FMT_NN_32",
                        "E_D07_FMT_N_NNN",
                        "E_D07_FMT_NN_NNNN",
                        "E_D07_FMT_NNN_N",
                        "E_D07_FMT_hhmmss",
                        "E_D07_FMT_MMDDhhmm",
                        "E_D07_FMT_MMDDNN_14",
                        "E_D07_FMT_hhmmNN_14",
                        "E_D07_FMT_DDhh",
                        "E_D07_FMT_YYMMDDWW",
                        "E_D07_FMT_YYMMDDNN",
                        "E_D07_FMT_YYMMDDhhmm",
                        "E_D07_FMT_YYMMDDhhmmNN",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmmss",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmmss_2",
                        "E_D07_FMT_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmm",
                        "E_D07_FMT_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss",
                        "E_D07_FMT_RECD_LOSS_LESS_VOLT",
                        "E_D07_FMT_RECD_ANTI_PHASE",
                        "E_D07_FMT_RECD_UN_BALANCE",
                        "E_D07_FMT_RECD_LESS_MORE_LOSS_ELEC",
                        "E_D07_FMT_RECD_VOLT_PASS_PERCENT",
                        "E_D07_FMT_RECD_METER_PROG",
                        "E_D07_FMT_RECD_METER_CLEAR",
                        "E_D07_FMT_RECD_DEAMD_CLEAR",
                        "E_D07_FMT_RECD_EVENT_CLEAR",
                        "E_D07_FMT_RECD_ADJUST_TIME",
                        "E_D07_FMT_RECD_PART_PROG",
                        "E_D07_FMT_RECD_ZONE_PROG",
                        "E_D07_FMT_RECD_WEEKDAY_PROG",
                        "E_D07_FMT_RECD_HOLIDAY_PROG",
                        "E_D07_FMT_RECD_POWER_PROG",
                        "E_D07_FMT_RECD_PAYOFF_PROG",
                        "E_D07_FMT_RECD_OPEN_METER_CAP",
                        "E_D07_FMT_RECD_OPEN_BTN_BOX"
                        };

    funcInfo funcs[] = {{NULL, "unkown"},
                        {trans_d07_data_XXXX ,"trans_d07_data_XXXX"},
                        {trans_d07_data_XXXXXX ,"trans_d07_data_XXXXXX"},
                        {trans_d07_data_XX_4,"trans_d07_data_XX_4"},
                        {trans_d07_data_XX_6 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_XX_8,"trans_d07_data_XX_4"},
                        {trans_d07_data_XX_10 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_XX_16 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_X_XXX ,"trans_d07_data_X_XXX"},
                        {trans_d07_data_XX_XX ,"trans_d07_data_XX_XX"},
                        {trans_d07_data_XX_XXXX,"trans_d07_data_XX_XXXX"},
                        {trans_d07_data_XX_XXXX_8,"trans_d07_data_XX_XXXX_8"},
                        {trans_d07_data_XXX_X ,"trans_d07_data_XXX_X"},
                        {trans_d07_data_XXX_XXX ,"trans_d07_data_XXX_XXX"},
                        {trans_d07_data_XXXXXX_XX ,"trans_d07_data_XXXXXX_XX"},
                        {trans_d07_data_XXXXXXXX ,"trans_d07_data_XXXXXXXX"},
                        {trans_d07_data_XXXXXX_2 ,"trans_d07_data_XXXXXX_2"},
                        {trans_d07_data_XXXXXX_6 ,"trans_d07_data_XXXXXX_6"},
                        {trans_d07_data_NN ,"trans_d07_data_NN"},
                        {trans_d07_data_NNNN ,"trans_d07_data_NNNN"},
                        {trans_d07_data_NN_4,"trans_d07_data_NN_4"},
                        {trans_d07_data_NN_6,"trans_d07_data_NN_6"},
                        {trans_d07_data_NN_32,"trans_d07_data_NN_32"},
                        {trans_d07_data_N_NNN,"trans_d07_data_N_NNN"},
                        {trans_d07_data_NN_NNNN,"trans_d07_data_NN_NNNN"},
                        {trans_d07_data_NNN_N,"trans_d07_data_NNN_N"},
                        {trans_d07_data_hhmmss ,"trans_d07_data_hhmmss"},
                        {trans_d07_data_MMDDhhmm ,"trans_d07_data_MMDDhhmm"},
                        {trans_d07_data_MMDDNN_14,"trans_d07_data_MMDDNN_14"},
                        {trans_d07_data_hhmmNN_14,"trans_d07_data_hhmmNN_14"},
                        {trans_d07_data_DDhh ,"trans_d07_data_DDhh"},
                        {trans_d07_data_YYMMDDWW ,"trans_d07_data_YYMMDDWW"},
                        {trans_d07_data_YYMMDDNN ,"trans_d07_data_YYMMDDNN"},
                        {trans_d07_data_YYMMDDhhmm,"trans_d07_data_YYMMDDhhmm"},
                        {trans_d07_data_YYMMDDhhmmNN,"trans_d07_data_YYMMDDhhmmNN"},
                        {trans_d07_data_YYMMDDhhmmss,"trans_d07_data_YYMMDDhhmmss"},
                        {trans_d07_data_YYMMDDhhmmss_2,"trans_d07_data_YYMMDDhhmmss_2"},
                        {trans_d07_data_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm,"trans_d07_data_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm"},
                        {trans_d07_data_XX_XXXX_YYMMDDhhmm ,"trans_d07_data_XX_XXXX_YYMMDDhhmm"},
                        {trans_d07_data_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss ,"trans_d07_data_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss"},
                        {trans_d07_data_recd_loss_less_volt ,"trans_d07_data_recd_loss_less_volt"},
                        {trans_d07_data_recd_anti_phase ,"trans_d07_data_recd_anti_phase"},
                        {trans_d07_data_recd_un_balance ,"trans_d07_data_recd_un_balance"},
                        {trans_d07_data_recd_less_more_loss_elec ,"trans_d07_data_recd_less_more_loss_elec"},
                        {trans_d07_data_recd_volt_pass_percent ,"trans_d07_data_recd_volt_pass_percent"},
                        {trans_d07_data_recd_meter_prog ,"trans_d07_data_recd_meter_prog"},
                        {trans_d07_data_recd_meter_clear ,"trans_d07_data_recd_meter_clear"},
                        {trans_d07_data_recd_demand_clear ,"trans_d07_data_recd_demand_clear"},
                        {trans_d07_data_recd_event_clear ,"trans_d07_data_recd_event_clear"},
                        {trans_d07_data_recd_adjust_time ,"trans_d07_data_recd_adjust_time"},
                        {trans_d07_data_recd_part_prog ,"trans_d07_data_recd_part_prog"},
                        {trans_d07_data_recd_zone_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_weekday_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_holiday_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_power_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_payoff_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_open_meter_cap ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_open_btn_box ,"trans_d07_data_recd_zone_prog"}
                        };





    int ret = 0;
    ret = get_d07_ruler_info(inRulerID, &info);
    if(ret != E_D07_OK)
    {
        return E_D07_ERRO_UNKOWN_ID;
    }
    else
    {
        //printf("\n---------------------[%d]---------------------\n\n", ++g_num);
        printf(" id\t=  0x%08X :", inRulerID);
        printf(" [%02X][%02X][%02X][%02X]\n", ucDi3, ucDi2, ucDi1, ucDi0);

        if(E_D07_RULER_TYPE_UNKNOWN != info.type)
        {
            printf(" type\t=  %d\n", info.type);

            if(info.para.payoff != E_D07_PAYOFF_NULL)
            {
                printf(" payoff\t=  %d\n", info.para.payoff);
            }

            if(info.para.rate != E_D07_RATE_NULL)
            {
                printf(" rate\t=  %d\n", info.para.rate);
            }

            // data length
            printf(" length\t=  %d\n", info.len);

            // read or write
            if(info.rdwr == E_D07_RDWR_READ_ONLY)
            {
                printf(" rdwr\t=  read only\n");
            }
            else if(info.rdwr == E_D07_RDWR_READ_WRITE)
            {
                printf(" rdwr\t=  read write\n");
            }
            else
            {
                printf(" rdwr\t=  write only\n");
            }

            //format
            printf(" format\t=  %s\n", formats[info.format]);


            printf(" func\t=  ");

            if(info.func == funcs[info.format].func)
            printf("%s\n", funcs[info.format].funcName);

            printf(" name\t=  %s\n\n", info.name);

        }
    }

    return E_D07_OK;
}

int show_ruler_info_all(unsigned long inRulerID)
{
    unsigned char ucDi3 = 0;
    unsigned char ucDi2 = 0;
    unsigned char ucDi1 = 0;
    unsigned char ucDi0 = 0;

    ucDi0 = inRulerID & 0xFF;
    ucDi1 = (inRulerID >> 8) & 0xFF;
    ucDi2 = (inRulerID >> 16) & 0xFF;
    ucDi3 = (inRulerID >> 24) & 0xFF;
    //S_D07_RULER_INFO info = {0};

    char* formats[] = { "E_D07_FMT_UNKOWN",
                        "E_D07_FMT_XXXX",
                        "E_D07_FMT_XXXXXX",
                        "E_D07_FMT_XX_4",
                        "E_D07_FMT_XX_6",
                        "E_D07_FMT_XX_8",
                        "E_D07_FMT_XX_10",
                        "E_D07_FMT_XX_16",
                        "E_D07_FMT_X_XXX",
                        "E_D07_FMT_XX_XX",
                        "E_D07_FMT_XX_XXXX",
                        "E_D07_FMT_XX_XXXX_8",
                        "E_D07_FMT_XXX_X",
                        "E_D07_FMT_XXX_XXX",
                        "E_D07_FMT_XXXXXX_XX",
                        "E_D07_FMT_XXXXXXXX",
                        "E_D07_FMT_XXXXXX_2",
                        "E_D07_FMT_XXXXXX_6",
                        "E_D07_FMT_NN",
                        "E_D07_FMT_NNNN",
                        "E_D07_FMT_NN_4",
                        "E_D07_FMT_NN_6",
                        "E_D07_FMT_NN_32",
                        "E_D07_FMT_N_NNN",
                        "E_D07_FMT_NN_NNNN",
                        "E_D07_FMT_NNN_N",
                        "E_D07_FMT_hhmmss",
                        "E_D07_FMT_MMDDhhmm",
                        "E_D07_FMT_MMDDNN_14",
                        "E_D07_FMT_hhmmNN_14",
                        "E_D07_FMT_DDhh",
                        "E_D07_FMT_YYMMDDWW",
                        "E_D07_FMT_YYMMDDNN",
                        "E_D07_FMT_YYMMDDhhmm",
                        "E_D07_FMT_YYMMDDhhmmNN",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmmss",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmmss_2",
                        "E_D07_FMT_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm",
                        "E_D07_FMT_XX_XXXX_YYMMDDhhmm",
                        "E_D07_FMT_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss",
                        "E_D07_FMT_RECD_LOSS_LESS_VOLT",
                        "E_D07_FMT_RECD_ANTI_PHASE",
                        "E_D07_FMT_RECD_UN_BALANCE",
                        "E_D07_FMT_RECD_LESS_MORE_LOSS_ELEC",
                        "E_D07_FMT_RECD_VOLT_PASS_PERCENT",
                        "E_D07_FMT_RECD_METER_PROG",
                        "E_D07_FMT_RECD_METER_CLEAR",
                        "E_D07_FMT_RECD_DEAMD_CLEAR",
                        "E_D07_FMT_RECD_EVENT_CLEAR",
                        "E_D07_FMT_RECD_ADJUST_TIME",
                        "E_D07_FMT_RECD_PART_PROG",
                        "E_D07_FMT_RECD_ZONE_PROG",
                        "E_D07_FMT_RECD_WEEKDAY_PROG",
                        "E_D07_FMT_RECD_HOLIDAY_PROG",
                        "E_D07_FMT_RECD_POWER_PROG",
                        "E_D07_FMT_RECD_PAYOFF_PROG",
                        "E_D07_FMT_RECD_OPEN_METER_CAP",
                        "E_D07_FMT_RECD_OPEN_BTN_BOX"
                        };

    funcInfo funcs[] = {{NULL, "unkown"},
                        {trans_d07_data_XXXX ,"trans_d07_data_XXXX"},
                        {trans_d07_data_XXXXXX ,"trans_d07_data_XXXXXX"},
                        {trans_d07_data_XX_4,"trans_d07_data_XX_4"},
                        {trans_d07_data_XX_6 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_XX_8,"trans_d07_data_XX_4"},
                        {trans_d07_data_XX_10 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_XX_16 ,"trans_d07_data_XX_6"},
                        {trans_d07_data_X_XXX ,"trans_d07_data_X_XXX"},
                        {trans_d07_data_XX_XX ,"trans_d07_data_XX_XX"},
                        {trans_d07_data_XX_XXXX,"trans_d07_data_XX_XXXX"},
                        {trans_d07_data_XX_XXXX_8,"trans_d07_data_XX_XXXX_8"},
                        {trans_d07_data_XXX_X ,"trans_d07_data_XXX_X"},
                        {trans_d07_data_XXX_XXX ,"trans_d07_data_XXX_XXX"},
                        {trans_d07_data_XXXXXX_XX ,"trans_d07_data_XXXXXX_XX"},
                        {trans_d07_data_XXXXXXXX ,"trans_d07_data_XXXXXXXX"},
                        {trans_d07_data_XXXXXX_2 ,"trans_d07_data_XXXXXX_2"},
                        {trans_d07_data_XXXXXX_6 ,"trans_d07_data_XXXXXX_6"},
                        {trans_d07_data_NN ,"trans_d07_data_NN"},
                        {trans_d07_data_NNNN ,"trans_d07_data_NNNN"},
                        {trans_d07_data_NN_4,"trans_d07_data_NN_4"},
                        {trans_d07_data_NN_6,"trans_d07_data_NN_6"},
                        {trans_d07_data_NN_32,"trans_d07_data_NN_32"},
                        {trans_d07_data_N_NNN,"trans_d07_data_N_NNN"},
                        {trans_d07_data_NN_NNNN,"trans_d07_data_NN_NNNN"},
                        {trans_d07_data_NNN_N,"trans_d07_data_NNN_N"},
                        {trans_d07_data_hhmmss ,"trans_d07_data_hhmmss"},
                        {trans_d07_data_MMDDhhmm ,"trans_d07_data_MMDDhhmm"},
                        {trans_d07_data_MMDDNN_14,"trans_d07_data_MMDDNN_14"},
                        {trans_d07_data_hhmmNN_14,"trans_d07_data_hhmmNN_14"},
                        {trans_d07_data_DDhh ,"trans_d07_data_DDhh"},
                        {trans_d07_data_YYMMDDWW ,"trans_d07_data_YYMMDDWW"},
                        {trans_d07_data_YYMMDDNN ,"trans_d07_data_YYMMDDNN"},
                        {trans_d07_data_YYMMDDhhmm,"trans_d07_data_YYMMDDhhmm"},
                        {trans_d07_data_YYMMDDhhmmNN,"trans_d07_data_YYMMDDhhmmNN"},
                        {trans_d07_data_YYMMDDhhmmss,"trans_d07_data_YYMMDDhhmmss"},
                        {trans_d07_data_YYMMDDhhmmss_2,"trans_d07_data_YYMMDDhhmmss_2"},
                        {trans_d07_data_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm,"trans_d07_data_YYMMDDhhmmss_2_XX_XXXX_YYMMDDhhmm"},
                        {trans_d07_data_XX_XXXX_YYMMDDhhmm ,"trans_d07_data_XX_XXXX_YYMMDDhhmm"},
                        {trans_d07_data_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss ,"trans_d07_data_YYMMDDhhmmss_XXX_XXX_YYMMDDhhmmss"},
                        {trans_d07_data_recd_loss_less_volt ,"trans_d07_data_recd_loss_less_volt"},
                        {trans_d07_data_recd_anti_phase ,"trans_d07_data_recd_anti_phase"},
                        {trans_d07_data_recd_un_balance ,"trans_d07_data_recd_un_balance"},
                        {trans_d07_data_recd_less_more_loss_elec ,"trans_d07_data_recd_less_more_loss_elec"},
                        {trans_d07_data_recd_volt_pass_percent ,"trans_d07_data_recd_volt_pass_percent"},
                        {trans_d07_data_recd_meter_prog ,"trans_d07_data_recd_meter_prog"},
                        {trans_d07_data_recd_meter_clear ,"trans_d07_data_recd_meter_clear"},
                        {trans_d07_data_recd_demand_clear ,"trans_d07_data_recd_demand_clear"},
                        {trans_d07_data_recd_event_clear ,"trans_d07_data_recd_event_clear"},
                        {trans_d07_data_recd_adjust_time ,"trans_d07_data_recd_adjust_time"},
                        {trans_d07_data_recd_part_prog ,"trans_d07_data_recd_part_prog"},
                        {trans_d07_data_recd_zone_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_weekday_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_holiday_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_power_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_payoff_prog ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_open_meter_cap ,"trans_d07_data_recd_zone_prog"},
                        {trans_d07_data_recd_open_btn_box ,"trans_d07_data_recd_zone_prog"}
                        };





    int ret = 0;
    ret = get_d07_ruler_info(inRulerID, &my_info);
    if(ret != E_D07_OK)
    {
        return E_D07_ERRO_UNKOWN_ID;
    }
    else
    {
        //printf("\n---------------------[%d]---------------------\n\n", ++g_num);
        printf(" id\t=  0x%08X :", inRulerID);
        printf(" [%02X][%02X][%02X][%02X]\n", ucDi3, ucDi2, ucDi1, ucDi0);

        if(E_D07_RULER_TYPE_UNKNOWN != my_info.type)
        {
            printf(" type\t=  %d\n", my_info.type);

            if(my_info.para.payoff != E_D07_PAYOFF_NULL)
            {
                printf(" payoff\t=  %d\n", my_info.para.payoff);
            }

            if(my_info.para.rate != E_D07_RATE_NULL)
            {
                printf(" rate\t=  %d\n", my_info.para.rate);
            }

            // data length
            printf(" length\t=  %d\n", my_info.len);

            // read or write
            if(my_info.rdwr == E_D07_RDWR_READ_ONLY)
            {
                printf(" rdwr\t=  read only\n");
            }
            else if(my_info.rdwr == E_D07_RDWR_READ_WRITE)
            {
                printf(" rdwr\t=  read write\n");
            }
            else
            {
                printf(" rdwr\t=  write only\n");
            }

            //format
            printf(" format\t=  %s\n", formats[my_info.format]);


            printf(" func\t=  ");

            if(my_info.func == funcs[my_info.format].func)
            printf("%s\n", funcs[my_info.format].funcName);

            printf(" name\t=  %s\n\n", my_info.name);

        }
    }

    return E_D07_OK;
}


void show_packet(int buflen, char* buf)
{	
    
	int j = buflen;
	int i;
	//printf("packet length :%d\n packet content : ",j);
	for(i = 0; i < j;i++)
	{
		printf("%02X ", (unsigned char)buf[i]);
	} 
}

void test_all_ruler_d07_meter()
{


}
