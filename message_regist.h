/*
 * message_regist.h
 *
 *  Created on: 2013��8��23��
 *      Author: 1854
 */

#include "server_namespace.h"
#include "message_define.h"
#include "message_handler.h"

//��Ϣӳ��꿪ʼ
MESSAGE_MAP_BEGIN()
//����Ϣ��������ӵ�ON_MESSAGE������
ON_MESSAGE(MSGID_LOGINGAME_RESP, CPlayer::LoginGameResp)
ON_MESSAGE(MSGID_WORLDCHAT_RESP, CPlayer::WorldChatResp)
//��Ϣӳ������
MESSAGE_MAP_END()
