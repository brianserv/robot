/*
 * data_mgt.h
 *
 *  Created on: 2013年9月10日
 *      Author: 1854
 */

#ifndef DATA_MGT_H_
#define DATA_MGT_H_

#include "server_namespace.h"
#include "common/common_object.h"
#include "common/common_singleton.h"
#include "player.h"
#include "lightframe_impl.h"

#include <list>
using namespace std;

FRAME_ROBOT_NAMESPACE_BEGIN

#define g_PlayerMgt		CSingleton<ObjectPool<CPlayer, 1000> >::GetInstance()

typedef list<CPlayer *>		PlayerList;

typedef uint8_t		CounterType;
enum
{
	enmCounterType_ConnectTimeout		= 0x00,
	enmCounterType_LoginTimeout		= 0x01,
};

class CDataMgt : public CObject, public ITimerEvent
{
public:
	CDataMgt()
	{
		m_nConnectTimeoutCount = 0;
		m_nLoginTimeoutCount = 0;
	}

	virtual ~CDataMgt(){};

	int32_t Initialize();
	int32_t Resume();
	int32_t Uninitialize();

	int32_t OnTimerEvent(CFrameTimer *pTimer);

	PlayerList &GetPlayerList();

	void PushPlayer(CPlayer *pPlayer);

	CPlayer *PopPlayer();

	void DelPlayer(CPlayer *pPlayer);

	void IncCounter(CounterType nType);

protected:
	PlayerList		m_stPlayerList;

	uint32_t		m_nConnectTimeoutCount;		//连接服务器超时的数量
	uint32_t		m_nLoginTimeoutCount;		//登陆游戏超时的数量
};

#define g_DataMgt		CSingleton<CDataMgt>::GetInstance()

FRAME_ROBOT_NAMESPACE_END

#endif /* DATA_MGT_H_ */
