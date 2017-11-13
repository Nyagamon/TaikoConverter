#pragma once

#include <stdio.h>
#ifndef _countof
#define _countof(o) (sizeof(o)/sizeof(o[0]))
#endif

//inline unsigned int Align(unsigned int v,unsigned int align){return((v+align-1)&(-1^(align-1)))-v;}
//inline unsigned int Aligned(unsigned int v,unsigned int align){return(v+align-1)&(-1^(align-1));}
inline unsigned int Align(unsigned int v,unsigned int align){v%=align;return v?align-v:0;}
inline unsigned int Aligned(unsigned int v,unsigned int align){return v+Align(v,align);}

//--------------------------------------------------
// 文字列変換
//--------------------------------------------------
char *SJIStoUTF8(char *utf8,size_t size,const char *str);
char *UTF8toSJIS(char *sjis,size_t size,const char *str);
char *UTF16toUTF8(char *utf8,size_t size,const wchar_t *str);

//--------------------------------------------------
// 文字列比較
//--------------------------------------------------
int _strrcmpi(const char *str1,const char *str2);

//--------------------------------------------------
// 文字列の前後の空白と改行を削除
//--------------------------------------------------
char *Trim(char *str);

//--------------------------------------------------
// ディレクトリを取得
//--------------------------------------------------
char *GetDirectory(char *directory,size_t size,const char *path);

//--------------------------------------------------
// プロセス作成
//--------------------------------------------------
void Process(char *cmd);

//--------------------------------------------------
// ディレクトリ作成
//--------------------------------------------------
void MakeDirectory(const char *path,bool path_is_file=true);

//--------------------------------------------------
// ファイルデータコピー
//--------------------------------------------------
void fcpy(FILE *d,FILE *s,size_t size);

//--------------------------------------------------
// ファイルデータ読み込み
//--------------------------------------------------
unsigned char *LoadFile(const char *path,size_t *size=nullptr);

//--------------------------------------------------
// ファイルの存在チェック
//--------------------------------------------------
bool ExistFile(const char *path);

//--------------------------------------------------
// 実数出力
//--------------------------------------------------
void fprintf_(FILE *fp,double f=0.0);
void fprintf_(FILE *fp,const char *str1,double f=0.0,const char *str2=nullptr);
