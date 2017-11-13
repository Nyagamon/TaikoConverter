
#include "clTJA.h"
#include "util.h"

class clCompFrac{
public:
	clCompFrac():count(_countof(nums)){for(unsigned int i=0;i<count;i++)nums[i]=0;}
	void Set(double num,unsigned char value){
		unsigned int i=(unsigned int)(num*count+0.5);
		nums[i<count?i:count-1]=value;
	}
	unsigned char Get(unsigned int index){
		return nums[(index<count)?index:count-1];
	}
	unsigned int Comp(void){


//	for(unsigned int i=0;i<m_count;i++){
//		m_c[i].c=(int)(48*m_c[i].v/m_m);
//		m_c[i].m=48;
//	}


	//if((int)(count*(stop-position2)/length)<count)n[(int)(count*(stop-position2)/length)]=8;
	//				}
	//			for(int i;count>0;count>>=1){
	//				for(i=0;i<count;i+=2)if(n[i+1])break;
	//				if(i<count)break;
	//				for(i=0;i<count;i+=2)n[i>>1]=n[i];
	//			}






		if(count==0){
			count=1;
			nums[0]=0;
		}
		return count;
	}
private:
	unsigned char nums[48];
	unsigned int count;
};



clTJA::clTJA():m_root(){}


/*
bool clTaiko::LoadTJA(const char *path){
	FILE *fp;
	if(fopen_s(&fp,path,"rb"))return false;
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	char *buf=new char [len+1];buf[len]='\0';
	fseek(fp,0,SEEK_SET);
	fread(buf,len,1,fp);
	fclose(fp);
	clNode *bar=nullptr;
	for(char *s=buf,*n=s,*p;s&&*s;s=n){
		n=strstr(s,"\n");if(n)*(n++)='\0';
		p=strstr(s,"//");if(p)*p='\0';
		s=Trim(s);
		if((*s>='0'&&*s<='9')||*s==','){
			for(;*s;s++){
				if(*s>='0'&&*s<='9'){
					if(!bar)bar=m_tja.AddChildLast("@");
					bar->AddChildLast()->SetValueInt(*s-'0');
				}else if(*s==','){
					bar=nullptr;
				}
			}
		}else if(*s){
			char *value=strstr(s,*s=='#'?" ":":");if(value)*(value++)='\0';
			value=Trim(value);value=value?value:"";
			clNode *node;if(bar)node=bar->AddPrev(s);else node=m_tja.AddChildLast(s);
			if(strcmp(s,"TITLE")==0)node->SetValueString(value);
			else if(strcmp(s,"LEVEL")==0)node->SetValueInt(atoi(value));
			else if(strcmp(s,"BPM")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"WAVE")==0)node->SetValueString(value);
			else if(strcmp(s,"OFFSET")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"BALLOON")==0)node->SetValueString(value);
			//  BALLOON: ゲキ連打およびイモ連打の打数。風船の数だけ ,（カンマ）で区切って指定。
			//  数字と数字の間に改行は含めないでください。
			//  省略した場合は一律5打。
			else if(strcmp(s,"SONGVOL")==0)node->SetValueInt(atoi(value));
			else if(strcmp(s,"SEVOL")==0)node->SetValueInt(atoi(value));
			else if(strcmp(s,"SCOREINIT")==0)node->SetValueString(value);
			//  :	10コンボ未満の時に小音符を良判定で叩いた時の得点を指定できます。
			//  省略した場合は自動で計算されます。
			//  得点方式がstableの時の配点は、550, 800 のようにカンマを入れた後に入力。
			else if(strcmp(s,"SCOREDIFF")==0)node->SetValueString(value);
			//  :	10コンボごとに加算される一打あたりの点数を指定します。
			//  省略した場合は自動で計算されます。
			//  値の最後にアルファベットのdを入れると、1打あたりの点数の加算は200コンボを越えたときの1回のみになります。
			//  SCOREINIT:1000、SCOREDIFF:1000d と入力するとドンだフルコースの配点（大音符は得点2倍）になります。
			//  またゴーゴータイムは1.2倍の得点として扱われます。
			else if(strcmp(s,"COURSE")==0){
				if(strcmp(value,"Easy")==0)node->SetValueInt(0);
				else if(strcmp(value,"Normal")==0)node->SetValueInt(1);
				else if(strcmp(value,"Hard")==0)node->SetValueInt(2);
				else if(strcmp(value,"Oni")==0)node->SetValueInt(3);
				else if(strcmp(value,"Edit")==0)node->SetValueInt(4);
				else if(strcmp(value,"Tower")==0)node->SetValueInt(5);
				else node->SetValueInt(atoi(value));
			}
			else if(strcmp(s,"STYLE")==0){
				if(strcmp(value,"Single")==0)node->SetValueInt(1);
				else if(strcmp(value,"Double")==0)node->SetValueInt(2);
				else if(strcmp(value,"Couple")==0)node->SetValueInt(2);
				else node->SetValueInt(atoi(value));
			}
			else if(strcmp(s,"GAME")==0){
				if(strcmp(value,"Taiko")==0)node->SetValueInt(0);
				else if(strcmp(value,"Jube")==0)node->SetValueInt(1);
				else node->SetValueInt(0);
			}
			else if(strcmp(s,"LIFE")==0)node->SetValueInt(atoi(value));
			else if(strcmp(s,"DEMOSTART")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"SIDE")==0){
				if(strcmp(value,"Normal")==0)node->SetValueInt(1);
				else if(strcmp(value,"Ex")==0)node->SetValueInt(2);
				else if(strcmp(value,"Both")==0)node->SetValueInt(3);
				else node->SetValueInt(atoi(value));
			}
			else if(strcmp(s,"SUBTITLE")==0)node->SetValueString(value);
			else if(strcmp(s,"SCOREMODE")==0)node->SetValueInt(atoi(value));
			else if(strcmp(s,"TOTAL")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"#START")==0){
				if(strcmp(value,"P1")==0)node->SetValueInt(1);
				else if(strcmp(value,"P2")==0)node->SetValueInt(2);
				else node->SetValueInt(1);
			}
			else if(strcmp(s,"#END")==0)node->SetValueInt(0);
			else if(strcmp(s,"#BPMCHANGE")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"#GOGOSTART")==0)node->SetValueInt(0);
			else if(strcmp(s,"#GOGOEND")==0)node->SetValueInt(0);
			else if(strcmp(s,"#MEASURE")==0)node->SetValueString(value);
			//  拍子を?/?拍子に変更する。「4/4」で4分の4拍子、「6/8」で8分の6拍子。
			//  省略されている場合は4/4になります。
			else if(strcmp(s,"#SCROLL")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"#DELAY")==0)node->SetValueFloat((float)atof(value));
			else if(strcmp(s,"#SECTION")==0)node->SetValueInt(0);
			else if(strcmp(s,"#BRANCHSTART")==0)node->SetValueString(value);
			//  #BRANCHSTART ?, X, Y
			//  譜面分岐を開始します。
			//  ?に r を入れると連打数で分岐（roll）、 p を入れると精度で分岐（precision, perfect）、s を入れるとスコアで分岐します（score）。
			//  分岐判定時の連打数がY打以上または精度がY％以上のとき達人譜面、
			//  連打数がX打以上またはX％以上の時に玄人譜面に分岐します。
			//  X＞Yとなるように設定すると玄人譜面に分岐しません。
			//  分岐判定はこの命令の一小節前に行われます（一小節前から連打が始まる場合、その連打もカウントします）。
			else if(strcmp(s,"#BRANCHEND")==0)node->SetValueInt(0);
			else if(strcmp(s,"#N")==0)node->SetValueInt(0);
			else if(strcmp(s,"#E")==0)node->SetValueInt(0);
			else if(strcmp(s,"#M")==0)node->SetValueInt(0);
			else if(strcmp(s,"#LEVELHOLD")==0)node->SetValueInt(0);
			else if(strcmp(s,"#BMSCROLL")==0)node->SetValueInt(0);
			else if(strcmp(s,"#HBSCROLL")==0)node->SetValueInt(0);
			else if(strcmp(s,"#BARLINEON")==0)node->SetValueInt(0);
			else if(strcmp(s,"#BARLINEOFF")==0)node->SetValueInt(0);
			else node->SetValueString(Trim(value));
		}
	}
	delete [] buf;
	return true;
}
*/
#include <string.h>
bool clTJA::Save(clNode *s,clNode *m,const char *dir){
	char buf[0x200];
	sprintf_s(buf,_countof(buf),"%s\\%s.tja",dir,s->GetChild("fumenname")->GetValueString());
	FILE *fp;if(!fopen_s(&fp,buf,"wb")){
		fprintf(fp,"TITLE:%s\r\n",UTF8toSJIS(buf,_countof(buf),s->GetChild("title")->GetValueString()));
		//fprintf(fp,"SUBTITLE:--\r\n");
		fprintf(fp,"WAVE:%s.wav\r\n",s->GetChild("songname")->GetValueString());
		fprintf(fp,"SONGVOL:%d\r\n",100);
		fprintf(fp,"SEVOL:%d\r\n",70);
		fprintf(fp,"GAME:Taiko\r\n");
		////fprintf(fp,"LIFE:");
		fprintf_(fp,"DEMOSTART:",m->GetChild("demo_position")->GetValueInt()/1000.0,"\r\n");
		fprintf(fp,"SCOREMODE:%d\r\n",2);
		fprintf(fp,"SIDE:Both\r\n");
		SaveCource(fp,s->GetChild("fumen_easy"),0,s->GetChild("level_easy")->GetValueInt());
		SaveCource(fp,s->GetChild("fumen_normal"),1,s->GetChild("level_normal")->GetValueInt());
		SaveCource(fp,s->GetChild("fumen_hard"),2,s->GetChild("level_hard")->GetValueInt());
		SaveCource(fp,s->GetChild("fumen_master"),3,s->GetChild("level_master")->GetValueInt());
		fclose(fp);
	}
	return true;
}
bool clTJA::SaveCource(FILE *fp,clNode *fumen,int cource,int level){
	if(!fumen->IsNull()){
		fprintf(fp,"\r\n");
		const char *cource_name[]={"Easy","Normal","Hard","Oni"};
		float bpm=fumen->GetChild("bars")->GetChild()->GetChild("bpm")->GetValueFloat();
		float position=fumen->GetChild("bars")->GetChild()->GetChild("position")->GetValueFloat();
		fprintf(fp,"COURSE:%s\r\n",cource_name[cource]);
		fprintf(fp,"LEVEL:%d\r\n",level);
		fprintf(fp,"STYLE:Single\r\n");
		//fprintf(fp,"SCOREINIT:\r\n");//@@@@@
		//fprintf(fp,"SCOREDIFF:\r\n");//@@@@
		//fprintf(fp,"TOTAL:\r\n");//@@@@
		fprintf_(fp,"BPM:",bpm,"\r\n");
		fprintf_(fp,"OFFSET:",-position/1000.0-4*60.0/bpm,"\r\n");
		{
			bool balloon=false;
			for(clNode *bar=fumen->GetChild("bars")->GetChild();!bar->IsNull();bar=bar->GetNext()){
				for(clNode *note=bar->GetChild("lines")->GetChild()->GetChild("notes")->GetChild();!note->IsNull();note=note->GetNext()){
					int type=note->GetChild("type")->GetValueInt();
					if(type==10||type==12){
						if(!balloon)fprintf(fp,"BALLOON:");else fprintf(fp,", ");
						fprintf(fp,"%d",note->GetChild("score")->GetValueInt());
						balloon=true;
					}
				}
			}
			if(balloon)fprintf(fp,"\r\n");
		}
		fprintf(fp,"#START\r\n");
		bool fever=false;
		bool barline=true;
		float scroll=1.0f;
		float stop=-1;
		int branch=0;
		for(clNode *bar=fumen->GetChild("bars")->GetChild();!bar->IsNull();bar=bar->GetNext()){
			float bpm2=bar->GetChild("bpm")->GetValueFloat(),length=4*60*1000/bpm2;
			float position2=bar->GetChild("position")->GetValueFloat();
			bool fever2=bar->GetChild("fever")->GetValueBool();
			bool barline2=bar->GetChild("line")->GetValueBool();
			if(bpm!=bpm2)fprintf_(fp,"#BPMCHANGE ",bpm2,"\r\n");
			float delay=position-position2;
			if(delay<=-1||delay>=1)if(position2+length-position-4*60*1000.0/bpm)fprintf_(fp,"#DELAY ",(position2+length-position-4*60*1000.0/bpm)/1000.0,"\r\n");
			if(fever!=fever2){if(fever2)fprintf(fp,"#GOGOSTART\r\n");else fprintf(fp,"#GOGOEND\r\n");}
			clNode *line=bar->GetChild("lines")->GetChild();
			if(branch>1)line=line->GetNext();
			if(branch>2)line=line->GetNext();
			float scroll2=line->GetChild("speed")->GetValueFloat();
			clCompFrac frac;
			for(clNode *note=line->GetChild("notes")->GetChild();!note->IsNull();note=note->GetNext()){
				//char *list[]={"0","1:ドン","2:ド","3:ドン3回目 (ド コ ドン)","4:カッ","5:カ","6:連打","7:ドン(大)","8:カッ(大)","9:連打(大)","10:ふうせん","11:ドン(大)","12:くすだま","13:カッ(大)"};
				static unsigned char type_list[]={0,1,1,1,2,2,5,3,4,6,7,3,9,4};
				int type=note->GetChild("type")->GetValueInt();
				float pos=note->GetChild("position")->GetValueFloat();
				float len=note->GetChild("length")->GetValueFloat();
				frac.Set(pos/length,type_list[type]);
				if(type==6||type==9||type==10||type==12)stop=position2+pos+len;
			}
			if(stop>=position2&&stop<position2+length){
				frac.Set((stop-position2)/length,8);
				stop=-1;
			}
			unsigned int count=frac.Comp();
			if(scroll!=scroll2)fprintf_(fp,"#SCROLL ",scroll2,"\r\n");
			//fprintf(fp,"#MEASURE %d/%d");
			for(unsigned int i=0;i<count;i++)fprintf(fp,"%d",frac.Get(i));
			fprintf(fp,",\r\n");
			scroll=scroll2;
			bpm=bpm2;
			position=position2+length;
			fever=fever2;
			barline=barline2;
			if(bar->GetNext()->IsNull()||!bar->GetNext()->GetChild("branch_1")->IsNull()){
				static clNode *bar3;
				static bool fever3;
				static bool barline3;
				static float scroll3;
				static float stop3;
				switch(branch){
				case 0:
					if(bar->GetNext()->IsNull())break;
					bar3=bar;
					fever3=fever;
					barline3=barline;
					scroll3=scroll;
					stop3=stop;
					branch=1;
					//fprintf(fp,"#SECTION\r\n");
					fprintf(fp,"#BRANCHSTART p, 60, 80\r\n");
					fprintf(fp,"#N\r\n");
					break;
				case 1:
					bar=bar3;
					fever=fever3;
					barline=barline3;
					scroll=scroll3;
					stop=stop3;
					branch=2;
					fprintf(fp,"#E\r\n");
					break;
				case 2:
					bar=bar3;
					fever=fever3;
					barline=barline3;
					scroll=scroll3;
					stop=stop3;
					branch=3;
					fprintf(fp,"#M\r\n");
					break;
				case 3:
					branch=0;
					fprintf(fp,"#BRANCHEND\r\n");
					for(bar3=bar->GetNext();bar3->GetChild("branch_1")->IsNull()&&!bar3->IsNull();bar3=bar3->GetNext());
					if(!bar3->IsNull())fprintf(fp,"#SECTION\r\n");
					break;
				}
			}
		}
		if(fever)fprintf(fp,"#GOGOEND\r\n");
		fprintf(fp,"#END\r\n");
	}
	//fprintf(fp,"#LEVELHOLD");
	return true;
}
