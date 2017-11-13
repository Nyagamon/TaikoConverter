
#include "clTaiko.h"
#include "util.h"
#include "compress.h"

clTaiko::clTaiko():m_root(),m_paths(){}

bool clTaiko::Load(const char *path){
	char buf[0x200],dir[0x200];bool v=true;
	sprintf_s(dir,_countof(dir),"%s\\_data",path);
	sprintf_s(buf,_countof(buf),"%s\\system\\SongInfo.dat",dir);if(!ExistFile(buf)){
		sprintf_s(dir,_countof(dir),"%s",path);
		sprintf_s(buf,_countof(buf),"%s\\system\\SongInfo.dat",dir);if(!ExistFile(buf)){
			GetDirectory(dir,_countof(dir),path);
			sprintf_s(buf,_countof(buf),"%s\\system\\SongInfo.dat",dir);if(!ExistFile(buf)){
				v=false;
				sprintf_s(dir,_countof(dir),"%s",path);
				sprintf_s(buf,_countof(buf),"%s\\MUSIC_INFO.EDAT",dir);if(!ExistFile(buf)){
					GetDirectory(dir,_countof(dir),path);
					sprintf_s(buf,_countof(buf),"%s\\MUSIC_INFO.EDAT",dir);if(!ExistFile(buf)){
						return false;
					}
				}
			}
		}
	}
	if(v){
		m_root.AddChildLast("type")->SetValueString("v");
		sprintf_s(buf,_countof(buf),"%s\\system\\SongInfo.dat",dir);Load_V_SongInfo(buf);
		sprintf_s(buf,_countof(buf),"%s\\system\\MusicInfo.dat",dir);Load_V_MusicInfo(buf);
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\select_main.anm",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\select_main.txp",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\select_main.anm",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\select_main.anm",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\game.anm",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		//sprintf_s(buf,_countof(buf),"%s\\_data\\system\\songtitle\\game.txp",path);LoadSongInfo(buf,m_dlc_v.AddChildLast("addon_info"));
		for(clNode *s=m_root.GetChild("songs")->GetChild();!s->IsNull();s=s->GetNext()){
			const char *dirname=s->GetChild("dirname")->GetValueString();
			const char *fumenname=s->GetChild("fumenname")->GetValueString();
			sprintf_s(buf,_countof(buf),"%s\\fumen\\%s\\solo\\%s_e.bin",dir,dirname,fumenname);if(ExistFile(buf))Load_V_Fumen(buf,s->AddChildLast("fumen_easy"));
			sprintf_s(buf,_countof(buf),"%s\\fumen\\%s\\solo\\%s_n.bin",dir,dirname,fumenname);if(ExistFile(buf))Load_V_Fumen(buf,s->AddChildLast("fumen_normal"));
			sprintf_s(buf,_countof(buf),"%s\\fumen\\%s\\solo\\%s_h.bin",dir,dirname,fumenname);if(ExistFile(buf))Load_V_Fumen(buf,s->AddChildLast("fumen_hard"));
			sprintf_s(buf,_countof(buf),"%s\\fumen\\%s\\solo\\%s_m.bin",dir,dirname,fumenname);if(ExistFile(buf))Load_V_Fumen(buf,s->AddChildLast("fumen_master"));
		}
		for(clNode *m=m_root.GetChild("musics")->GetChild();!m->IsNull();m=m->GetNext()){
			sprintf_s(buf,_countof(buf),"%s\\sound\\song\\%s.nus3bank",dir,m->GetChild("songname")->GetValueString());
			m_paths.AddChildLast()->SetValueString(buf);
		}
	}else{
		m_root.AddChildLast("type")->SetValueString("dx");
		sprintf_s(buf,_countof(buf),"%s\\MUSIC_INFO.EDAT",dir);Load_DX_MusicInfo(buf);
		for(clNode *s=m_root.GetChild("songs")->GetChild();!s->IsNull();s=s->GetNext()){
			const char *dirname=s->GetChild("dirname")->GetValueString();
			sprintf_s(buf,_countof(buf),"%s\\%s\\%s_FUMEN_E.EDAT",dir,dirname,dirname);if(ExistFile(buf))Load_DX_Fumen(buf,s->AddChildLast("fumen_easy"));
			sprintf_s(buf,_countof(buf),"%s\\%s\\%s_FUMEN_N.EDAT",dir,dirname,dirname);if(ExistFile(buf))Load_DX_Fumen(buf,s->AddChildLast("fumen_normal"));
			sprintf_s(buf,_countof(buf),"%s\\%s\\%s_FUMEN_H.EDAT",dir,dirname,dirname);if(ExistFile(buf))Load_DX_Fumen(buf,s->AddChildLast("fumen_hard"));
			sprintf_s(buf,_countof(buf),"%s\\%s\\%s_FUMEN_M.EDAT",dir,dirname,dirname);if(ExistFile(buf))Load_DX_Fumen(buf,s->AddChildLast("fumen_master"));
			sprintf_s(buf,_countof(buf),"%s\\%s\\%s_SONG.EDAT",dir,dirname,dirname);
			m_paths.AddChildLast()->SetValueString(buf);
		}
		//SHUSHU_SONG_S.EDAT
		//SHUSHU_TEXPACK.EDAT
	}
	return true;
}

