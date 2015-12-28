/*****************************************************************
*版权声明:本程序模块属开发者个人
*         版权所有 2015-2115
*
*模块名称：源码统计
*模块描述：源码文件代码行、空格、注释统计
*开发作者：taiyueoa
*创建日期：2015/12/25
*模块版本：1.0.0.0
*
*****************************************************************/

// 去除新增函数(_s)安全警告
#define _CRT_SECURE_NO_WARNINGS

#include "FCinclude.h"

int CFileCounter::m_siAllLines = 0;
int CFileCounter::m_siFileNum = 0;

CFileCounter::CFileCounter()
{
  FcReset();
}

CFileCounter::~CFileCounter()
{
  FcReset();
}

// 重置
int CFileCounter::FcReset()
{
  m_iCmmdFlg = 0;
  FC_MEMSET(m_stCounter);

  return 0;
}

// 获取源码文件行信息
int CFileCounter::GetSourceFileLine(const char *p_buffer)
{
  WIN32_FIND_DATA winFileInfo;
  char * strPathFileName = new char[BUF_LEN];
  memset(strPathFileName, 0x00, BUF_LEN);

  strncpy(strPathFileName, p_buffer, BUF_LEN-1);
  strncat(strPathFileName, "\\*.*", MIN(sizeof("\\*.*"), sizeof(strPathFileName) - strlen(strPathFileName) - 1));

  // 目录检查，空文件夹则退出
  HANDLE hFind = FindFirstFile(strPathFileName, &winFileInfo);
  delete []strPathFileName;

  if (hFind == INVALID_HANDLE_VALUE)
  {
    return 0;
  }

  m_siAllLines = GetFileOrSubfileSoure(p_buffer, hFind, winFileInfo);

  return m_siAllLines;
}

// 查询文件（子文件）源码
int CFileCounter::GetFileOrSubfileSoure(const char *p_buffer, const HANDLE hFind, WIN32_FIND_DATA &winFileInfo)
{
  // 统计文件、子文件源码，直到最后一个文件末尾
  while(TRUE)
  {
    if(winFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if(winFileInfo.cFileName[0] != '.')//非隐藏目录
      {
        char *strTmpPath = new char[BUF_LEN];
        memset(strTmpPath, 0x00, BUF_LEN);

        strncpy(strTmpPath, p_buffer, BUF_LEN-1);
        strncat(strTmpPath, "\\", MIN(sizeof("\\"), sizeof(strTmpPath) - strlen(strTmpPath) - 1));// 转义符“\”
        strncat(strTmpPath, winFileInfo.cFileName,
          MIN(sizeof(winFileInfo.cFileName), sizeof(strTmpPath) - strlen(strTmpPath) - 1));

        GetSourceFileLine(strTmpPath);
        delete []strTmpPath;
      }
    }
    //筛选源代码文件条件
    else if (strstr(winFileInfo.cFileName, ".cpp") != NULL || strstr(winFileInfo.cFileName, ".c") != NULL
      || strstr(winFileInfo.cFileName, ".h") != NULL)
    {
      char strFilePath[BUF_LEN] = {0};
      char strLineBuffer[BUF_LEN*4] = {0};

      strncpy(strFilePath, p_buffer, BUF_LEN-1);
      strncat(strFilePath, "\\", MIN(sizeof("\\"), sizeof(strFilePath) - strlen(strFilePath) - 1));
      strncat(strFilePath, winFileInfo.cFileName, MIN(sizeof(winFileInfo.cFileName), sizeof(strFilePath) - strlen(strFilePath) - 1));

      // mutexlock
      FILE *fp = fopen(strFilePath, "r"); 
      if(NULL == fp)
      {
        return FC_RTMSG_ERROR;
      }

      while (fgets(strLineBuffer, BUF_LEN * 4, fp))
      {
        this->JudgeThenStatCounter(strLineBuffer);
        memset(strLineBuffer, 0x00, BUF_LEN*4); // 重置
        m_stCounter.Total++;
      }
      m_siAllLines += m_stCounter.Total;
      m_siFileNum++;
      // 关闭文件
      fclose(fp);

      // 单文件信息显示
      this->OutputStatCounter(winFileInfo.cFileName);
      //mutex unlock
    }

    if(!FindNextFile(hFind, &winFileInfo))
    {
      break;
    }
  }

  return m_siAllLines;
}

