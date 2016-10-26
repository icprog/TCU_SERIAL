#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card_api.h"

// 数据域长度
#define CARD_DATA_LEN_METER_PROG         50
#define CARD_DATA_LEN_RECD_METER_CLEAR   106
#define CARD_DATA_LEN_RECD_DEAMD_CLEAR     202




/*************************************************
Function:       pack_card_frame_by_data
Description:    内部函数，仅完成结构体的填充

Author:         liming 

Calls:          
Called By:      
Input:          S_CARD_PACK_FRAME *inPara 用于封帧数据
                
Output:         char *outBuffer 封装后的帧buffer
                INT32  *outLength 该帧总长
Return:         正确返回0

Others:         
  
*************************************************/
INT32 pack_card_frame_by_data(S_CARD_PACK_FRAME *inPara, char *outBuffer, INT32 *outLength)
{
    INT32 i;
    INT32 len = 0; /* 计算包的总字节长 */
    UINT8 ucCheckSum = 0;
    UINT8 ucDi0,ucDi1;
    
    if(NULL == outBuffer || NULL == inPara ||  NULL == outLength)
    {
        return E_CARD_ERRO_NULL;
    }
    // 准备数据 
    ucDi0 = inPara->ruler_id & 0xFF;
    ucDi1 = (inPara->ruler_id >> 8) & 0xFF;
     

    /* 开始封帧 */
    // 1 帧起始符
    outBuffer[len++] = 0x02; 

	// 2 数据域字节数
	outBuffer[len++] = 0x00;
    outBuffer[len++] = inPara->data_len;

	// 3 命令字        
	outBuffer[len++] = ucDi1;

	// 4 命令参数    
    outBuffer[len++] = ucDi0;

    // 5 数据包
    for(i = 0; i < inPara->data_len - 2; i++)
    {
       outBuffer[len++] = inPara->data[i];
    }
	
    printf("len = %d\n", inPara->data_len);

      
    // 6 数据各字节异或值
    ucCheckSum = ucDi0^ucDi1;
	if(inPara->data_len > 2)
	{
	    for(i = 0; i < inPara->data_len-2; i++)//change by andy i = 0
	    {
	        ucCheckSum = ucCheckSum^inPara->data[i];
	    }
	}
	printf("ucCheckSum==%02x\n",ucCheckSum);
	outBuffer[len++] = ucCheckSum;
	
    // 7 结束符
    outBuffer[len++] = 0x03;

    /* 输出参数 */
    *outLength = len;

    return E_CARD_OK;
}