bool clTaiko::Load_V_SongInfo(const char *path){
	unsigned char *buf=LoadFile(path);if(!buf)return false;
	struct stHeader{
		unsigned int count,offset,r1,r2;
	};
	stHeader *h=(stHeader *)&buf[0];
	clNode *songs=m_root.AddChildLast("songs");
	for(unsigned int i=0;i<h->count;i++){
		clNode *song=songs->AddChildLast();
		struct stEntry{
			unsigned int offset_sl,offset_title,offset_songname,offset_dirname,offset_fumenname,offset_dirname2;
			unsigned int offset_enso_info;
			unsigned int unk1,unk2,unk3,unk4;
			signed short song_id,song_id_parent;
			unsigned char genre,unk5,unk6,unk7;
			unsigned char branch_easy,branch_normal,branch_hard,branch_master;
			unsigned char level_easy,level_normal,level_hard,level_master;
			unsigned char unk8,unk9,unk10,unk11;
		};
		stEntry *e=(stEntry *)&buf[h->offset+sizeof(stEntry)*i];
#ifdef _DEBUG
		//if(e->unk1)_asm int 3;//0x00001e5a
		//if(e->unk2)_asm int 3;//0x0000181a
		//if(e->unk3)_asm int 3;//0x00001036
		//if(e->unk4)_asm int 3;//0x00000a1e
		//if(e->unk5)_asm int 3;//1
		//if(e->unk6)_asm int 3;//0
		//if(e->unk7)_asm int 3;//1
		//if(e->unk8)_asm int 3;//0,1
		if(e->unk9)_asm int 3;
		if(e->unk10)_asm int 3;
		if(e->unk11)_asm int 3;
#endif
		song->AddChildLast("sl")->SetValueString(e->offset_sl?(char *)&buf[e->offset_sl]:"");
		song->AddChildLast("title")->SetValueString(e->offset_title?(char *)&buf[e->offset_title]:"");
		song->AddChildLast("songname")->SetValueString(e->offset_songname?(char *)&buf[e->offset_songname]:"");
		song->AddChildLast("dirname")->SetValueString(e->offset_dirname?(char *)&buf[e->offset_dirname]:"");
		song->AddChildLast("fumenname")->SetValueString(e->offset_fumenname?(char *)&buf[e->offset_fumenname]:"");
		song->AddChildLast("dirname2")->SetValueString(e->offset_dirname2?(char *)&buf[e->offset_dirname2]:"");
		//enso//@@@@@@@@@@@@
		song->AddChildLast("unk1")->SetValueInt(e->unk1);
		song->AddChildLast("unk2")->SetValueInt(e->unk2);
		song->AddChildLast("unk3")->SetValueInt(e->unk3);
		song->AddChildLast("unk4")->SetValueInt(e->unk4);
		song->AddChildLast("song_id")->SetValueInt(e->song_id);
		if(e->song_id_parent!=-1)song->AddChildLast("song_id_parent")->SetValueInt(e->song_id_parent);
		song->AddChildLast("genre")->SetValueInt(e->genre);
		song->AddChildLast("unk5")->SetValueInt(e->unk5);
		song->AddChildLast("unk6")->SetValueInt(e->unk6);
		song->AddChildLast("unk7")->SetValueInt(e->unk7);
		song->AddChildLast("branch_easy")->SetValueBool(e->branch_easy>0);
		song->AddChildLast("branch_normal")->SetValueBool(e->branch_normal>0);
		song->AddChildLast("branch_hard")->SetValueBool(e->branch_hard>0);
		song->AddChildLast("branch_master")->SetValueBool(e->branch_master>0);
		song->AddChildLast("level_easy")->SetValueInt(e->level_easy);
		song->AddChildLast("level_normal")->SetValueInt(e->level_normal);
		song->AddChildLast("level_hard")->SetValueInt(e->level_hard);
		song->AddChildLast("level_master")->SetValueInt(e->level_master);
		song->AddChildLast("unk8")->SetValueInt(e->unk8);
		if(e->unk9)song->AddChildLast("unk9")->SetValueInt(e->unk9);
		if(e->unk10)song->AddChildLast("unk10")->SetValueInt(e->unk10);
		if(e->unk11)song->AddChildLast("unk11")->SetValueInt(e->unk11);
	}
	delete [] buf;
	return true;
}

