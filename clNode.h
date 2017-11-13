#pragma once

#include <stdio.h>

class clNode{
public:
	clNode(const char *key=nullptr);
	~clNode();
	bool IsNull(void);
	bool IsBool(void);
	bool IsInt(void);
	bool IsFloat(void);
	bool IsString(void);
	clNode *AddPrev(const char *key=nullptr);
	clNode *AddNext(const char *key=nullptr);
	clNode *AddChildFirst(const char *key=nullptr);
	clNode *AddChildLast(const char *key=nullptr);
	void DeleteChild(void);
	clNode *GetNull(void);
	clNode *GetPrev(const char *key=nullptr);
	clNode *GetNext(const char *key=nullptr);
	clNode *GetParent(void);
	clNode *GetChild(const char *key=nullptr);
	clNode *GetChild(unsigned int index);
	clNode *GetChildFirst(unsigned int index=0);
	clNode *GetChildLast(void);
	unsigned int GetChildCount(void);
	const char *GetKey(void);
	bool GetValueBool(void);
	int GetValueInt(void);
	float GetValueFloat(void);
	const char *GetValueString(void);
	void SetKey(const char *key);
	void SetValueBool(bool value);
	void SetValueInt(int value);
	void SetValueFloat(float value);
	void SetValueString(const char *value);
	bool CreateJson(const char *path);
	void CreateJson(FILE *fp,int tab=0);
private:
	clNode *m_prev,*m_next,*m_parent,*m_child_first,*m_child_last;
	enum{
		TYPE_NULL,
		TYPE_NONE,
		TYPE_BOOL,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING,
	}m_type;
	char *m_key;
	union{
		bool m_value_bool;
		int m_value_int;
		float m_value_float;
		char *m_value_string;
	};
};
