//*****************************************************************
//版权声明:本程序模块属开发者
//         非原创，源码引用
//
//模块名称：字符串操作
//模块描述：
//开发作者：
//创建日期：2001/01/01
//模块版本：1.0.0.0
//
//*****************************************************************

#include <sdk_string.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

char * XSDKAPI FindString(
                          const char *p_pszString, 
                          const char *p_pszSubStr, 
                          bool p_bMatchCase /*= false*/, 
                          bool p_bMatchWord /*= false*/
                          )
{
  long lSubStrLen = 0L;
  char *pszSubStr = NULL, *pszSubStrUpr = NULL, *pszSubStrLwr = NULL, *pszCursor = NULL;

  assert(p_pszString != NULL && p_pszSubStr != NULL);
  pszCursor = (char *)p_pszString;
  lSubStrLen = ((p_bMatchCase == false || p_bMatchWord == true) ? (long)strlen(p_pszSubStr) : 0L);
  do
  {
    if (p_bMatchCase == false)
    {
      pszSubStrUpr = strchr(pszCursor, toupper(p_pszSubStr[0]));
      pszSubStrLwr = strchr(pszCursor, tolower(p_pszSubStr[0]));

      if (pszSubStrUpr == NULL && pszSubStrLwr == NULL)
      {
        pszSubStr = NULL;
        break;
      }
      else if (pszSubStrLwr == NULL)
      {
        pszCursor = pszSubStrUpr;
      }
      else if (pszSubStrUpr == NULL)
      {
        pszCursor = pszSubStrLwr;
      }
      else if ((long)(pszSubStrLwr - pszSubStrUpr) > 0L)
      {
        pszCursor = pszSubStrUpr;
      }
      else
      {
        pszCursor = pszSubStrLwr;
      }

      if (memicmp(pszCursor, p_pszSubStr, lSubStrLen) == 0)
      {
        if (p_bMatchWord == false
          || (pszCursor == p_pszString)
          || (pszCursor != p_pszString 
          && ((*(pszCursor + lSubStrLen)) != '_'
          && isalpha((unsigned char)(*(pszCursor + lSubStrLen))) == 0 
          && isdigit((unsigned char)(*(pszCursor + lSubStrLen))) == 0)))
        {
          pszSubStr = pszCursor;
          break;
        }
      }
      pszCursor++;
    }
    else
    {
      pszCursor = strstr(pszCursor, p_pszSubStr);
      if (pszCursor != NULL)
      {
        if (p_bMatchWord == false
          || (pszCursor == p_pszString 
          && (*(pszCursor + lSubStrLen)) != '_'
          && isalpha((unsigned char)(*(pszCursor + lSubStrLen))) == 0 
          && isdigit((unsigned char)(*(pszCursor + lSubStrLen))) == 0)
          || (pszCursor != p_pszString 
          && (*(pszCursor - 1)) != '_'
          && isalpha((unsigned char)(*(pszCursor - 1))) == 0 
          && isdigit((unsigned char)(*(pszCursor - 1))) == 0
          && (*(pszCursor + lSubStrLen)) != '_'
          && isalpha((unsigned char)(*(pszCursor + lSubStrLen))) == 0 
          && isdigit((unsigned char)(*(pszCursor + lSubStrLen))) == 0))
        {
          pszSubStr = pszCursor;
          break;
        }
        pszCursor += lSubStrLen;
      }
      else
      {
        pszSubStr = NULL;
        break;
      }      
    }
  }while(true);

  return pszSubStr;
}
