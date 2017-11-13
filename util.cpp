
#include <stdio.h>
#include <windows.h>
#include <direct.h>

//--------------------------------------------------
// 文字列変換
//--------------------------------------------------
char *SJIStoUTF8(char *utf8,size_t size,const char *str){
	wchar_t buf[0x200];
	int len=MultiByteToWideChar(CP_ACP,0,str,-1,buf,_countof(buf));
	utf8[WideCharToMultiByte(CP_UTF8,0,buf,len,utf8,size-1,nullptr,nullptr)]='\0';
	return utf8;
}
char *UTF8toSJIS(char *sjis,size_t size,const char *str){
	wchar_t buf[0x200];
	int len=MultiByteToWideChar(CP_UTF8,0,str,-1,buf,_countof(buf));
	sjis[WideCharToMultiByte(CP_ACP,0,buf,len,sjis,size-1,nullptr,nullptr)]='\0';
	return sjis;
}
char *UTF16toUTF8(char *utf8,size_t size,const wchar_t *str){
	utf8[WideCharToMultiByte(CP_UTF8,0,str,-1,utf8,size-1,nullptr,nullptr)]='\0';
	return utf8;
}

//--------------------------------------------------
// 文字列比較
//--------------------------------------------------
int _strrcmpi(const char *str1,const char *str2){
	size_t len1=strlen(str1),len2=strlen(str2);
	if(len1<len2)return 1;
	return _strcmpi(&str1[len1-len2],str2);
}

//--------------------------------------------------
// 文字列の前後の空白と改行を削除
//--------------------------------------------------
char *Trim(char *str){
	if(str){
		for(;*str;str++)if(*str!='\t'&&*str!='\v'&&*str!='\f'&&*str!=' '&&*str!='\n'&&*str!='\r')break;
		//case '\u00A0':case '\uFEFF':case '\u2028':case '\u2029':
		int len=strlen(str);
		if(len>0)for(char *s=&str[len-1];s>=str;s--){if(*s!='\t'&&*s!='\v'&&*s!='\f'&&*s!=' '&&*s!='\n'&&*s!='\r')break;*s='\0';}
	}
	return str;
}

//--------------------------------------------------
// ディレクトリを取得
//--------------------------------------------------
char *GetDirectory(char *directory,size_t size,const char *path){
	if(size>0)directory[0]='\0';
	for(int i=strlen(path)-1;i>=0;i--){
		if(path[i]=='\\'||path[i]=='/'){
			if(i>(int)size-1)i=(int)size-1;
			memcpy(directory,path,i);
			directory[i]='\0';
			break;
		}
	}
	return directory;
}

//--------------------------------------------------
// プロセス作成
//--------------------------------------------------
void Process(char *cmd){
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	GetStartupInfoA(&si);
	CreateProcessA(nullptr,cmd,nullptr,nullptr,0,0x08000000,nullptr,nullptr,&si,&pi);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
}

//--------------------------------------------------
// ディレクトリ作成
//--------------------------------------------------
void MakeDirectory(const char *path,bool path_is_file){
	if(path_is_file){
		char buf[0x200];
		strcpy_s(buf,_countof(buf),path);
		for(int i=strlen(buf)-1;i>2;i--){
			if(buf[i]=='\\'||buf[i]=='/'){
				buf[i]='\0';
				if(_mkdir(buf)){
					MakeDirectory(buf,true);
					_mkdir(buf);
				}
				break;
			}
		}
	}else{
		if(_mkdir(path)){
			MakeDirectory(path,true);
			_mkdir(path);
		}
	}
}

//--------------------------------------------------
// ファイルデータコピー
//--------------------------------------------------
void fcpy(FILE *d,FILE *s,size_t size){
	unsigned char data[0x4000];
	for(;size>=sizeof(data);size-=sizeof(data)){
		fread(data,sizeof(data),1,s);
		fwrite(data,sizeof(data),1,d);
	}
	if(size){
		fread(data,size,1,s);
		fwrite(data,size,1,d);
	}
}

//--------------------------------------------------
// ファイルデータ読み込み
//--------------------------------------------------
unsigned char *LoadFile(const char *path,size_t *size){
	if(size)*size=0;
	FILE *fp;if(fopen_s(&fp,path,"rb"))return nullptr;
	fseek(fp,0,SEEK_END);
	size_t len=ftell(fp);
	auto *buf=new unsigned char [len+1];
	if(buf){
		fseek(fp,0,SEEK_SET);
		fread(buf,1,len,fp);
		buf[len]='\0';
		if(size)*size=len;
	}
	fclose(fp);
	return buf;
}

//--------------------------------------------------
// ファイルの存在チェック
//--------------------------------------------------
bool ExistFile(const char *path){
	bool r=false;
	FILE *fp;if(fopen_s(&fp,path,"rb")==0){
		fclose(fp);
		r=true;
	}
	return r;
}

//--------------------------------------------------
// 実数出力
//--------------------------------------------------
void fprintf_(FILE *fp,double f){
	char buf[64];sprintf_s(buf,_countof(buf),"%.3lf",f);
	for(int i=strlen(buf)-1;i>=0&&(buf[i]=='0'||buf[i]=='.');i--){if(buf[i]=='.'){buf[i]='\0';break;}buf[i]='\0';}
	fputs(buf,fp);
}
void fprintf_(FILE *fp,const char *str1,double f,const char *str2){
	if(str1)fputs(str1,fp);
	fprintf_(fp,f);
	if(str2)fputs(str2,fp);
}
