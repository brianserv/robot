/*
 * message_handler.h
 *
 *  Created on: 2013年8月23日
 *      Author: 1854
 */

#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#include "server_namespace.h"
#include "lightframe_impl.h"
#include "common/common_singleton.h"
#include "player.h"
#include "main_frame.h"

#include <map>
using namespace std;

FRAME_ROBOT_NAMESPACE_BEGIN

typedef map<uint16_t, ServerMsgProc> ServerMsgMgt;

class CMessageHandler : public IMessageEventCS
{
public:
	CMessageHandler();
	//消息事件
	virtual int32_t OnMessageEvent(CSocket *pConnection, MessageHeadCS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

	void RegistAllMsgProc();

	void Regist(uint16_t nMsgID, ServerMsgProc proc)
	{
		m_stMsgMgt[nMsgID] = proc;
	}

protected:
	ServerMsgMgt			m_stMsgMgt;
};

#define g_MessageMgt				CSingleton<CMessageHandler>::GetInstance()

#define MESSAGE_MAP_BEGIN()		\
	FRAME_ROBOT_NAMESPACE_BEGIN		\
	void CMessageHandler::RegistAllMsgProc()	\
	{
#define ON_MESSAGE(id, proc)	\
		Regist(id, static_cast<ServerMsgProc>(&proc));	\
		g_Frame.RegistMsgHandler(id, this);
#define MESSAGE_MAP_END()	\
	}	\
	FRAME_ROBOT_NAMESPACE_END

FRAME_ROBOT_NAMESPACE_END

#endif /* MESSAGE_HANDLER_H_ */
