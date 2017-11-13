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
// ������ϊ�
//--------------------------------------------------
char *SJIStoUTF8(char *utf8,size_t size,const char *str);
char *UTF8toSJIS(char *sjis,size_t size,const char *str);
char *UTF16toUTF8(char *utf8,size_t size,const wchar_t *str);

//--------------------------------------------------
// �������r
//--------------------------------------------------
int _strrcmpi(const char *str1,const char *str2);

//--------------------------------------------------
// ������̑O��̋󔒂Ɖ��s���폜
//--------------------------------------------------
char *Trim(char *str);

//--------------------------------------------------
// �f�B���N�g�����擾
//--------------------------------------------------
char *GetDirectory(char *directory,size_t size,const char *path);

//--------------------------------------------------
// �v���Z�X�쐬
//--------------------------------------------------
void Process(char *cmd);

//--------------------------------------------------
// �f�B���N�g���쐬
//--------------------------------------------------
void MakeDirectory(const char *path,bool path_is_file=true);

//--------------------------------------------------
// �t�@�C���f�[�^�R�s�[
//--------------------------------------------------
void fcpy(FILE *d,FILE *s,size_t size);

//--------------------------------------------------
// �t�@�C���f�[�^�ǂݍ���
//--------------------------------------------------
unsigned char *LoadFile(const char *path,size_t *size=nullptr);

//--------------------------------------------------
// �t�@�C���̑��݃`�F�b�N
//--------------------------------------------------
bool ExistFile(const char *path);

//--------------------------------------------------
// �����o��
//--------------------------------------------------
void fprintf_(FILE *fp,double f=0.0);
void fprintf_(FILE *fp,const char *str1,double f=0.0,const char *str2=nullptr);
