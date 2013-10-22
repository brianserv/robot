/*
 * main_frame.cpp
 *
 *  Created on: 2013年8月21日
 *      Author: 1854
 */

#include "main_frame.h"
#include "frame_eventid.h"
#include "frame_logengine.h"
#include "def.h"
#include "frame_netthread.h"
#include "message_define.h"
#include "connection.h"
#include "message_handler.h"
#include "init_timer.h"

FRAME_ROBOT_NAMESPACE_BEGIN

CMainFrame::CMainFrame()
:CLightFrame(SERVER_NAME_STRING)
{

}

CMainFrame::~CMainFrame()
{
}

//框架初始话
int32_t CMainFrame::Initialize()
{
	int32_t ret=S_OK;
	ret = CLightFrame::Initialize();
	if(0 > ret)
	{
		//WriteLog
		return ret;
	}

	int32_t nTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CInitTimer::OnTimerEvent),
			&(GET_INITTIMER_INSTANCE()), NULL, 5 * US_PER_SECOND, false, nTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	//注册所有的消息
	RegistMsg();

	return S_OK;
}

//恢复框架
int32_t CMainFrame::Resume()
{
	return CLightFrame::Resume();

}
//销毁框架
int32_t CMainFrame::Terminate()
{
	g_eServerCommand = enmServerCommand_Terminate;
	return S_OK;
}

void CMainFrame::Run()
{
	CLightFrame::Run();
}

void CMainFrame::Reload()
{
	g_eServerCommand = enmServerCommand_Reload;
}

void CMainFrame::RegistMsg()
{
	//初始化消息编解码表
	INITIALIZE_MESSAGEMAP_DECL(message);
	g_MessageMgt.RegistAllMsgProc();
	//注册连接成功通知
	RegistSysEvent(SYS_EVENT_INITIATIVE_CONNECT_SUCCESS, &CREATE_SYSTEMEVENT_INSTANCE());
}

FRAME_ROBOT_NAMESPACE_END




