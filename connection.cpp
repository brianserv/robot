/*
 * connection.cpp
 *
 *  Created on: 2013年8月22日
 *      Author: 1854
 */

#include "server_namespace.h"
#include "common/common_api.h"
#include "connection.h"
#include "player.h"
#include "frame_mem_mgt.h"
#include "frame_netqueue.h"
#include "message_define.h"
#include "frame_msgevent_mgt.h"

FRAME_ROBOT_NAMESPACE_BEGIN

CConnection::CConnection()
{
	m_pPlayer = NULL;
	m_pConnectSucccess = NULL;
	m_pConnectTimeout = NULL;
}

CConnection::~CConnection()
{

}

int32_t CConnection::MakeMessage()
{
	//开始组包
	while(m_stRecvBuffer.Size() > 0)
	{
		MessageHeadCS stMsgHead;
		if(m_nCurPacketSize <= 0)
		{
			//获取包大小字段所占的字节数
			int32_t nPacketLengthSize = sizeof(stMsgHead.nTotalSize);
			if(m_stRecvBuffer.Size() < nPacketLengthSize)
			{
				break;
			}

			//取出包头前面的包大小字段，然后解码出来
			uint8_t arrPacketSize[nPacketLengthSize];
			m_stRecvBuffer.PeekRead(arrPacketSize, nPacketLengthSize);

			uint32_t nOffset = 0;
			uint32_t nPacketSize = *((uint32_t *)arrPacketSize);
			m_nCurPacketSize = nPacketSize + nPacketLengthSize;
			//CCodeEngine::Decode(arrPacketSize, nPacketLengthSize, nOffset, m_nCurPacketSize);
		}

		int32_t nWantDataSize = (int32_t)(m_nCurPacketSize - m_nPacketOffset);
		int32_t nRealGetDataSize = (m_stRecvBuffer.Size() >= nWantDataSize ? nWantDataSize : m_stRecvBuffer.Size());
		if(nRealGetDataSize <= 0)
		{
			break;
		}

		if(m_pPacket == NULL)
		{
			int32_t nPacketLen = m_nCurPacketSize + sizeof(ConnInfo);
			uint8_t *pMem = MALLOC(sizeof(NetPacket) + nPacketLen);
			if(pMem == NULL)
			{
				break;
			}

			m_pPacket = new(pMem)NetPacket();
			m_pPacket->m_nNetPacketLen = nPacketLen;
		}

		//前面存放conninfo结构
		uint8_t *pPacketHead = &m_pPacket->m_pNetPacket[sizeof(ConnInfo)];

		if(m_stRecvBuffer.Read(&pPacketHead[m_nPacketOffset], nRealGetDataSize) <= 0)
		{
			break;
		}

		m_nPacketOffset += nRealGetDataSize;
		if(m_nPacketOffset >= m_nCurPacketSize)
		{
			uint32_t nOffset = 0;
			int32_t nRet = stMsgHead.MessageDecode(pPacketHead, m_nPacketOffset, nOffset);
			if(nRet < 0)
			{
				FREE((uint8_t *)m_pPacket);//g_FrameMemMgt.RecycleBlock((uint8_t *)m_pPacket);
				m_pPacket = NULL;
				m_nPacketOffset = 0;
				m_nCurPacketSize = 0;
				return nRet;
			}

			//过滤不关心的消息包
			if(!g_FrameMsgEventMgt.IsHasMessageEvent(stMsgHead.nMessageID))
			{
				FREE((uint8_t *)m_pPacket);
				m_pPacket = NULL;
				m_nPacketOffset = 0;
				m_nCurPacketSize = 0;
				continue;
			}

			//m_nRoleID = stMsgHead.nRoleID;
			MsgEventInfo * arrPMsgEventInfo[MAX_MSGEVENT_COUNT] = {NULL};
			int32_t nEventCount = 0;
			nRet = g_FrameMsgEventMgt.GetMessageEvent(stMsgHead.nMessageID, arrPMsgEventInfo, nEventCount);
			if ((nRet < 0) || (nEventCount <= 0))
			{
//				WRITE_MAIN_LOG(enmLogLevel_Warning, "it's not found msg hander!{msgid=0x%08x}\n", stMsgHead.nMessageID);
//				FREE((uint8_t *)m_pPacket);
//				m_pPacket = NULL;
//				m_nPacketOffset = 0;
//				m_nCurPacketSize = 0;
//				return ret;
			}

			for (int32_t i = 0; i < nEventCount; i++)
			{
				//获取IMsgBody
				IMsgBody *pMsgBody = g_MessageMapDecl.GetMessageBody(stMsgHead.nMessageID);
				if(NULL == pMsgBody)
				{
					//WRITE_MAIN_LOG(enmLogLevel_Warning, "it's not found msg body!{msgid=0x%08x}\n", stMsgHead.nMessageID);
					continue;
				}

				nRet = pMsgBody->MessageDecode(pPacketHead, m_nPacketOffset, nOffset);
				if( 0 > nRet)
				{
					//WRITE_MAIN_LOG(enmLogLevel_Error, "decode msg body failed!{ret=0x%08x, msgid=0x%08x}\n", ret, stMsgHead.nMessageID);
					continue;
				}

				if(stMsgHead.nMessageID == 18)
				{
					char arrTimeString[1024];
					GetTimeString(arrTimeString);

					uint32_t offset = 0;
					char szLog[enmMaxLogInfoLength];
					szLog[0] = 0;

					sprintf(szLog + offset, "{RecvTime : %s} MessageHeadCS=", arrTimeString);
					offset = (uint32_t)strlen(szLog);
					stMsgHead.Dump(szLog, enmMaxLogInfoLength, offset);

					sprintf(szLog + offset, " MessageBody=");
					offset = (uint32_t)strlen(szLog);
					pMsgBody->Dump(szLog + offset, enmMaxLogInfoLength, offset);

					WRITE_MAIN_LOG(enmLogLevel_Notice, "%s\n", szLog);
				}
			}

			ConnInfo *pConnInfo = new(&m_pPacket->m_pNetPacket[0]) ConnInfo;
			pConnInfo->pSocket = this;
			pConnInfo->nTunnelIndex = (uint16_t)m_nSocketFD;
			pConnInfo->nErrorCode = 0;

			g_FrameNetQueue.PushRecvCSQueue(m_pPacket);
			m_pPacket = NULL;
			m_nPacketOffset = 0;
			m_nCurPacketSize = 0;
		}
	}

	return S_OK;
}

int32_t CConnection::OnConnected()
{
	int32_t ret = S_OK;
	if((m_pPlayer != NULL) && (m_pConnectSucccess != NULL))
	{
		ret = m_pPlayer->ConnectServerSuccess();
	}
	return ret;
}

int32_t CConnection::OnConnectTimeout()
{
	int32_t ret = S_OK;
	if((m_pPlayer != NULL) && (m_pConnectTimeout != NULL))
	{
		ret = m_pPlayer->ConnectServerTimeout();
	}
	return ret;
}

void CConnection::SetConnectSuccessCallback(CPlayer *pPlayer, connectsuccess proc)
{
	m_pPlayer = pPlayer;
	m_pConnectSucccess = proc;
}

void CConnection::SetConnectTimeoutCallback(CPlayer *pPlayer, connecttimeout proc)
{
	m_pPlayer = pPlayer;
	m_pConnectTimeout = proc;
}

CPlayer *CConnection::GetPlayerObj()
{
	return m_pPlayer;
}

FRAME_ROBOT_NAMESPACE_END
