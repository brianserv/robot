/*
 * common_func.h
 *
 *  Created on: 2013��9��9��
 *      Author: 1854
 */

#ifndef FORMAT_TIME_H_
#define FORMAT_TIME_H_

#include "server_namespace.h"
#include "common/common_typedef.h"

FRAME_ROBOT_NAMESPACE_BEGIN

//����ʱ���ʽ���ַ�������
#define MAX_FORMAT_TIMESTRING_LEN		64

//��ȡ��ǰʱ��ĸ�ʽ���ַ���
int32_t GetTimeString(char arrTimeString[]);

FRAME_ROBOT_NAMESPACE_END

#endif /* FORMAT_TIME_H_ */
