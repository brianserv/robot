/*
 * player.cpp
 *
 *  Created on: 2013年8月22日
 *      Author: 1854
 */

#include "server_namespace.h"
#include "common/common_errordef.h"
#include "player.h"
#include "frame_netthread.cpp"
#include "main_frame.h"
#include "message_define.h"
#include "common_func.h"
#include "data_mgt.h"


FRAME_ROBOT_NAMESPACE_BEGIN

CPlayer::CPlayer()
{
	m_nUserID = 0;
	m_nCurPos = 0;
	m_nLoginTimerIndex = enmInvalidTimerIndex;
	m_nOnlineTimerIndex = enmInvalidTimerIndex;
	m_nChangeMapTimerIndex = enmInvalidTimerIndex;
	m_nWorldChatTimerIndex = enmInvalidTimerIndex;
	m_nHeartBeatTimerIndex = enmInvalidTimerIndex;
	m_nMoveTimerIndex = enmInvalidTimerIndex;
}

CPlayer::~CPlayer()
{

}

int32_t CPlayer::Initialize()
{
	int32_t ret = S_OK;

	ret = m_stConnection.OpenSocketAsClient();
	if(ret < 0)
	{
		WRITE_ERROR_LOG("open socket as client error!{ret=0x%08x}", ret);
		return ret;
	}

	CEpoll *pEpoll = g_FrameNetThread.GetEpollObj();
	ret = pEpoll->RegistEvent(&m_stConnection, EPOLLIN | EPOLLOUT | EPOLLET);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("regist epoll event error!{ret=0x%08x}", ret);
		return ret;
	}

	m_stConnection.SetEpollObj(pEpoll);

	m_stConnection.SetConnectSuccessCallback(this, &CPlayer::ConnectServerSuccess);
	m_stConnection.SetConnectTimeoutCallback(this, &CPlayer::ConnectServerTimeout);

	return ret;
}

int32_t CPlayer::Resume()
{
	int32_t ret = S_OK;

	return ret;
}

int32_t CPlayer::Uninitialize()
{
	int32_t ret = S_OK;

	return ret;
}

int32_t CPlayer::OnTimerEvent(CFrameTimer *pTimer)
{
	return S_OK;
}

//世界聊天定时器
int32_t CPlayer::OnWorldChatTimer(CFrameTimer *pTimer)
{
	int32_t ret = S_OK;

	//return ret;

	static int32_t nCount = 0;

	char arrChat[256];
	int32_t nContentLen = sprintf(arrChat, "count=%d", ++nCount);
	arrChat[nContentLen] = '\0';

	WorldChatReq(arrChat);

	return ret;
}
//心跳定时器
int32_t CPlayer::OnHeartBeatTimer(CFrameTimer *pTimer)
{
	int32_t ret = S_OK;

	//不处于连接状态
	if((m_stConnection.GetStatus() != enmSocketStatus_Connected) &&
			(m_stConnection.GetStatus() != enmSocketStatus_Available))
	{
		//删除已存在的心跳定时器
		if(m_nHeartBeatTimerIndex != enmInvalidTimerIndex)
		{
			g_Frame.RemoveTimer(m_nHeartBeatTimerIndex);
			m_nHeartBeatTimerIndex = enmInvalidTimerIndex;
		}

		return ret;
	}

	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_HEARTBEAT_REQ;

	CHeartBeatReq stHeartBeatReq;

	//发送心跳请求
	ret = SendMessage(&stMsgHead, &stHeartBeatReq);

	return ret;
}

int32_t CPlayer::OnLoginTimeout(CFrameTimer *pTimer)
{
	int32_t ret = S_OK;

	WRITE_DEBUG_LOG("user[%s] login time out!", m_strUserName.GetString());
	LogoutGameReq();

	g_DataMgt.IncCounter(enmCounterType_LoginTimeout);

	return ret;
}

int32_t CPlayer::OnlineTimer(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("online timer : user[%s] logout!", m_strUserName.GetString());
	//登出
	LogoutGameReq();
	return S_OK;
}

