/*
 * system_event.cpp
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#include "system_event.h"

FRAME_ROBOT_NAMESPACE_BEGIN

int32_t CSystemEventHandler::OnSystemEvent(uint16_t nEventID, void *pParam)
{
	WRITE_DEBUG_LOG("recv sys connect event!\n");

	return S_OK;
}

FRAME_ROBOT_NAMESPACE_END


