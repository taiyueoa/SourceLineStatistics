/*****************************************************************
*版权声明:本程序模块属开发者个人
*         版权所有 2015-2115
*
*模块名称：主文件
*模块描述：源码文件代码行、空格、注释统计
*开发作者：taiyueoa
*创建日期：2015/12/25
*模块版本：1.0.0.0
*
*****************************************************************/

// 去除新增函数(_s)安全警告
#define _CRT_SECURE_NO_WARNINGS

#include "FCinclude.h"

// 函数入口
int main(int argc, char* argv[])
{
  int iAllLineCount = 0;
  int iAllFileNUM = 0;
  int i = 0;
  CFileCounter *clFCounter = new CFileCounter();

  for (i=1; i<argc; i++)
  {
    // 代码统计计数
    iAllLineCount += clFCounter->GetSourceFileLine(argv[i]);
  }

  iAllFileNUM = clFCounter->GetsiAllFileNum();
  iAllLineCount = clFCounter->GetsiAllFileLine();
  cout<<"@@@@@ "<<iAllFileNUM<<" Source Files are "<<iAllLineCount<<" Lines"<<endl;
  // 释放
  delete clFCounter;
  clFCounter = NULL;

  return 0;
}