//切换地图定时器
int32_t CPlayer::OnChangeMapTimer(CFrameTimer *pTimer)
{
	int32_t ret = S_OK;

	CPoint stPoint((500 + Random(2000)), (250 + Random(500)));

	int32_t arrMapID[] = {4};
	int32_t nMapID = arrMapID[Random((sizeof(arrMapID) / sizeof(int32_t)) - 1)];

	ret = ChangeMapReq(nMapID, stPoint);
	if(ret < 0)
	{
		return ret;
	}

	//初始化玩家移动路线
	InitMoveArea(stPoint.m_nCoordX, stPoint.m_nCoordY);

	int32_t nCyclePeriod = 1;
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnMoveTimer),
			this, NULL, nCyclePeriod * US_PER_SECOND, true, m_nMoveTimerIndex);
	if(ret < 0)
	{
		WRITE_ERROR_LOG("create move timer error!{ret=0x%08x}\n", ret);

		LogoutGameReq();
		return ret;
	}

	return ret;
}

//移动定时器
int32_t CPlayer::OnMoveTimer(CFrameTimer *pTimer)
{
	CPoint stPoint = GetNextMovePosition();

	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_PLAYERMOVE_REQ;

	CPlayerMoveReq stReq;
	stReq.m_nCoordX = stPoint.m_nCoordX;
	stReq.m_nCoordY = stPoint.m_nCoordY;

	//WRITE_DEBUG_LOG("x=%d, y=%d", stPoint.m_nCoordX, stPoint.m_nCoordY);

	return SendMessage(&stMsgHead, &stReq);
}

int32_t CPlayer::ConnectServer(const char *arrAddr, uint16_t nPort)
{
	WRITE_WARNING_LOG("user[%s] connecting server[%s]:port[%d]", m_strUserName.GetString(), arrAddr, nPort);
	return m_stConnection.Connect(arrAddr, nPort);
}

int32_t CPlayer::ConnectServerSuccess()
{
	WRITE_DEBUG_LOG("user[%s] connect server[%s]:port[%d] success!{socketfd=%d}",
			m_strUserName.GetString(), m_stConnection.GetPeerAddressStr(),
			m_stConnection.GetPeerPort(), m_stConnection.GetSocketFD());

//	int32_t nCyclePeriod = 5 + Random(5);
//	int32_t ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnlineTimer),
//			this, NULL, nCyclePeriod * US_PER_SECOND, false, m_nOnlineTimerIndex);
//	if(ret < 0)
//	{
//		WRITE_ERROR_LOG("connect server success, but create online timer error!{ret = 0x%08x}\n", ret);
//
//		LogoutGameReq();
//		return E_UNKNOWN;
//	}

	return LoginGameReq();
}

int32_t CPlayer::ConnectServerTimeout()
{
	WRITE_WARNING_LOG("user:%s connect server timeout!", m_strUserName.GetString());
	LogoutGameReq();

	g_DataMgt.IncCounter(enmCounterType_ConnectTimeout);

	return S_OK;
}

int32_t CPlayer::SendMessage(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody)
{
	if((m_stConnection.GetStatus() != enmSocketStatus_Connected) &&
			(m_stConnection.GetStatus() != enmSocketStatus_Available))
	{
		WRITE_DEBUG_LOG("send message failed, user[%s] logout!", m_strUserName.GetString());
		return LogoutGameReq();
	}

	return g_Frame.PostMessage(pMsgHead, pMsgBody, &m_stConnection);
}

void CPlayer::SetUserName(const char *arrUserName)
{
	m_strUserName = arrUserName;
}

//获取用户名
const char *CPlayer::GetUserName()
{
	return m_strUserName.GetString();
}

void CPlayer::SetPassword(const char *arrPassword)
{
	m_strPassword = arrPassword;
}

//获取用户密码
const char *CPlayer::GetPassword()
{
	return m_strPassword.GetString();
}

int32_t CPlayer::LoginGameReq()
{
	WRITE_DEBUG_LOG("user[%s] going to login game!", m_strUserName.GetString());

	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_LOGINGAME_REQ;

	CLoginGameReq stLoginGameReq;
	stLoginGameReq.strUserName = m_strUserName;
	stLoginGameReq.strPassword = m_strPassword;

	//发送登陆请求
	int32_t ret = SendMessage(&stMsgHead, &stLoginGameReq);

	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnLoginTimeout),
			this, NULL, 5 * US_PER_SECOND, false, m_nLoginTimerIndex);
	if (ret < 0)
	{
		WRITE_DEBUG_LOG("create logintimeout timer failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return ret;
	}

	return ret;
}