/*************************************************
Function:       unpack_card_frame
Description:    解析card 2016帧功能函数

Author:         liming 

Calls:          
Called By:      
Input:          inpBuffer      传入包有帧buffer指针
                inBufferLength 该buffer长度
                
Output:         outpFrame 该帧的各类详细信息

Return:         正确返回0

Others:         重要的功能函数
  
*************************************************/
INT32 unpack_card_frame(void *inpBuffer, INT32 inBufferLength, S_CARD_UNPACK *outpFrame)
{
#if 0
    INT32 nNumber0xFE = 0;                     /* 前导字符的个数 */
    INT32 i = 0;
    INT32 pos = 0;
    INT32 nCheckSumPosStart, nCheckSumPos, nEndPos;
    INT32 ret = 0;
    UINT8 ucCtrlCode = 0;
    UINT8 ucDataLen = 0;
    UINT8 ucCheckSum = 0;
    UINT32 ulRulerID = 0;
    
    UINT8 *buffer = (UINT8 *)inpBuffer;

    char addr_bcd[6] = {0};                  //地址信息(BCD形式)
    char addr_str[CARD_ADDR_LEN+1] = {0};         //地址信息(string形式)
    UINT8 aucDataTmp[CARD_DATA_MAX];
    S_CARD_RULER_INFO stRulerInfor = {0};
    F_CARD_RULER_TRANS pTransFunc;    /* 解析函数指针 */

    if(NULL == inpBuffer || NULL == outpFrame)
    {
        return E_CARD_ERRO_NULL;
    }

    if(inBufferLength < CARD_FRAME_LEN_MIN)
    {
        return E_CARD_ERRO_FRAME_UNCOMP;
    }

    for(i = 0; i < 4; i++)
    {
        if(0xFE == buffer[i])
        {
            pos++;
            nNumber0xFE++;
        }
    }
    
    nCheckSumPosStart = pos;

    // 检查前导字符 0x68
    if(0x68 != buffer[pos] || 0x68 != buffer[pos+7])
    {
        return E_CARD_ERRO_FRAME_0x68;
    }

    //地址
    pos++;
    for(i = 0; i < 6; i++)
    {
        addr_bcd[i] = buffer[pos++];
    }

    pos++;

    //地址
    card_bcd2str(addr_bcd, addr_str, 6);
    //控制码
    ucCtrlCode = buffer[pos++];
    //printf("\nCtrlCode = 0x%02X\n", ucCtrlCode);
    //转换控制码
    ret =  trans_cardctrl_char2struct(ucCtrlCode, &(outpFrame->ctrl_s));
    if(E_CARD_OK != ret)
    {
        return ret;
    }

    //数据域长度
    ucDataLen = buffer[pos++];
    nCheckSumPos = pos + ucDataLen;
    nEndPos = nCheckSumPos+1;

    for(i = 0; i < ucDataLen; i++)
    {
        aucDataTmp[i] = (buffer[pos++] - 0x33); 
    }
    //查检checksum
    for(i = nCheckSumPosStart; i < nCheckSumPos; i++)
    {
        ucCheckSum +=  buffer[i];
    }


    if(ucCheckSum != buffer[nCheckSumPos])
    {
    
        return E_CARD_ERRO_FRAME_CHECK_SUM;
    }

    //结束符
    if(0x16 != buffer[nEndPos])
    {
        return E_CARD_ERRO_FRAME_END_0x16;
    }


    //地址
    for(i = 0; i < CARD_ADDR_LEN; i++)
    {
        outpFrame->address[i] = addr_str[i];
        //printf("%d - %c \n",i, addr_str[i]);
    }

    // 正常的报文
    
    if(E_CARD_CTRL_SR_OK == outpFrame->ctrl_s.reply)
    {    
        // 原数据 
        for(i = 4; i < ucDataLen; i++)
        {
            outpFrame->data_pack[i-4] = aucDataTmp[i];
        }

        // 规约ID
        ulRulerID = ((aucDataTmp[0] & 0xFF) | 
                    ((aucDataTmp[1] << 8) & 0xFF00) |
                    ((aucDataTmp[2] << 16) & 0xFF0000) |
                    ((aucDataTmp[3] << 24) & 0xFF000000));

        // 通过ID获取详细信息    
        //ret = get_CARD_ruler_info(ulRulerID, &stRulerInfor);

        if(E_CARD_OK != ret)
        {
            return ret;
        }
        
        if(NULL == stRulerInfor.func)
        {
            outpFrame->flag    = E_CARD_UNPD_FLG_FUNC_NULL;
        }
        else
        {

            if(ucDataLen > 4)
            {
                pTransFunc          = stRulerInfor.func;
                ret = pTransFunc(E_CARD_TRANS_F2U,
                                (void*)&(outpFrame->data_unpack),
                                (void*)&(outpFrame->data_pack));
              
                if(E_CARD_OK == ret)
                {
                    outpFrame->flag    = E_CARD_UNPD_FLG_OK;
                }
                else
                {
                    outpFrame->flag =  E_CARD_UNPD_FLG_FUNC_ERROR;
                }
            }
            
        }

        outpFrame->data_len  = ucDataLen - 4;
    }
    else
    {
        for(i = 0; i < ucDataLen; i++)
        {
            outpFrame->data_pack[i] = aucDataTmp[i];
        }
        // 异常报文
        outpFrame->data_len  = ucDataLen;
        outpFrame->flag    = E_CARD_UNPD_FLG_ERROR_OK;
        ret = trans_card_data_err(E_CARD_TRANS_F2U,
                    (void*)&(outpFrame->data_unpack),
                    (void*)&(outpFrame->data_pack));

        if(E_CARD_OK == ret)
        {
            outpFrame->flag    = E_CARD_UNPD_FLG_ERROR_OK;
        }
        else
        {
            outpFrame->flag =  E_CARD_UNPD_FLG_FUNC_ERROR;
        }    
        
    
    }
    
    /* 封装输出最后信息 */
    outpFrame->lead_num  = nNumber0xFE;
    outpFrame->ctrl_c      = ucCtrlCode;
    outpFrame->ruler_id  = ulRulerID;
    
    outpFrame->frame_len = ucDataLen + 12;
    outpFrame->type      = stRulerInfor.type;
    outpFrame->format    = stRulerInfor.format;
#endif
    return E_CARD_OK;
}