// 统计源码行信息,只考虑能编译通过的源码
int CFileCounter::JudgeThenStatCounter(const char * p_strLineBuffer)
{
  assert(p_strLineBuffer);
  int iRtset = 0;
  // 无信息，空行
  if(('\n' == p_strLineBuffer[0] || '\0' == p_strLineBuffer[0]) && m_iCmmdFlg == 0)
  {
    m_stCounter.Empty++;
  }
  else
  {    
    if(FC_RTMSG_OK == JudgeStartLineShield(p_strLineBuffer))
    {
      return FC_RTMSG_OK;
    }

    iRtset = JudgeStartLineOther(p_strLineBuffer);
  }// else

  return FC_RTMSG_OK;
}

// 屏蔽标识起始行判断
int CFileCounter::JudgeStartLineShield(const char *p_strLineBuffer)
{
  int iRtset = FC_RTMSG_ERROR;

  if(0 == strncmp("/*", p_strLineBuffer, 2))
  {
    m_stCounter.Comment++;

    if(NULL == FindString(p_strLineBuffer, "*/", false, true))
    {
      m_iCmmdFlg = 1; // (/**/) 注释体标识
    }
    iRtset = FC_RTMSG_OK;
  }
  else if(1 == m_iCmmdFlg) // (/*  */)注释体
  {
    m_stCounter.Comment++;

    if(NULL != FindString(p_strLineBuffer, "*/", false, true))
    {
      m_iCmmdFlg = 0; // (/**/) 注释体结束
    }
    iRtset = FC_RTMSG_OK;
  }
  else if(0 == strncmp("//", p_strLineBuffer, 2))
  {
    m_stCounter.Comment++; // (//)注释行
    iRtset = FC_RTMSG_OK;
  }

  return iRtset;
}

// 非标识起始行判断
int CFileCounter::JudgeStartLineOther(const char *p_strLineBuffer)
{
  int iLineLen = 0;
  int ipos = 0;
  int iEmpty = 0;
  int iEff = 0;
  int iCmmd = 0;

  iLineLen = strlen(p_strLineBuffer) - 1; // 作为下标判断条件，从0开始

  // 行遍历判断
  while(ipos <= iLineLen && 0 == m_iCmmdFlg)
  {
    // 过滤空格并进行空格标示，其他条件满足直接跳出
    if(' ' == p_strLineBuffer[ipos] || '\0' == p_strLineBuffer[ipos] || '\n' == p_strLineBuffer[ipos])
    {
      iEmpty = 1;
      ipos++;
    }
    else if(0 == strncmp("/*", p_strLineBuffer+ipos, 2) || 0 == strncmp("//", p_strLineBuffer+ipos, 2))
    {
      iCmmd = 1;
      iEmpty = 0;
      // 行中（/*  */)注释体标识
      if(NULL != FindString(p_strLineBuffer+ipos, "/*", false, true) && NULL == FindString(p_strLineBuffer+ipos, "*/", false, true))
      {
        m_iCmmdFlg = 1;
      }
      break;
    }
    else
    {
      iEff = 1; // 有效代码行
      iEmpty = 0;
      if(NULL != FindString(p_strLineBuffer, "/*", false, true) || FindString(p_strLineBuffer, "//", false, true))
      {
        iCmmd = 1;
        // 行中（/*  */)注释体标识
        if(NULL != FindString(p_strLineBuffer, "/*", false, true) && NULL == FindString(p_strLineBuffer, "*/", false, true))
        {
          m_iCmmdFlg = 1;
        }
      }
      
      // 非空格行，跳出
      break;
    }
  }//while

  // 根据标志位进行统计
  if(1 == iEmpty)
  {
    m_stCounter.Empty++;
  }
  if(1 == iEff)
  {
    m_stCounter.Effective++;
  }
  if(1 == iCmmd)
  {
    m_stCounter.Comment++;
  }

  return FC_RTMSG_OK;
}

// 计数结果输出打印
int CFileCounter::OutputStatCounter(const char *p_strFileName)
{
  if(NULL == p_strFileName)
  {
    return FC_RTMSG_ERROR;
  }

  cout<<"file: "<<p_strFileName<<"  total:"<<m_stCounter.Total<<"  empty:"<<m_stCounter.Empty
    <<"  effective:"<<m_stCounter.Effective<<"  comment:"<<m_stCounter.Comment<<endl;

  // 成员函数复位
  FcReset();

  return FC_RTMSG_OK;
}

// 获取成员变量
int CFileCounter::GetsiAllFileNum()
{
  return m_siFileNum;
}

int CFileCounter::GetsiAllFileLine()
{
  return m_siAllLines;
}
