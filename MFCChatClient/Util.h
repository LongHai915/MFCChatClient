#ifndef _UTIL_H_
#define _UTIL_H_

#include <afxwin.h>

enum //自定义协议
{
	USER_LOAD = 0,              //用户登陆
	USER_LOAD_SUCCEED,          //登录成功
	USER_LOAD_LOSS_HAS_USER,	//登录失败(用户已登录)
	USER_LOAD_LOSS_NO_USER,     //登录失败(用户名不存在)
	USER_LOAD_LOSS_ERROR_PSW,	//登录失败(密码错误)

	USER_REGISTER,              //用户注册
	USER_REGISTER_SUCCEED,      //注册成功
	USER_REGISTER_LOSS_HAS_USER,//注册失败(用户名已存在)
	USER_REGISTER_LOSS_ERROR,	//注册失败(未知原因)
	USER_REGISTER_STOP,			//注册被禁止

	USER_UPDATE_PWD,            //用户密码修改
	USER_UPDATE_PWD_SUCCEED,    //更新密码成功
	USER_UPDATE_PWD_ERROR,		//更新密码（旧密码错误）
	USER_UPDATE_PWD_LOSS,       //更新密码失败

	USER_UPDATE_NOTICE,         //用户公告管理
	USER_UPDATE_NOTICE_SUCCEED, //更新公告成功
	USER_UPDATE_NOTICE_LOSS,    //更新公告失败

	USER_MSGTOALL,              //发送信息给所有的人
	USER_MSGTOPERSONAL,         //发送信息给私人
	USER_STOP_MSG,				//禁言用户

	USER_OUT_USER,				//踢出用户
	USER_DELETE_USER,			//删除用户

	USER_USERNAMELIST,          //用户列表
	USER_SYSTEMNOTICE,			//系统公告
	USER_PERSONNOTICE,			//请求用户公告详情
	USER_NOTICELIST,            //用户公告列表

	USER_EXIT,
};

struct UserMsg
{
	int Protocol;              //协议 	
	TCHAR UserName[20];        //用户名	
	TCHAR UserPwd[20];         //密码	
	TCHAR m_FontName[20];
	int m_FontSize;
	BOOL bIsBold;	//黑体
	BOOL bIsItalic;	//斜体
	BOOL bIsUnderline;//下划线
	COLORREF crColor;
	TCHAR UserNotice[300];     //公告
	TCHAR Msg[5000];            //接收到消息
};

struct UserInfo
{
	CString UserName;          //用户昵称
	CString IP;                //用户IP
	SOCKET socket;             //用户的套接字
};

#endif