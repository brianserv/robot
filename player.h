/*
 * player.h
 *
 *  Created on: 2013年8月22日
 *      Author: 1854
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "server_namespace.h"
#include "common/common_string.h"
#include "common/common_singleton.h"
#include "common/common_object.h"
#include "def.h"
#include "connection.h"
#include "lightframe_impl.h"

#include <vector>
using namespace std;

FRAME_ROBOT_NAMESPACE_BEGIN

typedef int32_t (CPlayer::*ServerMsgProc)(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);

class CPoint
{
public:
	CPoint()
	{
		m_nCoordX = 0;
		m_nCoordY = 0;
	}

	CPoint(int32_t x, int32_t y):m_nCoordX(x),m_nCoordY(y)
	{

	}

	int32_t		m_nCoordX;
	int32_t		m_nCoordY;
};

typedef vector<CPoint>		WayList;

class CPlayer : public CObject, public ITimerEvent
{
public:
	CPlayer();
	virtual ~CPlayer();

	int32_t Initialize();
	int32_t Resume();
	int32_t Uninitialize();

	int32_t OnTimerEvent(CFrameTimer *pTimer);
	//世界聊天定时器
	int32_t OnWorldChatTimer(CFrameTimer *pTimer);
	//心跳定时器
	int32_t OnHeartBeatTimer(CFrameTimer *pTimer);
	//登陆响应超时
	int32_t OnLoginTimeout(CFrameTimer *pTimer);
	//玩家在线时间定时器
	int32_t OnlineTimer(CFrameTimer *pTimer);
	//切换地图定时器
	int32_t OnChangeMapTimer(CFrameTimer *pTimer);
	//移动定时器
	int32_t OnMoveTimer(CFrameTimer *pTimer);

	int32_t ConnectServer(const char *arrAddr, uint16_t nPort);
	int32_t ConnectServerSuccess();
	int32_t ConnectServerTimeout();

	int32_t SendMessage(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);

	//设置用户名
	void SetUserName(const char *arrUserName);
	//获取用户名
	const char *GetUserName();
	//设置用户密码
	void SetPassword(const char *arrPassword);
	//获取用户密码
	const char *GetPassword();
	//登陆游戏请求
	int32_t LoginGameReq();
	//登陆游戏响应处理
	int32_t LoginGameResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);
	//登出游戏请求
	int32_t LogoutGameReq();
	//进入游戏请求
	int32_t EnterGameReq();

	//切换地图请求
	int32_t ChangeMapReq(int32_t nMapID, CPoint &stPoint);

	//世界聊天请求
	int32_t WorldChatReq(char *arrChat);
	//世界聊天响应处理
	int32_t WorldChatResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);

protected:
	//初始化玩家移动的区域
	int32_t InitMoveArea(int32_t nMaxCoordX, int32_t nMaxCoordY);
	//获取玩家下一个走的位置（逆时针移动）
	CPoint GetNextMovePosition();

public:
	CConnection							m_stConnection;
	CString<MAX_USERNAME_LENGTH>		m_strUserName;
	CString<MAX_PASSWORD_LENGTH>		m_strPassword;

	uint32_t							m_nUserID;
	int32_t								m_nCurPos;					//当前的位置
	WayList								m_stWayList;				//玩家的行走路线

	TimerIndex							m_nLoginTimerIndex;			//用户登陆定时器
	TimerIndex							m_nOnlineTimerIndex;		//用户在线定时器
	TimerIndex							m_nChangeMapTimerIndex;		//用户切换地图定时器
	TimerIndex							m_nWorldChatTimerIndex;		//世界聊天定时器
	TimerIndex							m_nHeartBeatTimerIndex;		//心跳定时器
	TimerIndex							m_nMoveTimerIndex;			//移动定时器
};


TIMER_DATA_BEGIN(PlayerData)
public:
	CPlayer * playerData;
TIMER_DATA_END()


FRAME_ROBOT_NAMESPACE_END

#endif /* PLAYER_H_ */
