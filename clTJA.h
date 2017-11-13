#pragma once

#include <stdio.h>
#include "clNode.h"

class clTJA{
public:
	clTJA();
	bool Save(clNode *s,clNode *m,const char *dir);
private:
	clNode m_root;
	bool SaveCource(FILE *fp,clNode *fumen,int cource,int level);
};
