#pragma once

#include "Base.h"

// 存储统计计数
typedef struct sStatCounters
{
  int Total;
  int Empty;
  int Effective;
  int Comment;
}stStatCounters;

class CFileCounter: public CBase
{
public: 
  CFileCounter();
  ~CFileCounter();

  // reset
  int FcReset();
  // 获取文件行
  int GetSourceFileLine(const char *p_strPathFileName);
  // 查询文件（子文件）源码
  int GetFileOrSubfileSoure(const char *p_buffer, const HANDLE hFind, WIN32_FIND_DATA &winFileInfo);
  // 根据p_buffer进行对应项计数
  int JudgeThenStatCounter(const char * p_strLineBuffer);
  // 输出打印代码统计计数结果
  int OutputStatCounter(const char *p_strFileName);
  // 注释标识起始行判断
  int JudgeStartLineShield(const char *p_strLineBuffer);
  // 非注释标识起始行判断
  int JudgeStartLineOther(const char *p_strLineBuffer);

  //返回成员变量
  int GetsiAllFileNum();
  int GetsiAllFileLine();

private:
  stStatCounters m_stCounter;
  int m_iCmmdFlg; // /*  */注释体标识
  static int m_siAllLines;// 多线程运行时统计总数
  static int m_siFileNum;
};