bool clTaiko::Load_V_MusicInfo(const char *path){
	unsigned char *buf=LoadFile(path);if(!buf)return false;
	struct stHeader{
		unsigned int count,offset,r1,r2;
	};
	stHeader *h=(stHeader *)&buf[0];
	clNode *musics=m_root.AddChildLast("musics");
	for(unsigned int i=0;i<h->count;i++){
		clNode *music=musics->AddChildLast();
		struct stEntry{
			unsigned int offset_songname;
			unsigned int demo_position;
			signed int unk1;
		};
		stEntry *e=(stEntry *)&buf[h->offset+sizeof(stEntry)*i];
#ifdef _DEBUG
		//if(e->unk1)_asm int 3;//3
#endif
		music->AddChildLast("songname")->SetValueString(e->offset_songname?(char *)&buf[e->offset_songname]:"");
		music->AddChildLast("demo_position")->SetValueInt(e->demo_position);
		music->AddChildLast("unk1")->SetValueInt(e->unk1);
	}
	delete [] buf;
	return true;
}

bool clTaiko::Load_V_Fumen(const char *path,clNode *fumen){
	size_t size;
	unsigned char *buf=LoadFile(path,&size);if(!(buf&&size>=4))return false;

	int d_len=Decompress(nullptr,0,&buf[4],size-4);
	unsigned char *d=new unsigned char [d_len];
	Decompress(d,d_len,&buf[4],size-4);
	delete [] buf;

	if(!Load_Fumen(d,fumen)){
		delete [] d;
		return false;
	}

	delete [] d;
	return true;
}

bool clTaiko::Load_DX_MusicInfo(const char *path){
	unsigned char *buf=LoadFile(path);if(!buf)return false;
	struct stInfo{
		int song_id;
		char direname[0x20];
		char content_id_dir[0x20];
		wchar_t title1[0x20];
		char title2[0x20];
		int genre;
		int unk1,unk2,unk3,unk4,unk5,unk6,unk7;
		int level_easy,level_normal,level_hard,level_master;
		char unk8[4];
		int unk9;//0
	};
	clNode *songs=m_root.AddChildLast("songs");
	clNode *musics=m_root.AddChildLast("musics");
	for(stInfo *i=(stInfo *)&buf[0];i->song_id!=-1;i++){
		clNode *song=songs->AddChildLast();
#ifdef _DEBUG
		//if(strcmp("content_id_dir")
		//if(e->unk1)_asm int 3;
		if(i->unk2!=-1)_asm int 3;
		//if(e->unk3)_asm int 3;
		//if(e->unk4)_asm int 3;
		//if(e->unk5)_asm int 3;
		//if(e->unk6)_asm int 3;
		//if(e->unk7)_asm int 3;
		if(i->unk8[1]!=0x20||i->unk8[2]!=0x20||i->unk8[3]!=0x20)_asm int 3;
		if(i->unk9)_asm int 3;
#endif
		char buf1[0x200];UTF16toUTF8(buf1,_countof(buf1),i->title1);
		//song->AddChildLast("sl")->SetValueString("");
		song->AddChildLast("title")->SetValueString(buf1);
		song->AddChildLast("songname")->SetValueString(i->direname);
		song->AddChildLast("dirname")->SetValueString(i->direname);
		song->AddChildLast("fumenname")->SetValueString(i->direname);
		//song->AddChildLast("dirname2")->SetValueString(i->direname);
		song->AddChildLast("song_id")->SetValueInt(i->song_id);
		song->AddChildLast("genre")->SetValueInt(i->genre);
		song->AddChildLast("unk1")->SetValueInt(i->unk1);
		song->AddChildLast("unk2")->SetValueInt(i->unk2);
		song->AddChildLast("unk3")->SetValueInt(i->unk3);
		song->AddChildLast("unk4")->SetValueInt(i->unk4);
		song->AddChildLast("unk5")->SetValueInt(i->unk5);
		song->AddChildLast("unk6")->SetValueInt(i->unk6);
		song->AddChildLast("unk7")->SetValueInt(i->unk7);
		//song->AddChildLast("branch_easy")->SetValueBool(false);
		//song->AddChildLast("branch_normal")->SetValueBool(false);
		//song->AddChildLast("branch_hard")->SetValueBool(false);
		//song->AddChildLast("branch_master")->SetValueBool(false);
		song->AddChildLast("level_easy")->SetValueInt(i->level_easy);
		song->AddChildLast("level_normal")->SetValueInt(i->level_normal);
		song->AddChildLast("level_hard")->SetValueInt(i->level_hard);
		song->AddChildLast("level_master")->SetValueInt(i->level_master);
		//song->AddChildLast("unk8")->SetValueInt(e->unk8);
		if(i->unk9)song->AddChildLast("unk9")->SetValueInt(i->unk9);
		clNode *music=musics->AddChildLast();
		music->AddChildLast("songname")->SetValueString(i->direname);
		//music->AddChildLast("demo_position")->SetValueInt(e->demo_position);
	}
	delete [] buf;
	return true;
}

bool clTaiko::Load_DX_Fumen(const char *path,clNode *fumen){
	unsigned char *buf=LoadFile(path);if(!buf)return false;
	if(!Load_Fumen(buf,fumen)){
		delete [] buf;
		return false;
	}
	delete [] buf;
	return true;
}

