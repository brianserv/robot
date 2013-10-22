/*
 * login_timer.h
 *
 *  Created on: 2013Äê9ÔÂ10ÈÕ
 *      Author: 1854
 */

#ifndef LOGIN_TIMER_H_
#define LOGIN_TIMER_H_

#include "server_namespace.h"
#include "lightframe_impl.h"
#include "common/common_singleton.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"

FRAME_ROBOT_NAMESPACE_BEGIN

class CLoginTimer : public ITimerEvent
{
public:
	CLoginTimer(){}
	virtual ~CLoginTimer(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_LOGINTIMER_INSTANCE		CSingleton<CLoginTimer>::CreateInstance
#define	GET_LOGINTIMER_INSTANCE			CSingleton<CLoginTimer>::GetInstance
#define	DESTROY_LOGINTIMER_INSTANCE		CSingleton<CLoginTimer>::DestroyInstance

FRAME_ROBOT_NAMESPACE_END



#endif /* LOGIN_TIMER_H_ */
