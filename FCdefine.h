#pragma once

// 去除新增函数(_s)安全警告
#define _CRT_SECURE_NO_WARNINGS

#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))

#define BUF_LEN             256

#define FC_MEMSET(a)      memset((void *)&(a), 0x00, sizeof(a));

enum
{
    FC_RTMSG_ERROR = -1,
    FC_RTMSG_OK = 0,
    FC_RTMSG_EXCPETION = 100
};
