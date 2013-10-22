/*
 * message_handler.cpp
 *
 *  Created on: 2013Äê8ÔÂ23ÈÕ
 *      Author: 1854
 */

#include "message_handler.h"
#include "message_regist.h"


FRAME_ROBOT_NAMESPACE_BEGIN

CMessageHandler::CMessageHandler()
{
}

int32_t CMessageHandler::OnMessageEvent(CSocket *pConnection, MessageHeadCS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t ret = S_OK;

	CConnection *pConn = dynamic_cast<CConnection *>(pConnection);
	if(pConn == NULL)
	{
		return ret;
	}

	CPlayer *pPlayer = pConn->GetPlayerObj();
	if(pPlayer == NULL)
	{
		return ret;
	}

	if(m_stMsgMgt.count(pMsgHead->nMessageID) <= 0)
	{
		return ret;
	}

	ServerMsgProc proc = m_stMsgMgt[pMsgHead->nMessageID];

	ret = (pPlayer->*proc)(pMsgHead, pMsgBody);

	return ret;
}


FRAME_ROBOT_NAMESPACE_END

