/*
 * init_timer.h
 *
 *  Created on: 2013Äê8ÔÂ22ÈÕ
 *      Author: 1854
 */

#ifndef INIT_TIMER_H_
#define INIT_TIMER_H_

#include "server_namespace.h"
#include "lightframe_impl.h"
#include "common/common_singleton.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"

FRAME_ROBOT_NAMESPACE_BEGIN

class CInitTimer : public ITimerEvent
{
public:
	CInitTimer(){}
	virtual ~CInitTimer(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_INITTIMER_INSTANCE		CSingleton<CInitTimer>::CreateInstance
#define	GET_INITTIMER_INSTANCE			CSingleton<CInitTimer>::GetInstance
#define	DESTROY_INITTIMER_INSTANCE		CSingleton<CInitTimer>::DestroyInstance

FRAME_ROBOT_NAMESPACE_END


#endif /* INIT_TIMER_H_ */
