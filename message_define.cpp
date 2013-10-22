/*
 * message_define.cpp
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#include "common/common_errordef.h"
#include "message_define.h"
#include "server_namespace.h"

#include <string.h>

FRAME_ROBOT_NAMESPACE_BEGIN

int32_t CLoginGameReq::GetSize()
{
	return strUserName.GetBufSize() + strPassword.GetBufSize();
}

int32_t CLoginGameReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	memcpy(buf + offset, strUserName.GetString(), strUserName.GetBufSize());
	offset += strUserName.GetBufSize();

	memcpy(buf + offset, strPassword.GetString(), strPassword.GetBufSize());
	offset += strPassword.GetBufSize();

	return ret;
}

int32_t CLoginGameReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}

void CLoginGameReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{strUserName=%s, strPassWord=%s}", strUserName.GetString(),
			strPassword.GetString());
	offset = (uint32_t)strlen(buf);
}

int32_t CLoginGameResp::GetSize()
{
	return sizeof(m_nResult) + sizeof(m_nPlayerID) + sizeof(m_nUserID) + sizeof(m_nSex)
			+ sizeof(m_arrNickName) + sizeof(m_nUserCountry) + sizeof(m_nUserMajor)
			+ sizeof(m_nUserLevel) + sizeof(m_nUserHair);
}

int32_t CLoginGameResp::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}

int32_t CLoginGameResp::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	m_nResult = *(LoginGameResult *)(buf + offset);
	offset += sizeof(LoginGameResult);

	if(m_nResult != LOGIN_SUCCESS)
	{
		return ret;
	}

	m_nPlayerID = *(int32_t *)(buf + offset);
	offset += sizeof(int32_t);

	m_nUserID = *(uint32_t *)(buf + offset);
	offset += sizeof(uint32_t);

	m_nSex = *(uint8_t *)(buf + offset);
	offset += sizeof(uint8_t);

	memcpy(m_arrNickName, buf + offset, sizeof(m_arrNickName));
	offset += sizeof(m_arrNickName);

	m_nUserCountry = *(uint8_t *)(buf + offset);
	offset += sizeof(uint8_t);

	m_nUserMajor = *(uint8_t *)(buf + offset);
	offset += sizeof(uint8_t);

	m_nUserLevel = *(uint8_t *)(buf + offset);
	offset += sizeof(uint8_t);

	m_nUserHair = *(uint8_t *)(buf + offset);
	offset += sizeof(uint8_t);

	return ret;
}

void CLoginGameResp::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nResult=%d, m_nPlayerID=%d, m_nUserID=%u, m_nSex=%d, "
			"m_arrNickName=%s, m_nUserCountry=%d, m_nUserMajor=%d, m_nUserLevel=%d, "
			"m_nUserHair=%d}",
			m_nResult, m_nPlayerID, m_nUserID, m_nSex, m_arrNickName, m_nUserCountry,
			m_nUserMajor, m_nUserLevel, m_nUserHair);
	offset = (uint32_t)strlen(buf);
}


int32_t CEnterGameReq::GetSize()
{
	return sizeof(m_nIsRelogin);
}

int32_t CEnterGameReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	*((uint8_t *)(buf + offset)) = m_nIsRelogin;
	offset += sizeof(m_nIsRelogin);

	return ret;
}

int32_t CEnterGameReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}

void CEnterGameReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nIsRelogin=%d}", m_nIsRelogin);
	offset = (uint32_t)strlen(buf);
}

int32_t CWorldChatReq::GetSize()
{
	return sizeof(m_nUserID) + sizeof(m_nInfoType) + sizeof(m_nNameLen) + sizeof(m_nChatLen)
			+ m_nNameLen + m_nChatLen;
}
int32_t CWorldChatReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	*((uint32_t *)(buf + offset)) = m_nUserID;
	offset += sizeof(m_nUserID);

	*((uint8_t *)(buf + offset)) = m_nInfoType;
	offset += sizeof(m_nInfoType);

	*((uint8_t *)(buf + offset)) = m_nNameLen;
	offset += sizeof(m_nNameLen);

	*((uint8_t *)(buf + offset)) = m_nChatLen;
	offset += sizeof(m_nChatLen);

	memcpy(buf + offset, m_strUserName.GetString(), m_nNameLen);
	offset += m_nNameLen;

	memcpy(buf + offset, m_strChatContent.GetString(), m_nChatLen);
	offset += m_nChatLen;

	return ret;
}
int32_t CWorldChatReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}
void CWorldChatReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nUserID=%u, m_nInfoType=%d, m_nNameLen=%d, m_nChatLen=%d, "
			"m_strUserName=%s, m_strChatContent=%s}",
			m_nUserID, m_nInfoType, m_nNameLen, m_nChatLen, m_strUserName.GetString(),
			m_strChatContent.GetString());
	offset = (uint32_t)strlen(buf);
}

int32_t CWorldChatResp::GetSize()
{
	return sizeof(m_nUserID) + sizeof(m_nInfoType) + sizeof(m_nNameLen) + sizeof(m_nChatLen)
			+ sizeof(m_nUserCountry) + sizeof(m_nUserTitle) + m_nNameLen + m_nChatLen;
}
int32_t CWorldChatResp::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}
int32_t CWorldChatResp::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	m_nUserID = *((uint32_t *)(buf + offset));
	offset += sizeof(m_nUserID);

	m_nUserCountry = *((uint8_t *)(buf + offset));
	offset += sizeof(m_nUserCountry);

	m_nUserTitle = *((uint8_t *)(buf + offset));
	offset += sizeof(m_nUserTitle);

	m_nInfoType = *((uint8_t *)(buf + offset));
	offset += sizeof(m_nInfoType);

	m_nNameLen = *((uint8_t *)(buf + offset));
	offset += sizeof(m_nNameLen);

	m_nChatLen = *((uint8_t *)(buf + offset));
	offset += sizeof(m_nChatLen);

	char arrUserName[MAX_USERNAME_LENGTH];
	memcpy(arrUserName, buf + offset, m_nNameLen);
	arrUserName[m_nNameLen] = '\0';
	offset += m_nNameLen;

	m_strUserName = arrUserName;

	char arrChatContent[MAX_CHATCONTENT_LENGTH];
	memcpy(arrChatContent, buf + offset, m_nChatLen);
	arrChatContent[m_nChatLen] = '\0';
	offset += m_nChatLen;

	m_strChatContent = arrChatContent;

	return ret;
}

void CWorldChatResp::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nUserID=%u, m_nInfoType=%d, m_nNameLen=%d, m_nChatLen=%d, "
			"m_strUserName=%s, m_strChatContent=%s}",
			m_nUserID, m_nInfoType, m_nNameLen, m_nChatLen, m_strUserName.GetString(),
			m_strChatContent.GetString());
	offset = (uint32_t)strlen(buf);
}

int32_t CHeartBeatReq::GetSize()
{
	return 0;
}

int32_t CHeartBeatReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}

int32_t CHeartBeatReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	return ret;
}

void CHeartBeatReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{send heart beat!}");
	offset = (uint32_t)strlen(buf);
}

int32_t CChangeMapReq::GetSize()
{
	return sizeof(m_nNewMapID) + sizeof(m_nCoordX) + sizeof(m_nCoordY);
}

int32_t CChangeMapReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	*((int32_t *)(buf + offset)) = m_nNewMapID;
	offset += sizeof(m_nNewMapID);

	*((int32_t *)(buf + offset)) = m_nCoordX;
	offset += sizeof(m_nCoordX);

	*((int32_t *)(buf + offset)) = m_nCoordY;
	offset += sizeof(m_nCoordY);

	return ret;
}

int32_t CChangeMapReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	return S_OK;
}

void CChangeMapReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nNewMapID=%d, m_nCoordX=%d, m_nCoordY=%d}",
			m_nNewMapID, m_nCoordX, m_nCoordY);
	offset = (uint32_t)strlen(buf);
}

int32_t CPlayerMoveReq::GetSize()
{
	return sizeof(m_nCoordX) + sizeof(m_nCoordY);
}

int32_t CPlayerMoveReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	*((int32_t *)(buf + offset)) = m_nCoordX;
	offset += sizeof(m_nCoordX);

	*((int32_t *)(buf + offset)) = m_nCoordY;
	offset += sizeof(m_nCoordY);

	return ret;
}

int32_t CPlayerMoveReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	return S_OK;
}

void CPlayerMoveReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nCoordX=%d, m_nCoordY=%d}", m_nCoordX, m_nCoordY);
	offset = (uint32_t)strlen(buf);
}

int32_t CPlayerMoveNoti::GetSize()
{
	return sizeof(m_nUserID) + sizeof(m_nCoordX) + sizeof(m_nCoordY) + sizeof(m_nLastActTime);
}

int32_t CPlayerMoveNoti::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t ret = S_OK;

	*((int32_t *)(buf + offset)) = m_nUserID;
	offset += sizeof(m_nUserID);

	*((int32_t *)(buf + offset)) = m_nCoordX;
	offset += sizeof(m_nCoordX);

	*((int32_t *)(buf + offset)) = m_nCoordY;
	offset += sizeof(m_nCoordY);

	*((int32_t *)(buf + offset)) = m_nLastActTime;
	offset += sizeof(m_nLastActTime);

	return ret;
}

int32_t CPlayerMoveNoti::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	return S_OK;
}

void CPlayerMoveNoti::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf, "{m_nUserID=%d, m_nCoordX=%d, m_nCoordY=%d, m_nLastActTime=%d}",
			m_nUserID, m_nCoordX, m_nCoordY, m_nLastActTime);
	offset = (uint32_t)strlen(buf);
}

FRAME_ROBOT_NAMESPACE_END