bool clTaiko::Load_Fumen(unsigned char *d,clNode *fumen){
	struct stH{
		unsigned int unk1;
		unsigned int gauge_max;
		unsigned int gauge_quota;
		signed int gauge_up[3];
		unsigned int unk2[3];
		unsigned int unk3[10];
		unsigned int unk4;
		unsigned int count;
		unsigned int unk5;
	};
	struct stB{
		float bpm;
		float position;
		unsigned char fever;
		unsigned char line;
		unsigned char unk2;
		unsigned char unk3;
		signed int unk4[6];
		unsigned int unk5;
	};
	struct stO{
		unsigned short count;
		unsigned short unk1;
		float speed;
	};
	struct stN{
		unsigned int type;
		float position;
		unsigned int unk1;
		unsigned int unk2;
		unsigned short score;//ふうせん と くすだま のときは回数
		unsigned short unk3;
		float length;
	};
	unsigned int a=0x1B0;
	stH *h=(stH *)&d[a];a+=sizeof(*h);
#ifdef _DEBUG
	//if(h->unk1)_asm int 3;// 0x390100 コードアドレス？
	if(h->unk2[0]!=0x10000)_asm int 3;
	//if(h->unk2[1]!=0x10000)_asm int 3;//0x10000,0xFA10
	//if(h->unk2[2]!=0x10000)_asm int 3;//0x10000,0xFA10
	if(h->unk3[0]!=20)_asm int 3;
	if(h->unk3[1]!=10)_asm int 3;
	if(h->unk3[2]!=0)_asm int 3;
	if(h->unk3[3]!=1)_asm int 3;
	if(h->unk3[4]!=20)_asm int 3;
	if(h->unk3[5]!=10)_asm int 3;
	if(h->unk3[6]>1)_asm int 3;//0(1)
	if(h->unk3[7]!=30)_asm int 3;
	if(h->unk3[8]!=30)_asm int 3;
	if(h->unk3[9]!=0)_asm int 3;
	//if(h->unk4)_asm int 3;//0xEF8A8 最大スコア？
	//if(h->unk5)_asm int 3;//0
#endif
	if(h->unk1)fumen->AddChildLast("unk1")->SetValueInt(h->unk1);
	fumen->AddChildLast("gauge_max")->SetValueInt(h->gauge_max);
	fumen->AddChildLast("gauge_quota")->SetValueInt(h->gauge_quota);
	fumen->AddChildLast("gauge_up1")->SetValueInt(h->gauge_up[0]);
	fumen->AddChildLast("gauge_up2")->SetValueInt(h->gauge_up[1]);
	fumen->AddChildLast("gauge_up3")->SetValueInt(h->gauge_up[2]);
	fumen->AddChildLast("unk2")->SetValueInt(h->unk2[0]);
	fumen->AddChildLast("unk3")->SetValueInt(h->unk2[1]);
	fumen->AddChildLast("unk4")->SetValueInt(h->unk2[2]);
	fumen->AddChildLast("unk5")->SetValueInt(h->unk3[0]);
	fumen->AddChildLast("unk6")->SetValueInt(h->unk3[1]);
	fumen->AddChildLast("unk7")->SetValueInt(h->unk3[2]);
	fumen->AddChildLast("unk8")->SetValueInt(h->unk3[3]);
	fumen->AddChildLast("unk9")->SetValueInt(h->unk3[4]);
	fumen->AddChildLast("unk10")->SetValueInt(h->unk3[5]);
	fumen->AddChildLast("unk11")->SetValueInt(h->unk3[6]);
	fumen->AddChildLast("unk12")->SetValueInt(h->unk3[7]);
	fumen->AddChildLast("unk13")->SetValueInt(h->unk3[8]);
	fumen->AddChildLast("unk14")->SetValueInt(h->unk3[9]);
	if(h->unk4)fumen->AddChildLast("unk15")->SetValueInt(h->unk4);
	if(h->unk5)fumen->AddChildLast("unk16")->SetValueInt(h->unk5);
	clNode *bars=fumen->AddChildLast("bars");
	for(unsigned int i=0;i<h->count;i++){
		clNode *bar=bars->AddChildLast();
		stB *b=(stB *)&d[a];a+=sizeof(*b);
#ifdef _DEBUG
		//if(b->unk2)_asm int 3;//0,0x75
		//if(b->unk3)_asm int 3;//00,0x61
		//if(b->unk4[0]!=-1)_asm int 3;//0x340
		//if(b->unk4[1]!=-1)_asm int 3;//0x3DC
		//if(b->unk4[2]!=-1)_asm int 3;
		//if(b->unk4[3]!=-1)_asm int 3;
		//if(b->unk4[4]!=-1)_asm int 3;
		//if(b->unk4[5]!=-1)_asm int 3;
		//if(b->unk5)_asm int 3;
#endif
		bar->AddChildLast("bpm")->SetValueFloat(b->bpm);
		bar->AddChildLast("position")->SetValueFloat(b->position);
		bar->AddChildLast("fever")->SetValueBool(b->fever>0);
		bar->AddChildLast("line")->SetValueBool(b->line>0);
		bar->AddChildLast("unk1")->SetValueInt(b->unk2);
		bar->AddChildLast("unk2")->SetValueInt(b->unk3);
		if(b->unk4[0]!=-1)bar->AddChildLast("branch_1")->SetValueInt(b->unk4[0]);
		if(b->unk4[1]!=-1)bar->AddChildLast("branch_2")->SetValueInt(b->unk4[1]);
		if(b->unk4[2]!=-1)bar->AddChildLast("branch_3")->SetValueInt(b->unk4[2]);
		if(b->unk4[3]!=-1)bar->AddChildLast("branch_4")->SetValueInt(b->unk4[3]);
		if(b->unk4[4]!=-1)bar->AddChildLast("branch_5")->SetValueInt(b->unk4[4]);
		if(b->unk4[5]!=-1)bar->AddChildLast("branch_6")->SetValueInt(b->unk4[5]);
		if(b->unk5)bar->AddChildLast("unk3")->SetValueInt(b->unk5);
		clNode *lines=bar->AddChildLast("lines");
		for(unsigned int j=0;j<3;j++){
			clNode *line=lines->AddChildLast();
			stO *o=(stO *)&d[a];a+=sizeof(*o);
#ifdef _DEBUG
			//if(o->unk1)_asm int 3;//0,17
#endif
			line->AddChildLast("unk1")->SetValueInt(o->unk1);
			line->AddChildLast("speed")->SetValueFloat(o->speed);
			clNode *notes=line->AddChildLast("notes");
			for(unsigned int k=0;k<o->count;k++){
				clNode *note=notes->AddChildLast();
				stN *n=(stN *)&d[a];a+=sizeof(*n);
#ifdef _DEBUG
				//"0","1:ドン","2:ド","3:ドン3回目 (ド コ ドン)","4:カッ","5:カ","6:連打","7:ドン(大)","8:カッ(大)","9:連打(大)","10:ふうせん","11:ドン(大)","12:くすだま","13:カッ(大)"};
				//if(n->unk1)_asm int 3;//0,1
				//if(n->unk2)_asm int 3;
				//if(n->unk3)_asm int 3;
#endif
				note->AddChildLast("type")->SetValueInt(n->type);
				note->AddChildLast("position")->SetValueFloat(n->position);
				if(n->unk1)note->AddChildLast("unk1")->SetValueInt(n->unk1);
				if(n->unk2)note->AddChildLast("unk2")->SetValueInt(n->unk2);
				note->AddChildLast("score")->SetValueInt(n->score);
				note->AddChildLast("unk3")->SetValueInt(n->unk3);
				note->AddChildLast("length")->SetValueFloat(n->length);
				if(n->type==6||n->type==9){
					unsigned int unk;
#ifdef _DEBUG
					unk=*(unsigned int *)&d[a];a+=sizeof(unk);if(unk)_asm int 3;
					unk=*(unsigned int *)&d[a];a+=sizeof(unk);if(unk)_asm int 3;
#else
					unk=*(unsigned int *)&d[a];a+=sizeof(unk);if(unk)note->AddChildLast("unk4")->SetValueInt(unk);
					unk=*(unsigned int *)&d[a];a+=sizeof(unk);if(unk)note->AddChildLast("unk5")->SetValueInt(unk);
#endif
				}
			}
		}
	}
	return true;
}