int32_t CPlayer::LoginGameResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody)
{
	CLoginGameResp *pResp = dynamic_cast<CLoginGameResp *>(pMsgBody);
	if(pResp == NULL)
	{
		WRITE_DEBUG_LOG("dynamic cast logingame resp failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return E_UNKNOWN;
	}

	WRITE_DEBUG_LOG("user[%s] id[%d] recv login game resp!", m_strUserName.GetString(),
			pResp->m_nUserID);

	m_nUserID = pResp->m_nUserID;

	g_Frame.RemoveTimer(m_nLoginTimerIndex);

	int32_t ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnHeartBeatTimer),
			this, NULL, 30 * US_PER_SECOND, true, m_nHeartBeatTimerIndex);
	if (ret < 0)
	{
		WRITE_DEBUG_LOG("create heartbeat timer failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return ret;
	}

	return EnterGameReq();
}

//登出游戏请求
int32_t CPlayer::LogoutGameReq()
{
	WRITE_DEBUG_LOG("user[%s] logout game!", m_strUserName.GetString());

	if((m_stConnection.GetStatus() == enmSocketStatus_Connected) ||
			(m_stConnection.GetStatus() == enmSocketStatus_Available))
	{
		m_stConnection.CloseSocket(SYS_EVENT_CONN_CLIENT_CLOSED);
	}

	//删除已存在的登陆定时器
	if(m_nLoginTimerIndex != enmInvalidTimerIndex)
	{
		g_Frame.RemoveTimer(m_nLoginTimerIndex);
		m_nLoginTimerIndex = enmInvalidTimerIndex;
	}

	//删除已存在的在线定时器
	if(m_nOnlineTimerIndex != enmInvalidTimerIndex)
	{
		g_Frame.RemoveTimer(m_nOnlineTimerIndex);
		m_nOnlineTimerIndex = enmInvalidTimerIndex;
	}

	//删除已存在的心跳定时器
	if(m_nHeartBeatTimerIndex != enmInvalidTimerIndex)
	{
		g_Frame.RemoveTimer(m_nHeartBeatTimerIndex);
		m_nHeartBeatTimerIndex = enmInvalidTimerIndex;
	}

	//删除已存在的世界聊天定时器
	if(m_nWorldChatTimerIndex != enmInvalidTimerIndex)
	{
		g_Frame.RemoveTimer(m_nWorldChatTimerIndex);
		m_nWorldChatTimerIndex = enmInvalidTimerIndex;
	}

	//回收对象
	g_DataMgt.PushPlayer(this);

	return S_OK;
}

int32_t CPlayer::EnterGameReq()
{
	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_ENTERGAME_REQ;

	CEnterGameReq stReq;
	stReq.m_nIsRelogin = 0;

	int32_t ret = SendMessage(&stMsgHead, &stReq);

	int32_t nSendPeriod = 1000;//5000 + Random(15000);

	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnWorldChatTimer),
			this, NULL, nSendPeriod * US_PER_MS, true, m_nWorldChatTimerIndex);
	if (ret < 0)
	{
		WRITE_DEBUG_LOG("create worldchat timer failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return ret;
	}

	int32_t nPeriod = 3 + Random(10);
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CPlayer::OnChangeMapTimer),
			this, NULL, nPeriod * US_PER_SECOND, false, m_nChangeMapTimerIndex);
	if (ret < 0)
	{
		WRITE_DEBUG_LOG("create changemap timer failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return ret;
	}

	return ret;
}

//切换地图请求
int32_t CPlayer::ChangeMapReq(int32_t nMapID, CPoint &stPoint)
{
	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_CHANGEMAP_REQ;

	CChangeMapReq stReq;
	stReq.m_nNewMapID = nMapID;
	stReq.m_nCoordX = stPoint.m_nCoordX;
	stReq.m_nCoordY = stPoint.m_nCoordY;

	int32_t ret = SendMessage(&stMsgHead, &stReq);
	if(ret < 0)
	{
		return ret;
	}

	return ret;
}

