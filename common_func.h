/*
 * common_func.h
 *
 *  Created on: 2013年9月9日
 *      Author: 1854
 */

#ifndef FORMAT_TIME_H_
#define FORMAT_TIME_H_

#include "server_namespace.h"
#include "common/common_typedef.h"

FRAME_ROBOT_NAMESPACE_BEGIN

//最大的时间格式化字符串长度
#define MAX_FORMAT_TIMESTRING_LEN		64

//获取当前时间的格式化字符串
int32_t GetTimeString(char arrTimeString[]);

FRAME_ROBOT_NAMESPACE_END

#endif /* FORMAT_TIME_H_ */
