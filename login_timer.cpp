/*
 * login_timer.cpp
 *
 *  Created on: 2013Äê9ÔÂ10ÈÕ
 *      Author: 1854
 */


#include "login_timer.h"
#include "player.h"
#include "data_mgt.h"

FRAME_ROBOT_NAMESPACE_BEGIN

int32_t CLoginTimer::OnTimerEvent(CFrameTimer* pTimer)
{
	int32_t ret = S_OK;

	char arrAddress[] = "10.10.9.2";
	uint16_t nPort = 1234;
	//char arrAddress[] = "10.10.9.113";
	//uint16_t nPort = 1135;

	int32_t nUserCount = 1 + Random(29);

	for(int32_t i = 0; i < nUserCount; ++i)
	{
		CPlayer *pPlayer = g_DataMgt.PopPlayer();
		if(pPlayer == NULL)
		{
			break;
		}

		pPlayer->Initialize();

		ret = pPlayer->ConnectServer(arrAddress, nPort);
		if((ret != S_OK) && ((uint32_t)ret != E_SOCKET_CONNECTING))
		{
			WRITE_ERROR_LOG("user[%s]connect server[%s]:port[%d] is error!{ret=0x%08x}",
					pPlayer->GetUserName(), arrAddress, nPort, ret);
			g_DataMgt.PushPlayer(pPlayer);

			return E_UNKNOWN;
		}
	}

	return ret;
}

FRAME_ROBOT_NAMESPACE_END

