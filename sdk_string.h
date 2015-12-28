#if !defined(__XSDK_STRING_H__)
#define __XSDK_STRING_H__

#define XSDKAPI               __stdcall

// c++ 混合编程
#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// 功能描述：字符串查找子字符串
// 入参说明：
//     [in]  p_pszString        被查找字符串
//     [in]  p_pszSubStr        待查找字符串
//     [in]  p_bMatchCase       大小写匹配
//     [in]  p_bMatchWord       整字匹配
// 返回说明：
//     NULL  被查找字符串中不包含待查找字符串
//     !NULL 偏移位置
char * XSDKAPI FindString(
      const char *p_pszString, 
      const char *p_pszSubStr, 
      bool p_bMatchCase = true, 
      bool p_bMatchWord = false
);

#ifdef __cplusplus
}
#endif

#endif  // __XSDK_STRING_H__