//世界聊天请求
int32_t CPlayer::WorldChatReq(char *arrChat)
{
	MessageHeadCS stMsgHead;
	stMsgHead.nMessageID = MSGID_WORLDCHAT_REQ;

	CWorldChatReq stReq;
	stReq.m_nUserID = m_nUserID;
	stReq.m_nInfoType = 1;//世界聊天
	stReq.m_nNameLen = m_strUserName.GetStringLength();
	stReq.m_nChatLen = strlen(arrChat);
	stReq.m_strUserName = m_strUserName;
	stReq.m_strChatContent = arrChat;

	int32_t ret = SendMessage(&stMsgHead, &stReq);

	char arrTimeString[MAX_FORMAT_TIMESTRING_LEN];
	GetTimeString(arrTimeString);

	WRITE_DEBUG_LOG("userid[%d] send worldchat[%s] at %s", m_nUserID, arrChat, arrTimeString);

	return ret;
}

//世界聊天响应处理
int32_t CPlayer::WorldChatResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody)
{
	//WRITE_DEBUG_LOG("recv world chat resp!\n");

	CWorldChatResp *pResp = dynamic_cast<CWorldChatResp *>(pMsgBody);
	if(pResp == NULL)
	{
		WRITE_DEBUG_LOG("dynamic cast worldchat resp failed, user[%s] logout!", m_strUserName.GetString());
		LogoutGameReq();
		return E_UNKNOWN;
	}

	char arrTimeString[MAX_FORMAT_TIMESTRING_LEN];
	GetTimeString(arrTimeString);

	WRITE_DEBUG_LOG("userid[%d] recv worldchat[%s] at %s", m_nUserID,
			pResp->m_strChatContent.GetString(), arrTimeString);

	return S_OK;
}

//初始化玩家移动的区域
int32_t CPlayer::InitMoveArea(int32_t nMaxCoordX, int32_t nMaxCoordY)
{
	//左上的x坐标
	int32_t nUpLeftCoordX = 50 + Random(nMaxCoordX - 50);
	//左上的y坐标
	int32_t nUpLeftCoordY = 50 + Random(nMaxCoordY - 50);
	//左下的x坐标
	int32_t nDownLeftCoordX = nUpLeftCoordX;
	//左下的y坐标
	int32_t nDownLeftCoordY = 10 + Random(nUpLeftCoordY - 20);
	//右下的x坐标
	int32_t nDownRightCoordX = (nDownLeftCoordX + Random(nMaxCoordX - nDownLeftCoordX));
	//右下的y坐标
	int32_t nDownRightCoordY = nDownLeftCoordY;
	//右上的x坐标
	int32_t nUpRightCoordX = nDownRightCoordX;
	//右上的y坐标
	int32_t nUpRightCoordY = nUpLeftCoordY;

	m_nCurPos = 0;

	for(int32_t i = 0; i < (nUpLeftCoordY - nDownLeftCoordY); ++i)
	{
		CPoint stPoint(nUpLeftCoordX, nUpLeftCoordY - i);

		m_stWayList.push_back(stPoint);
	}

	for(int32_t i = 0; i < (nDownRightCoordX - nDownLeftCoordX); ++i)
	{
		CPoint stPoint(nDownLeftCoordX + i, nDownLeftCoordY);

		m_stWayList.push_back(stPoint);
	}

	for(int32_t i = 0; i < (nUpRightCoordY - nDownRightCoordY); ++i)
	{
		CPoint stPoint(nDownRightCoordX, nDownRightCoordY + i);

		m_stWayList.push_back(stPoint);
	}

	for(int32_t i = 0; i < (nUpRightCoordX - nUpLeftCoordX); ++i)
	{
		CPoint stPoint(nUpLeftCoordX - i, nUpRightCoordY);

		m_stWayList.push_back(stPoint);
	}
	return S_OK;
}

//获取玩家下一个走的位置（逆时针移动）
CPoint CPlayer::GetNextMovePosition()
{
	CPoint &stPoint = m_stWayList[m_nCurPos++];
	m_nCurPos %= m_stWayList.size();
	return stPoint;
}

FRAME_ROBOT_NAMESPACE_END
