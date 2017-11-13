#pragma once

#include "clNode.h"

class clTaiko{
public:
	clTaiko();
	bool Load(const char *path);
	//bool Save_V(const char *dir);
	//bool Save_DX(const char *dir);
	bool CreateJson(const char *path);
	clNode *Get(void){return &m_root;}
	clNode *GetPaths(void){return &m_paths;}
private:
	bool Load_V_SongInfo(const char *path);
	bool Load_V_MusicInfo(const char *path);
	bool Load_V_Fumen(const char *path,clNode *fumen);
	bool Load_DX_MusicInfo(const char *path);
	bool Load_DX_Fumen(const char *path,clNode *fumen);
	bool Load_Fumen(unsigned char *d,clNode *fumen);
	clNode m_root;
	clNode m_paths;

	//bool SaveAddonInfo(const char *path);
	//bool SaveSongInfo(const char *path);
	//bool SaveMusicInfo(const char *path);
	//bool SaveFumen(const char *path,int course);
};
