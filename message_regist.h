/*
 * message_regist.h
 *
 *  Created on: 2013年8月23日
 *      Author: 1854
 */

#include "server_namespace.h"
#include "message_define.h"
#include "message_handler.h"

//消息映射宏开始
MESSAGE_MAP_BEGIN()
//把消息处理函数添加到ON_MESSAGE宏里面
ON_MESSAGE(MSGID_LOGINGAME_RESP, CPlayer::LoginGameResp)
ON_MESSAGE(MSGID_WORLDCHAT_RESP, CPlayer::WorldChatResp)
//消息映射宏结束
MESSAGE_MAP_END()