/*************************************************
Function:       get_card_ruler_info
Description:    通过规约ID获得对应规约的详细信息:

                (1)规约的类型及额外数据
                (2)对应数据格式类型
                (3)数据域长度
                (4)数据域各数据单位
                (5)规约的可读写属性
                (6)该规约的数据项名称
                (7)实现对该条规约解析和封装函数指针

Author:         liming 

Calls:          
Called By:      
Input:          UINT32 rulerID 规约ID
                
Output:         S_CARD_RULER_INFO *outRulerInfo
Return:         正确返回 0 其它为错误类型

Others:         重要函数，规约解析核心函数
  
*************************************************/
INT32 get_card_ruler_info(UINT32 rulerID, S_CARD_PACK_FRAME *outPackInfo,S_CARD_RULER_INFO *outRulerInfo)
{
    UINT8 ucDi1 = 0;
    UINT8 ucDi0 = 0;
    UINT32 u_Data;

    INT32                len    = 0;                          // 数据域字节长度 
    char                 name[NAME_LEN]   = {0};              // 该条规约数据项名称

	char data[256] = {0};    
    char newdata[256] = {0};

    if(NULL == outRulerInfo)
    {
        return E_CARD_ERRO_NULL;
    }
	if(NULL == outPackInfo)
    {
        return E_CARD_ERRO_NULL;
    }

	//=======命令字与命令参数，数据的0和1=====
    ucDi0 =  rulerID & 0xFF;
    ucDi1 = (rulerID >> 8) & 0xFF;

    
    printf("D1 = 0x%02X \nD0 = 0x%02X\n",ucDi1,ucDi0);
//==================================================================================================
//===15H	功能命令类别(NAK)   接收数据错误（DKQ）   10H	查看版本命令代码
//===30H	功能命令类别  01H	设置串口通讯波特率
//===31H	功能命令类别    11H	查看读卡器版本信息
//===                                       13H	蜂鸣器控制命令代码
//===                                       14H	设置 LED 灯状态命令代码
//===                                       90H	打开射频
//===                                       91H	关闭射频
//===                                       26H	功能指令：进入低功耗
//===32H	功能命令类别    22H	卡上电命令代码
//===                                       23H	卡下电命令代码
//===                                       42H	弹卡命令代码
//===                                       41H	接触式卡弹卡命令代码
//===                                       24H	激活非接触卡命令代码
//===                                       26H	操作卡片数据命令代码   应用层传输
//===                                       93H	操作卡片数据命令代码   非接触卡直通命令
//=================================================================================================

    switch(ucDi1)
	{
		case 0x15:
            switch(ucDi0)
            {
				case 0x10://==== 查看版本命令代码
					sprintf(name, "查看版本命令代码\n");   
					break;
            }
            break;
		case 0x30:
            switch(ucDi0)
             {
				case 0x01://==== 设置串口通讯波特率
					outRulerInfo->len = 3;
					sprintf(name, "设置串口通讯波特率\n输入0x00 设置为9600bps\n输入0x01 设置为19200bps\n输入0x02 设置为38400bps\n输入0x03 设置为57600bps\n输入0x04 设置为115200bps\n");    
					//printf("输入0x00 设置为9600bps\n输入0x01 设置为19200bps\n输入0x02 设置为38400bps\n输入0x03 设置为57600bps\n输入0x04 设置为115200bps\n");						
					//scanf("%s",data);
					//printf("data==0x%x\n",(unsigned long)strtol(data,NULL,16));					
					break;
            }
			break;
		case 0x31:
            switch(ucDi0)
            {
				case 0x11://==== 查看读卡器版本信息
					outRulerInfo->len = 2;
					sprintf(name,"查看读卡器版本信息\n");
					break;
				case 0x13://==== 蜂鸣器控制命令代码
					outRulerInfo->len = 5;
					sprintf(name,"蜂鸣器控制命令代码\n设置蜂鸣器鸣叫时间0x0000-0xffff\n鸣叫次数0x01-0xff\n");
					//scanf("%s",data);
                    sprintf(data,"01F402"); //printf("data==0x%x\n",(unsigned long)strtol(data,NULL,16));
                    //u_Data = (unsigned long)strtol(data,NULL,16);
                    //sprintf(data,"%02X%02X%02X",(u_Data>>16)&0xFF,(u_Data>>8)&0xFF,u_Data&0xFF);
                    //HexStrToByte(data,newdata,strlen(data));
                    //printf("data==%s newdata==%s\n",data,newdata);
					break;
				case 0x14://==== 设置 LED 灯状态命令代码
					outRulerInfo->len = 3;
					sprintf(name,"设置 LED 灯状态命令代码\nBIT7 对应绿灯；0－关,1－亮\nBIT6 对应红灯；0－关,1－亮\n全亮0xb0\t绿灯亮0x80\t红灯亮0x40\t全灭0x00\n");
					//scanf("%s",data);
					//printf("data==0x%x\n",(unsigned long)strtol(data,NULL,16));	
                    sprintf(data,"b0");
					break;
				case 0x90://==== 打开射频
					outRulerInfo->len = 2;
					sprintf(name,"打开射频\n");
					break;	
				case 0x91://==== 关闭射频
					outRulerInfo->len = 2;
					sprintf(name,"关闭射频\n");
					break;
				case 0x26://==== 功能指令：进入低功耗
					outRulerInfo->len = 2;
					sprintf(name,"功能指令：进入低功耗\n");
					break;	
            }
			break;
		case 0x32:
            switch(ucDi0)
            {
				case 0x22://==== 卡上电命令代码
					outRulerInfo->len = 5;
                    sprintf(name,"等待插卡的时间,卡座号（用户卡：00H～0FH， PSAM 卡：10H~1FH）");
                    sprintf(data,"0000FF");
					break;
				case 0x23://==== 卡下电命令代码
                    outRulerInfo->len = 2;
					break;
                case 0x42://====接触式卡弹卡命令代码
					outRulerInfo->len = 5;
                    sprintf(name,"等待插卡的时间,卡座号（用户卡：00H～0FH， PSAM 卡：10H~1FH）");
                    sprintf(data,"0000FF");
					break;	
                case 0x41://==== 接触式卡主电源掉电
                    outRulerInfo->len = 4;
                    sprintf(name,"等待插卡的时间");
                    sprintf(data,"0000");
					break;
				case 0x24://==== 激活非接触卡命令代码
                    outRulerInfo->len = 4;
                    sprintf(name,"等待插卡的时间");
                    sprintf(data,"FFFF");
					break;
                case 0x26://==== 操作卡片数据命令代码   应用层传输
					
                    break;
				case 0x93://==== 操作卡片数据命令代码   非接触卡直通命令
                    break;
            }
			break;
        default:
            return  E_CARD_ERRO_UNKOWN_ID;
	}

	
	// 结果数据输出     
    outPackInfo->data_len = outRulerInfo->len;	
    sprintf(outRulerInfo->name, "%s", name);
	// data length			
	printf(" length\t=  %d\n", outRulerInfo->len);
	printf(" name\t=  %s\n\n", outRulerInfo->name);
    //scanf("%s",data);
    //printf("data==0x%x\n",(unsigned long)strtol(data,NULL,16));
    HexStrToByte(data,newdata,strlen(data));
    printf("data==%s newdata==%s\n",data,newdata);
    outPackInfo->data = newdata;

    return E_CARD_OK;
}

