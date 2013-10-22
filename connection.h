/*
 * connection.h
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "server_namespace.h"
#include "common/common_objectpool.h"
#include "frame_outsidesocket.h"

#include <string>
using namespace std;

FRAME_ROBOT_NAMESPACE_BEGIN

class CPlayer;
typedef int32_t (CPlayer::*connectsuccess)(void);
typedef int32_t (CPlayer::*connecttimeout)(void);

class CConnection : public COutsideSocket
{
public:
	CConnection();
	virtual ~CConnection();

	virtual int32_t OnConnected();

	virtual int32_t OnConnectTimeout();

	void SetConnectSuccessCallback(CPlayer *pPlayer, connectsuccess proc);

	void SetConnectTimeoutCallback(CPlayer *pPlayer, connecttimeout proc);

	CPlayer *GetPlayerObj();

protected:
	int32_t MakeMessage();

protected:
	CPlayer				*m_pPlayer;
	connectsuccess		m_pConnectSucccess;
	connecttimeout		m_pConnectTimeout;
};

FRAME_ROBOT_NAMESPACE_END

#endif /* CONNECTION_H_ */
