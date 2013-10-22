/*
 * message_define.h
 *
 *  Created on: 2013年8月22日
 *      Author: 1854
 */

#ifndef MESSAGE_DEFINE_H_
#define MESSAGE_DEFINE_H_

#include "frame_msg_impl.h"
#include "server_namespace.h"
#include "common/common_string.h"
#include "common/common_singleton.h"
#include "frame_msg_impl.h"
#include "def.h"

FRAME_ROBOT_NAMESPACE_BEGIN

//玩家登陆游戏请求
#define MSGID_LOGINGAME_REQ		5
class CLoginGameReq : public IMsgBody
{
public:
	CLoginGameReq()
	{
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	CString<MAX_USERNAME_LENGTH>		strUserName;
	CString<MAX_PASSWORD_LENGTH>		strPassword;
};


typedef uint8_t		LoginGameResult;
enum
{
	LOGIN_SUCCESS		= 0x00,
	LOGIN_INFO_ERROR	= 0x01,
	LOGIN_USER_ONLINE	= 0x02,
};

#define MSGID_LOGINGAME_RESP	6
class CLoginGameResp : public IMsgBody
{
public:
	CLoginGameResp()
	{
		m_nResult		= 0;
		m_nPlayerID		= 0;
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	LoginGameResult		m_nResult;
	int32_t				m_nPlayerID;
	uint32_t			m_nUserID;
	uint8_t				m_nSex;
	uint8_t				m_arrNickName[48];
	uint8_t				m_nUserCountry;
	uint8_t				m_nUserMajor;
	uint8_t				m_nUserLevel;
	uint8_t				m_nUserHair;
};

#define MSGID_ENTERGAME_REQ		11
class CEnterGameReq : public IMsgBody
{
public:
	CEnterGameReq()
	{
		m_nIsRelogin = 0;
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	uint8_t				m_nIsRelogin;
};


#define MSGID_WORLDCHAT_REQ		18
class CWorldChatReq : public IMsgBody
{
public:
	CWorldChatReq()
	{
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	uint32_t				m_nUserID;
	uint8_t					m_nInfoType;
	uint8_t					m_nNameLen;
	uint8_t					m_nChatLen;
	CString<MAX_USERNAME_LENGTH>	m_strUserName;
	CString<MAX_CHATCONTENT_LENGTH>	m_strChatContent;
};


#define MSGID_WORLDCHAT_RESP	18
class CWorldChatResp : public IMsgBody
{
public:
	CWorldChatResp()
	{
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	uint32_t				m_nUserID;
	uint8_t					m_nUserCountry;
	uint8_t					m_nUserTitle;
	uint8_t					m_nInfoType;
	uint8_t					m_nNameLen;
	uint8_t					m_nChatLen;
	CString<MAX_USERNAME_LENGTH>	m_strUserName;
	CString<MAX_CHATCONTENT_LENGTH>	m_strChatContent;
};

#define MSGID_HEARTBEAT_REQ		33
class CHeartBeatReq : public IMsgBody
{
public:
	CHeartBeatReq()
	{
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);
};

#define MSGID_CHANGEMAP_REQ		157
class CChangeMapReq : public IMsgBody
{
public:
	CChangeMapReq()
	{
		m_nNewMapID = 0;
		m_nCoordX = 0;
		m_nCoordY = 0;
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	int32_t			m_nNewMapID;
	int32_t			m_nCoordX;
	int32_t			m_nCoordY;
};

#define MSGID_PLAYERMOVE_REQ		16
class CPlayerMoveReq : public IMsgBody
{
public:
	CPlayerMoveReq()
	{
		m_nCoordX = 0;
		m_nCoordY = 0;
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	int32_t			m_nCoordX;
	int32_t			m_nCoordY;
};

#define MSGID_PLAYERMOVE_NOTI		17
class CPlayerMoveNoti : public IMsgBody
{
public:
	CPlayerMoveNoti()
	{
		m_nUserID = 0;
		m_nCoordX = 0;
		m_nCoordY = 0;
		m_nLastActTime = 0;
	}

	virtual int32_t GetSize();
	virtual int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	virtual int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	virtual void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	int32_t			m_nUserID;
	int32_t			m_nCoordX;
	int32_t			m_nCoordY;
	int32_t			m_nLastActTime;
};


//只有接收的消息才加映射
MESSAGEMAP_DECL_BEGIN(message)
MESSAGEMAP_DECL_MSG(MSGID_LOGINGAME_RESP, new CLoginGameResp())
MESSAGEMAP_DECL_MSG(MSGID_WORLDCHAT_RESP, new CWorldChatResp())
//MESSAGEMAP_DECL_MSG(MSGID_PLAYERMOVE_NOTI, new CPlayerMoveNoti())
MESSAGEMAP_DECL_END()

FRAME_ROBOT_NAMESPACE_END

#endif /* MESSAGE_DEFINE_H_ */
