/*
 * system_event.h
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#ifndef SYSTEM_EVENT_H_
#define SYSTEM_EVENT_H_

#include "server_namespace.h"
#include "lightframe_impl.h"
#include "main_frame.h"

FRAME_ROBOT_NAMESPACE_BEGIN

class CSystemEventHandler : public ISystemEvent
{
public:
	int32_t OnSystemEvent(uint16_t nEventID, void *pParam);
};

#define	CREATE_SYSTEMEVENT_INSTANCE		CSingleton<CSystemEventHandler>::CreateInstance
#define	GET_SYSTEMEVENT_INSTANCE		CSingleton<CSystemEventHandler>::GetInstance
#define	DESTROY_SYSTEMEVENT_INSTANCE	CSingleton<CSystemEventHandler>::DestroyInstance

FRAME_ROBOT_NAMESPACE_END


#endif /* SYSTEM_EVENT_H_ */
