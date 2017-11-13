
#include "clSFO.h"
#include <stdio.h>
#include <string.h>
#include "util.h"

clSFO::clSFO():m_node(){}

bool clSFO::Load(const char *path){
	m_node.DeleteChild();
	FILE *fp;
	if(fopen_s(&fp,path,"rb"))return false;
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	auto *buf=new unsigned char [len];
	fseek(fp,0,SEEK_SET);
	fread(buf,len,1,fp);
	fclose(fp);
	struct stHeader{
		unsigned char magic[4];
		unsigned int version;
		unsigned int offset_key;
		unsigned int offset_data;
		unsigned int count;
	};
	auto *h=(stHeader *)&buf[0];
	for(unsigned int i=0;i<h->count;i++){
		struct stEntry{
			unsigned short offset_key;
			unsigned short data_fmt;
			unsigned int data_len;
			unsigned int data_max_len;
			unsigned int data_offset;
		};
		auto *e=(stEntry *)&buf[sizeof(stHeader)+sizeof(stEntry)*i];
		clNode *n=m_node.AddChildLast((char *)&buf[h->offset_key+e->offset_key]);
		if(e->data_fmt==0x404){
			n->SetValueInt(*(int *)&buf[h->offset_data+e->data_offset]);
		}else if(e->data_fmt==0x204){
			n->SetValueString((char *)&buf[h->offset_data+e->data_offset]);
		}
	}
	delete [] buf;
	return true;
}

bool clSFO::Save(const char *path){
	struct stHeader{
		unsigned char magic[4];
		unsigned int version;
		unsigned int offset_key;
		unsigned int offset_data;
		unsigned int count;
	};
	struct stEntry{
		unsigned short offset_key;
		unsigned short data_fmt;
		unsigned int data_len;
		unsigned int data_max_len;
		unsigned int data_offset;
	};
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	const unsigned char padding[]={0,0,0,0};
	stHeader h={'\0','P','S','F',0x101,0,0,m_node.GetChildCount()};
	h.offset_key=h.offset_data=sizeof(stHeader)+sizeof(stEntry)*h.count;
	for(clNode *n=m_node.GetChild();!n->IsNull();n=n->GetNext()){
		h.offset_data+=strlen(n->GetKey())+1;
	}
	h.offset_data=Aligned(h.offset_data,4);
	fwrite(&h,sizeof(h),1,fp);
	unsigned int offset_key=0,offset_data=0;
	for(clNode *n=m_node.GetChild();!n->IsNull();n=n->GetNext()){
		stEntry e={(unsigned short)offset_key,0,0,0,offset_data};
		offset_key+=strlen(n->GetKey())+1;
		if(n->IsInt()){
			e.data_fmt=0x404;
			e.data_len=4;
		}else if(n->IsString()){
			e.data_fmt=0x204;
			e.data_len=strlen(n->GetValueString());
		}
		e.data_max_len=Align(e.data_len,4);
		offset_data+=e.data_max_len;
		fwrite(&e,sizeof(e),1,fp);
	}
	for(clNode *n=m_node.GetChild();!n->IsNull();n=n->GetNext()){
		fputs(n->GetKey(),fp);fputc(0,fp);
	}
	fwrite(padding,1,Align(ftell(fp),4),fp);
	for(clNode *n=m_node.GetChild();!n->IsNull();n=n->GetNext()){
		if(n->IsInt()){
			int v=n->GetValueInt();fwrite(&v,sizeof(v),1,fp);
		}else if(n->IsString()){
			fputs(n->GetValueString(),fp);fputc(0,fp);
		}
		fwrite(padding,1,Align(ftell(fp),4),fp);
	}
	fclose(fp);
	return true;
}

const clNode *clSFO::GetNode(void){return &m_node;}
