#pragma once

#include "clNode.h"

class clSFO{
public:
	clSFO();
	bool Load(const char *path);
	bool Save(const char *path);
	const clNode *GetNode(void);
private:
	clNode m_node;
};