/*
bool clTaiko::SaveDLV_V(const char *dir,const char *path_bin){
	if(m_tja.GetChild()->IsNull())return false;

	char buf[0x200];
	clNode sfo(nullptr);
	sfo.AddChildLast("ATTRIBUTE")->SetValueInt(0);
	sfo.AddChildLast("CATEGORY")->SetValueString("ac");
	sfo.AddChildLast("CONTENT_ID")->SetValueString("PCSG00551-DLC");
	sfo.AddChildLast("PUBTOOLINFO")->SetValueString("c_date=20150610");
	sfo.AddChildLast("TITLE")->SetValueString("");
	sfo.AddChildLast("TITLE_00")->SetValueString("");
	sfo.AddChildLast("TITLE_10")->SetValueString("");
	sfo.AddChildLast("TITLE_ID")->SetValueString("PCSG00551");
	sfo.AddChildLast("VERSION")->SetValueString("01.00");
	sprintf_s(buf,_countof(buf),"%s\\sce_sys\\param.sfo",dir);MakeDirectory(buf);SaveSFO(buf,&sfo);
	sprintf_s(buf,_countof(buf),"%s\\_data\\addoninfo.dat",dir);MakeDirectory(buf);SaveAddonInfo(buf);
	sprintf_s(buf,_countof(buf),"%s\\_data\\system\\SongInfo.dat",dir);MakeDirectory(buf);SaveSongInfo(buf);
	sprintf_s(buf,_countof(buf),"%s\\_data\\system\\MusicInfo.dat",dir);MakeDirectory(buf);SaveMusicInfo(buf);
	//songtitle\\select_main.anm
	//songtitle\\select_main.txp
	////songtitle\\select_sub.anm
	////songtitle\\select_sub.txp
	//songtitle\\game.anm
	//songtitle\\game.txp
	sprintf_s(buf,_countof(buf),"%s\\_data\\fumen\\fumen\\solo\\fumen_e.bin",dir);MakeDirectory(buf);SaveFumen(buf,0);
	sprintf_s(buf,_countof(buf),"%s\\_data\\fumen\\fumen\\solo\\fumen_n.bin",dir);MakeDirectory(buf);SaveFumen(buf,1);
	sprintf_s(buf,_countof(buf),"%s\\_data\\fumen\\fumen\\solo\\fumen_h.bin",dir);MakeDirectory(buf);SaveFumen(buf,2);
	sprintf_s(buf,_countof(buf),"%s\\_data\\fumen\\fumen\\solo\\fumen_m.bin",dir);MakeDirectory(buf);SaveFumen(buf,3);
	//sprintf_s(buf,_countof(buf),"%s\\_data\\fumen\\fumen\\solo\\ex_fumen_m.bin",path);MakeDirectory(buf);SaveFumen(buf);
	char path[0x200];
	sprintf_s(path,_countof(path),"%s\\%s",dir,m_tja.GetChild("WAVE")->GetValueString());
	sprintf_s(buf,_countof(buf),"%s\\_data\\sound\\song\\song.nus3bank",dir);MakeDirectory(buf);
	if(_strrcmpi(path,".ogg")==0){
		char path2[0x200];
		sprintf_s(path2,_countof(path2),"%s\\tmp.wav",path_bin);
		ConvertOGGtoWAVE(path,path2,path_bin);
		ConvertWAVEtoNUS3(path2,buf,path_bin);
		remove(path2);
	}else{
		ConvertWAVEtoNUS3(path,buf,path_bin);
	}
	//enso\\don_bg

	return true;
}

bool clTaiko::SaveAddonInfo(const char *path){
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	struct stHeader{
		unsigned int type,r1,r2,r3;
	}h={4,0,0,0};fwrite(&h,sizeof(h),1,fp);
	fclose(fp);
	return true;
}
bool clTaiko::SaveSongInfo(const char *path){
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	const unsigned char padding[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	struct stHeader{
		unsigned int count,offset,r1,r2;
	}h={1,sizeof(h),0,0};fseek(fp,sizeof(h),SEEK_CUR);
	struct stEntry{
		unsigned int offset_sl,offset_title,offset_songname,offset_dirname,offset_fumenname,offset_dirname2;
		unsigned int offset_enso_info;
		unsigned int unk1,unk2,unk3,unk4;
		signed short song_id,song_id_parent;
		unsigned char genre,unk5,unk6,unk7;
		unsigned char branch_easy,branch_normal,branch_hard,branch_master;
		unsigned char level_easy,level_normal,level_hard,level_master;
		unsigned char unk8,unk9,unk10,unk11;
	}e={
		0,0,0,0,0,0,0,
		0,0,0,0,1000,-1,//@@@ unk1,unk2,unk3,unk4 ソングID @@@
		0,1,0,1,//@@@ unk5,unk6,unk7 @@@
		0,0,0,0,//@@@分岐
		0,0,0,0,
		0,0,0,0,//@@@ unk8,unk9,unk10,unk11 @@@
	};fseek(fp,sizeof(e),SEEK_CUR);
	char buf[0x200];
#define SONGINFO_STR(offset,str) {(offset)=ftell(fp);fputs(SJIStoUTF8(buf,_countof(buf),(str)),fp);fputc(0,fp);fwrite(padding,1,align(ftell(fp),16),fp);}
	//SONGINFO_STR(e.offset_sl,"SL");
	SONGINFO_STR(e.offset_title,m_tja.GetChild("TITLE")->GetValueString());
	SONGINFO_STR(e.offset_songname,"song");
	SONGINFO_STR(e.offset_dirname,"fumen");
	e.offset_fumenname=e.offset_dirname;//SONGINFO_STR(e.offset_fumenname,"fumen");
	e.offset_dirname2=e.offset_dirname;//SONGINFO_STR(e.offset_dirname2,"fumen");
																		 //info[i].r1=7380;//@@@@@@@@@@
																		 //info[i].r2=5570;//@@@@@@@@@@
																		 //info[i].r3=3120;//@@@@@@@@@@
																		 //info[i].r4=2030;//@@@@@@@@@@
	for(clNode *n=m_tja.GetChild("COURSE");!n->IsNull();n=n->GetNext("COURSE")){
		int level=n->GetNext("LEVEL")->GetValueInt();
		switch(n->GetValueInt()){
		case 0:e.level_easy=(unsigned char)level;break;
		case 1:e.level_normal=(unsigned char)level;break;
		case 2:e.level_hard=(unsigned char)level;break;
		case 3:e.level_master=(unsigned char)level;break;
		}
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&h,sizeof(h),1,fp);
	fwrite(&e,sizeof(e),1,fp);
	fclose(fp);
	return true;
}
bool clTaiko::SaveMusicInfo(const char *path){
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	const unsigned char padding[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	struct stHeader{
		unsigned int count,offset,r1,r2;
	}h={1,sizeof(h),0,0};fwrite(&h,sizeof(h),1,fp);
	struct stEntry{
		unsigned int offset_songname;
		unsigned int demo_position;
		signed int unk1;
	}e={
		aligned(sizeof(stHeader)+sizeof(stEntry)*h.count,16),
		(unsigned int)(m_tja.GetChild("DEMOSTART")->GetValueFloat()*1000),
		0,
	};fwrite(&e,sizeof(e),1,fp);fwrite(padding,1,align(ftell(fp),16),fp);
	fputs("song",fp);fputc(0,fp);
	fwrite(padding,1,align(ftell(fp),16),fp);
	fclose(fp);
	return true;
}
bool clTaiko::SaveFumen(const char *path,int course){
	clNode *c;for(c=m_tja.GetChild("COURSE");!c->IsNull()&&c->GetValueInt()!=course;c=c->GetNext("COURSE"));
	if(c->IsNull())return false;
	FILE *fp;if(fopen_s(&fp,path,"wb"))return false;
	fseek(fp,0x1B0,SEEK_CUR);
	struct stH{
		unsigned int unk1;
		unsigned int gauge_max;
		unsigned int gauge_quota;
		signed int gauge_up[3];
		unsigned int unk2[3];
		unsigned int unk3[10];
		unsigned int unk4;
		unsigned int count;
		unsigned int unk5;
	}h={
		0,10000,7000,100,50,-50,0x10000,0x10000,0x10000,
		20,10,0,1,20,10,1,30,30,0,0,0,0,
	};
	float bpm=0;
	float position=0;
	bool gogo=false;
	bool line=true;
	float speed=1;
	clNode *n;
	for(n=m_tja.GetChild();!n->IsNull()&&n->GetKey()[0]!='#';n=n->GetNext()){
		if(strcmp(n->GetKey(),"BPM")==0)bpm=n->GetValueFloat();
		//else if(strcmp(n->GetKey(),"BALLOON")==0)node->SetValueString(value);
		//  BALLOON: ゲキ連打およびイモ連打の打数。風船の数だけ ,（カンマ）で区切って指定。
		//  数字と数字の間に改行は含めないでください。
		//  省略した場合は一律5打。
		//else if(strcmp(s,"SCOREINIT")==0)node->SetValueString(value);
		//  :	10コンボ未満の時に小音符を良判定で叩いた時の得点を指定できます。
		//  省略した場合は自動で計算されます。
		//  得点方式がstableの時の配点は、550, 800 のようにカンマを入れた後に入力。
		//else if(strcmp(s,"SCOREDIFF")==0)node->SetValueString(value);
		//  :	10コンボごとに加算される一打あたりの点数を指定します。
		//  省略した場合は自動で計算されます。
		//  値の最後にアルファベットのdを入れると、1打あたりの点数の加算は200コンボを越えたときの1回のみになります。
		//  SCOREINIT:1000、SCOREDIFF:1000d と入力するとドンだフルコースの配点（大音符は得点2倍）になります。
		//  またゴーゴータイムは1.2倍の得点として扱われます。
		else if(strcmp(n->GetKey(),"COURSE")==0)if(c)n=c;else n=n->GetNull();
		//else if(strcmp(s,"STYLE")==0){
		//	if(strcmp(value,"Single")==0)node->SetValueInt(1);
		//	else if(strcmp(value,"Double")==0)node->SetValueInt(2);
		//	else if(strcmp(value,"Couple")==0)node->SetValueInt(2);
		//	else node->SetValueInt(atoi(value));
		//}
		//else if(strcmp(s,"GAME")==0){
		//	if(strcmp(value,"Taiko")==0)node->SetValueInt(0);
		//	else if(strcmp(value,"Jube")==0)node->SetValueInt(1);
		//	else node->SetValueInt(0);
		//}
		//else if(strcmp(s,"LIFE")==0)node->SetValueInt(atoi(value));
		//else if(strcmp(s,"SCOREMODE")==0)node->SetValueInt(atoi(value));
		//else if(strcmp(s,"TOTAL")==0)node->SetValueFloat((float)atof(value));
	}
	for(;!n->IsNull();n=n->GetNext()){
		if(n->GetKey()[0]=='@'){
			if(bpm==0)break;
			float length=4*60*1000/bpm;
			struct stB{
				float bpm;
				float position;
				unsigned char gogo_time;
				unsigned char line;
				unsigned char unk2;
				unsigned char unk3;
				signed int unk4[6];
				unsigned int unk5;
			}b={
				bpm,position,
				(unsigned char)(gogo?1:0),
				(unsigned char)(line?1:0),
				0,0,-1,-1,-1,-1,-1,-1,0
			};fwrite(&b,sizeof(b),1,fp);
			struct stO{
				unsigned short count;
				unsigned short unk1;
				float speed;
			}o={0,0,speed};
			for(clNode *note=n->GetChild();!note->IsNull();note=note->GetNext())if(note->GetValueInt()!=0&&note->GetValueInt()!=8)o.count++;
			fwrite(&o,sizeof(o),1,fp);
			unsigned int pos=0;
			for(clNode *note=n->GetChild();!note->IsNull();note=note->GetNext(),pos++){
				if(note->GetValueInt()!=0&&note->GetValueInt()!=8){
					//char *list[]={"0","1:ドン","2:ド","3:ドン3回目 (ド コ ドン)","4:カッ","5:カ","6:連打","7:ドン(大)","8:カッ(大)","9:連打(大)","10:ふうせん","11:ドン(大)","12:くすだま","13:カッ(大)"};
					static unsigned int type_list[]={0,1,4,7,8,6,9,10,0,12};
					struct stN{
						unsigned int type;
						float position;
						unsigned int unk1;
						unsigned int unk2;
						unsigned short score;//ふうせん と くすだま のときは回数
						unsigned short unk3;
						float length;
					}nn={type_list[note->GetValueInt()],pos/o.count*length,0,0,0,0,0};fwrite(&nn,sizeof(nn),1,fp);
					if(nn.type==6||nn.type==9||nn.type==10||nn.type==12){
						int v=0;fwrite(&v,sizeof(v),1,fp);fwrite(&v,sizeof(v),1,fp);
					}
				}
			}
			o.count=0;o.unk1=0;o.speed=1;fwrite(&o,sizeof(o),1,fp);fwrite(&o,sizeof(o),1,fp);
			position+=length;
		}
		//if(strcmp(s,"#START")==0){
		//	if(strcmp(value,"P1")==0)node->SetValueInt(1);
		//	else if(strcmp(value,"P2")==0)node->SetValueInt(2);
		//	else node->SetValueInt(1);
		//}
		else if(strcmp(n->GetKey(),"#END")==0)break;
		else if(strcmp(n->GetKey(),"#BPMCHANGE")==0)bpm=n->GetValueFloat();
		else if(strcmp(n->GetKey(),"#GOGOSTART")==0)gogo=true;
		else if(strcmp(n->GetKey(),"#GOGOEND")==0)gogo=false;
		//else if(strcmp(s,"#MEASURE")==0)node->SetValueString(value);
		//  拍子を?/?拍子に変更する。「4/4」で4分の4拍子、「6/8」で8分の6拍子。
		//  省略されている場合は4/4になります。
		else if(strcmp(n->GetKey(),"#SCROLL")==0)speed=n->GetValueFloat();
		else if(strcmp(n->GetKey(),"#DELAY")==0)position+=n->GetValueFloat();
		//else if(strcmp(n->GetKey(),"#SECTION")==0)node->SetValueInt(0);
		//else if(strcmp(n->GetKey(),"#BRANCHSTART")==0)node->SetValueString(value);
		//  #BRANCHSTART ?, X, Y
		//  譜面分岐を開始します。
		//  ?に r を入れると連打数で分岐（roll）、 p を入れると精度で分岐（precision, perfect）、s を入れるとスコアで分岐します（score）。
		//  分岐判定時の連打数がY打以上または精度がY％以上のとき達人譜面、
		//  連打数がX打以上またはX％以上の時に玄人譜面に分岐します。
		//  X＞Yとなるように設定すると玄人譜面に分岐しません。
		//  分岐判定はこの命令の一小節前に行われます（一小節前から連打が始まる場合、その連打もカウントします）。
		//else if(strcmp(s,"#BRANCHEND")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#N")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#E")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#M")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#LEVELHOLD")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#BMSCROLL")==0)node->SetValueInt(0);
		//else if(strcmp(s,"#HBSCROLL")==0)node->SetValueInt(0);
		else if(strcmp(n->GetKey(),"#BARLINEON")==0)line=true;
		else if(strcmp(n->GetKey(),"#BARLINEOFF")==0)line=false;
	}
	fclose(fp);

	if(fopen_s(&fp,path,"rb"))return false;
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	unsigned char *buf=new unsigned char [len];
	fseek(fp,0,SEEK_SET);
	fread(buf,len,1,fp);
	fclose(fp);

	int d_len=Compress(nullptr,0,buf,len);
	unsigned char *d=new unsigned char [d_len];
	Compress(d,d_len,buf,len);
	delete [] buf;

	if(fopen_s(&fp,path,"wb"))return false;
	fputc(0x19,fp);
	fwrite(&len,3,1,fp);
	fwrite(d,d_len,1,fp);
	fclose(fp);
	delete [] d;

	return true;
}
*/

bool clTaiko::CreateJson(const char *path){
	return m_root.CreateJson(path);
}
