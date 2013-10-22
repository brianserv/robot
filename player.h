/*
 * player.h
 *
 *  Created on: 2013��8��22��
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
	//�������춨ʱ��
	int32_t OnWorldChatTimer(CFrameTimer *pTimer);
	//������ʱ��
	int32_t OnHeartBeatTimer(CFrameTimer *pTimer);
	//��½��Ӧ��ʱ
	int32_t OnLoginTimeout(CFrameTimer *pTimer);
	//�������ʱ�䶨ʱ��
	int32_t OnlineTimer(CFrameTimer *pTimer);
	//�л���ͼ��ʱ��
	int32_t OnChangeMapTimer(CFrameTimer *pTimer);
	//�ƶ���ʱ��
	int32_t OnMoveTimer(CFrameTimer *pTimer);

	int32_t ConnectServer(const char *arrAddr, uint16_t nPort);
	int32_t ConnectServerSuccess();
	int32_t ConnectServerTimeout();

	int32_t SendMessage(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);

	//�����û���
	void SetUserName(const char *arrUserName);
	//��ȡ�û���
	const char *GetUserName();
	//�����û�����
	void SetPassword(const char *arrPassword);
	//��ȡ�û�����
	const char *GetPassword();
	//��½��Ϸ����
	int32_t LoginGameReq();
	//��½��Ϸ��Ӧ����
	int32_t LoginGameResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);
	//�ǳ���Ϸ����
	int32_t LogoutGameReq();
	//������Ϸ����
	int32_t EnterGameReq();

	//�л���ͼ����
	int32_t ChangeMapReq(int32_t nMapID, CPoint &stPoint);

	//������������
	int32_t WorldChatReq(char *arrChat);
	//����������Ӧ����
	int32_t WorldChatResp(MessageHeadCS *pMsgHead, IMsgBody *pMsgBody);

protected:
	//��ʼ������ƶ�������
	int32_t InitMoveArea(int32_t nMaxCoordX, int32_t nMaxCoordY);
	//��ȡ�����һ���ߵ�λ�ã���ʱ���ƶ���
	CPoint GetNextMovePosition();

public:
	CConnection							m_stConnection;
	CString<MAX_USERNAME_LENGTH>		m_strUserName;
	CString<MAX_PASSWORD_LENGTH>		m_strPassword;

	uint32_t							m_nUserID;
	int32_t								m_nCurPos;					//��ǰ��λ��
	WayList								m_stWayList;				//��ҵ�����·��

	TimerIndex							m_nLoginTimerIndex;			//�û���½��ʱ��
	TimerIndex							m_nOnlineTimerIndex;		//�û����߶�ʱ��
	TimerIndex							m_nChangeMapTimerIndex;		//�û��л���ͼ��ʱ��
	TimerIndex							m_nWorldChatTimerIndex;		//�������춨ʱ��
	TimerIndex							m_nHeartBeatTimerIndex;		//������ʱ��
	TimerIndex							m_nMoveTimerIndex;			//�ƶ���ʱ��
};


TIMER_DATA_BEGIN(PlayerData)
public:
	CPlayer * playerData;
TIMER_DATA_END()


FRAME_ROBOT_NAMESPACE_END

#endif /* PLAYER_H_ */
