
#include "clNode.h"
#include <string.h>

clNode::clNode(const char *key):m_prev(nullptr),m_next(nullptr),m_parent(nullptr),m_child_first(nullptr),m_child_last(nullptr),m_type(TYPE_NONE),m_key(nullptr){
	if(key){int len=strlen(key);m_key=new char [len+1];if(m_key)strcpy_s(m_key,len+1,key);}
}
clNode::~clNode(){
	DeleteChild();
	if(m_key)delete [] m_key;
	if(m_type==TYPE_STRING&&m_value_string)delete [] m_value_string;
	if(m_prev)m_prev->m_next=m_next;else if(m_parent)m_parent->m_child_first=m_next;
	if(m_next)m_next->m_prev=m_prev;else if(m_parent)m_parent->m_child_last=m_prev;
}
bool clNode::IsNull(void){return m_type==TYPE_NULL;}
bool clNode::IsBool(void){return m_type==TYPE_BOOL;}
bool clNode::IsInt(void){return m_type==TYPE_INT;}
bool clNode::IsFloat(void){return m_type==TYPE_FLOAT;}
bool clNode::IsString(void){return m_type==TYPE_STRING;}
clNode *clNode::AddPrev(const char *key){
	clNode *node=m_parent?new clNode(key):nullptr;
	if(node){
		if(m_prev)node->m_prev=m_prev;else if(m_parent)m_parent->m_child_first=node;
		node->m_next=this;
		node->m_parent=m_parent;
	}
	return node?node:GetNull();
}
clNode *clNode::AddNext(const char *key){
	clNode *node=m_parent?new clNode(key):nullptr;
	if(node){
		node->m_prev=this;
		if(m_next)node->m_next=m_next;else if(m_parent)m_parent->m_child_first=node;
		node->m_parent=m_parent;
	}
	return node?node:GetNull();
}
clNode *clNode::AddChildFirst(const char *key){
	clNode *node=new clNode(key);
	if(node){
		if(m_child_first){node->m_next=m_child_first;m_child_first->m_prev=node;}else m_child_last=node;
		node->m_parent=this;
		m_child_first=node;
	}
	return node?node:GetNull();
}
clNode *clNode::AddChildLast(const char *key){
	clNode *node=new clNode(key);
	if(node){
		if(m_child_last){node->m_prev=m_child_last;m_child_last->m_next=node;}else m_child_first=node;
		node->m_parent=this;
		m_child_last=node;
	}
	return node?node:GetNull();
}
void clNode::DeleteChild(void){
	for(clNode *c=m_child_first,*n;c;c=n){
		n=c->m_next;
		delete c;
	}
}
clNode *clNode::GetNull(void){static clNode null(nullptr);null.m_type=TYPE_NULL;return &null;}
clNode *clNode::GetPrev(const char *key){
	if(!key)return m_prev?m_prev:GetNull();
	clNode *c=m_prev;for(;c;c=c->m_prev)if(c->m_key&&strcmp(c->m_key,key)==0)break;
	return c?c:GetNull();
}
clNode *clNode::GetNext(const char *key){
	if(!key)return m_next?m_next:GetNull();
	clNode *c=m_next;for(;c;c=c->m_next)if(c->m_key&&strcmp(c->m_key,key)==0)break;
	return c?c:GetNull();
}
clNode *clNode::GetParent(void){return m_parent?m_parent:GetNull();}
clNode *clNode::GetChild(const char *key){
	if(!key)return m_child_first?m_child_first:GetNull();
	clNode *c=m_child_first;for(;c;c=c->m_next)if(c->m_key&&strcmp(c->m_key,key)==0)break;
	return c?c:GetNull();
}
clNode *clNode::GetChild(unsigned int index){return GetChildFirst(index);}
clNode *clNode::GetChildFirst(unsigned int index){
	clNode *c=m_child_first;for(;c&&index;c=c->m_next)index--;
	return c?c:GetNull();
}
clNode *clNode::GetChildLast(void){return m_child_last?m_child_last:GetNull();}
unsigned int clNode::GetChildCount(void){unsigned int count=0;for(clNode *c=m_child_first;c;c=c->m_next)count++;return count;}
const char *clNode::GetKey(void){return m_key?m_key:"";}
bool clNode::GetValueBool(void){return m_type==TYPE_BOOL?m_value_bool:false;}
int clNode::GetValueInt(void){return m_type==TYPE_INT?m_value_int:0;}
float clNode::GetValueFloat(void){return m_type==TYPE_FLOAT?m_value_float:0;}
const char *clNode::GetValueString(void){return (m_type==TYPE_STRING&&m_value_string)?m_value_string:"";}
void clNode::SetKey(const char *key){
	if(m_key)delete [] m_key;
	m_key=nullptr;
	if(key){int len=strlen(key);m_key=new char [len+1];if(m_key)strcpy_s(m_key,len+1,key);}
}
void clNode::SetValueBool(bool value){
	if(m_type==TYPE_STRING&&m_value_string)delete [] m_value_string;
	m_type=TYPE_BOOL;
	m_value_bool=value;
}
void clNode::SetValueInt(int value){
	if(m_type==TYPE_STRING&&m_value_string)delete [] m_value_string;
	m_type=TYPE_INT;
	m_value_int=value;
}
void clNode::SetValueFloat(float value){
	if(m_type==TYPE_STRING&&m_value_string)delete [] m_value_string;
	m_type=TYPE_FLOAT;
	m_value_float=value;
}
void clNode::SetValueString(const char *value){
	if(m_type==TYPE_STRING&&m_value_string)delete [] m_value_string;
	m_type=TYPE_STRING;
	m_value_string=nullptr;
	if(value){
		int len=strlen(value);
		m_value_string=new char [len+1];
		if(m_value_string)strcpy_s(m_value_string,len+1,value);
	}
}
bool clNode::CreateJson(const char *path){
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	CreateJson(fp);
	fclose(fp);
	return true;
}
void clNode::CreateJson(FILE *fp,int tab){
	if(m_type!=TYPE_NULL){
		if(m_key&&m_key[0]){for(int i=0;i<tab;i++)fputs("  ",fp);fprintf(fp,"\"%s\": ",m_key);}
		switch(m_type){
		case TYPE_NONE:
			if(m_child_first){
				if(!(m_key&&m_key[0]||m_prev))for(int i=0;i<tab;i++)fputs("  ",fp);
				if(m_child_first->m_key&&m_child_first->m_key[0])fputs("{\r\n",fp);else fputs("[\r\n",fp);
				for(clNode *c=m_child_first;c;c=c->m_next)c->CreateJson(fp,tab+1);
				for(int i=0;i<tab;i++)fputs("  ",fp);
				if(m_child_first->m_key&&m_child_first->m_key[0])fputc('}',fp);else fputc(']',fp);
			}else{
				fputs("[]",fp);
			}
			break;
		case TYPE_BOOL:fprintf(fp,"%s",m_value_bool?"true":"false");break;
		case TYPE_INT:fprintf(fp,"%d",m_value_int);break;
		case TYPE_FLOAT:{
			char buf[64];sprintf_s(buf,sizeof(buf),"%.24f",m_value_float);
			for(int i=strlen(buf)-1;i>=0&&(buf[i]=='0'||buf[i]=='.');i--){if(buf[i]=='.'){buf[i]='\0';break;}buf[i]='\0';}
			fputs(buf,fp);
		}break;
		case TYPE_STRING:fprintf(fp,"\"%s\"",m_value_string?m_value_string:"");break;
		}
		if(m_next)fputc(',',fp);
		if((m_key&&m_key[0])||!m_next)fputs("\r\n",fp);
	}
}
