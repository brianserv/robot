/*
 * init_timer.cpp
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#include "server_namespace.h"
#include "init_timer.h"
#include "player.h"
#include "data_mgt.h"
#include "login_timer.h"

FRAME_ROBOT_NAMESPACE_BEGIN

#define IPV4_ADDRESS_LEN 	16

int32_t CInitTimer::OnTimerEvent(CFrameTimer* pTimer)
{
	WRITE_DEBUG_LOG("-----------init timer----------\n");

	int32_t ret = S_OK;

	for(int i = 200; i < 300; ++i)
	{
		CPlayer *pPlayer = g_PlayerMgt.pop_obj();
		if(pPlayer == NULL)
		{
			WRITE_WARNING_LOG("we cann't pop a player object!\n");
			return E_NULLPOINTER;
		}

		char buf[64];
		int32_t len = sprintf(buf, "autorun%d", i + 1);
		buf[len] = '\0';

		WRITE_DEBUG_LOG("{account = %s, password = %s}\n", buf, buf);

		pPlayer->SetUserName(buf);
		pPlayer->SetPassword(buf);

		g_DataMgt.PushPlayer(pPlayer);
	}

	int32_t nTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CLoginTimer::OnTimerEvent),
			&(GET_LOGINTIMER_INSTANCE()), NULL, 1000 * US_PER_MS, true, nTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CDataMgt::OnTimerEvent),
			&g_DataMgt, NULL, 10 * US_PER_SECOND, true, nTimerIndex);
	if (ret < 0)
	{
		return ret;
	}

	return ret;
}

FRAME_ROBOT_NAMESPACE_END
