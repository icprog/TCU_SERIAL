#ifndef SPN_LINLONG_DEFINE
#define SPN_LINLONG_DEFINE

#include<stdio.h>
//登录的权限	
#define ROOT 	0
#define DO   	1
#define QUERY	2
//电流曲线设置数据库对应的id号
#define A_COLOR_ID 1
#define B_COLOR_ID 2
#define C_COLOR_ID 3
#define N_COLOR_ID 4
#define FLUSH_LINE 5
#define LINE_TYPE  6
	//线的类型
  	#define FRANK_LINE 0	
	#define BROKEN_LINE 1
//铃声设置
#define SOUNDWARNING	7
#define SOUNDWARN	8
#define SOUNDOUT	9
//系统参数设置
#define SYSNET	10 
	#define SYSNETHAVE 1
	#define SYSNETNO   0
#define SYSV	11 //巡检速度
#define SYSC	12 //指令的超时时间
#define SYSD	13 //屏幕等待时间
#define SYSCO	14 //重发次数
#define SYSIFCO	15 //系统是否重发
	#define SYSIFCOYES 1
	#define SYSIFCONO  0
#define SYSCOLORWARNING 16 //系统报警颜色
#define SYSCOLORWARN	17 //预警颜色
#define SYSCOLOROUT	18 //系统故障颜色
#define SYSCOLORGEAR	19 //系统故障颜色
//报警查询
#define QUERYSEE	1
#define QUERYNOTSEE 	0
//类型的id号
#define	LOGIN	4	//登录
#define	UUSER	5	//修改用户
#define DUSER	6	//删除用户
#define IUSER 	7	//添加用户
#define PQUER	8  	//打印记录
#define SLIU	9 	//设置电流参数
#define SNODE	10	//设置节点参数
#define SNODES	11	//设置节点属性
#define SOUND	12	//设置了铃声
#define STOU	13	//进行脱扣
#define SSYS	14	//进行系统参数设置 
#define SZI	15	//进行自检
#define SREBOOT	16	//重启系统
#define SINIT	17	//初始化系统
#define SSYSTI	18	//设置系统时间
#define SPWD	19	//修改密码
#define SQUIT	20	//系统退出
#endif
