/*
 * data_mgt.cpp
 *
 *  Created on: 2013Äê9ÔÂ10ÈÕ
 *      Author: 1854
 */

#include "data_mgt.h"
#include "main_frame.h"

FRAME_ROBOT_NAMESPACE_BEGIN

int32_t CDataMgt::Initialize()
{
	return S_OK;
}

int32_t CDataMgt::Resume()
{
	return S_OK;
}

int32_t CDataMgt::Uninitialize()
{
	return S_OK;
}

int32_t CDataMgt::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("Connect Timeout : %d, Login Timeout : %d",
			m_nConnectTimeoutCount, m_nLoginTimeoutCount);

	return S_OK;
}

PlayerList &CDataMgt::GetPlayerList()
{
	return m_stPlayerList;
}

void CDataMgt::PushPlayer(CPlayer *pPlayer)
{
	if(pPlayer != NULL)
	{
		m_stPlayerList.push_back(pPlayer);
	}
}

CPlayer *CDataMgt::PopPlayer()
{
	CPlayer *pPlayer = NULL;
	if(m_stPlayerList.empty())
	{
		return pPlayer;
	}

	pPlayer = m_stPlayerList.front();
	m_stPlayerList.pop_front();

	return pPlayer;
}

void CDataMgt::DelPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL)
	{
		return;
	}

	for(PlayerList::iterator it = m_stPlayerList.begin(); it != m_stPlayerList.end();)
	{
		if(*it == pPlayer)
		{
			m_stPlayerList.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void CDataMgt::IncCounter(CounterType nType)
{
	switch(nType)
	{
	case enmCounterType_ConnectTimeout:
		++m_nConnectTimeoutCount;
		break;
	case enmCounterType_LoginTimeout:
		++m_nLoginTimeoutCount;
		break;
	default:
		break;
	}
}


FRAME_ROBOT_NAMESPACE_END
