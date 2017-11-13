
#include <string.h>
#include "clTaiko.h"
#include "clTJA.h"
#include "util.h"

bool ConvertNUS3toWAVE(const char *path_nus3,const char *path_wave,const char *path_bin){
	char cmd[0x400],path_at9[0x200];
	sprintf_s(path_at9,_countof(path_at9),"%s\\tmp.at9",path_bin);
	sprintf_s(cmd,_countof(cmd),"\"%s\\at9tool.exe\" -d \"%s\" \"%s\"",path_bin,path_at9,path_wave);
	FILE *fp;if(fopen_s(&fp,path_nus3,"rb"))return false;
	struct stH{
		char magic[4];
		unsigned int size;
		char bank[4];
	}h;
	struct stC{
		char magic[4];
		unsigned int size;
	}c;
	fread(&h,sizeof(h),1,fp);
	for(unsigned int a=sizeof(h);a<h.size;a+=sizeof(c)+c.size){
		fseek(fp,a,SEEK_SET);
		fread(&c,sizeof(c),1,fp);
		if(*(unsigned int *)c.magic==0x4B434150){
			FILE *fp2;if(!fopen_s(&fp2,path_at9,"wb")){
				fcpy(fp2,fp,c.size);
				fclose(fp2);
			}
			break;
		}
	}
	fclose(fp);
	Process(cmd);
	remove(path_at9);
	return true;
}

bool ConvertWAVEtoNUS3(const char *path_wave,const char *path_nus3,const char *path_bin){
	char cmd[0x400],path_at9[0x200];
	sprintf_s(path_at9,_countof(path_at9),"%s\\tmp.at9",path_bin);
	sprintf_s(cmd,_countof(cmd),"\"%s\\at9tool.exe\" -e -rb 144 \"%s\" \"%s\"",path_bin,path_at9,path_wave);
	Process(cmd);
	FILE *fp;if(fopen_s(&fp,path_nus3,"wb")){remove(path_at9);return false;}
	struct stH{
		char magic[4];
		unsigned int size;
		char bank[4];
	}h={'N','U','S','3',0,'B','A','N','K'};fseek(fp,sizeof(h),SEEK_CUR);
	struct stC{
		char magic[4];
		unsigned int size;
	};
	{
		//stC c={'T','O','C',' ',0x3C};fwrite(&c,sizeof(c),1,fp);
		//@@@@@@@@
	}
	{
		//@@@@@
	}
	{
		stC c={'P','A','C','K',0};
		FILE *fp2;if(!fopen_s(&fp2,path_at9,"rb")){
			fseek(fp2,0,SEEK_END);
			c.size=ftell(fp2);
			fseek(fp2,0,SEEK_SET);
			fwrite(&c,sizeof(c),1,fp);
			fcpy(fp,fp2,c.size);
			fclose(fp2);
		}
	}
	h.size=ftell(fp)-8;
	fseek(fp,0,SEEK_SET);
	fwrite(&h,sizeof(h),1,fp);
	fclose(fp);
	remove(path_at9);
	return true;
}

bool ConvertEDATtoWAVE(const char *path_edat,const char *path_wave,const char *path_bin){
	char cmd[0x400],path_at3[0x200];
	sprintf_s(path_at3,_countof(path_at3),"%s\\tmp.at3",path_bin);
	sprintf_s(cmd,_countof(cmd),"\"%s\\at3tool.exe\" -d \"%s\" \"%s\"",path_bin,path_at3,path_wave);
	FILE *fp;if(fopen_s(&fp,path_edat,"rb"))return false;
	struct stH{
		unsigned int r[4];
		unsigned int offset;
		unsigned int size;
	}h;
	fread(&h,sizeof(h),1,fp);
	fseek(fp,h.offset,SEEK_SET);
	FILE *fp2;if(!fopen_s(&fp2,path_at3,"wb")){
		fcpy(fp2,fp,h.size);
		fclose(fp2);
	}
	fclose(fp);
	Process(cmd);
	remove(path_at3);
	return true;
}

bool ConvertOGGtoWAVE(const char *path_ogg,const char *path_wave,const char *path_bin){
	//	char cmd[0x400];
	//	sprintf_s(cmd,_countof(cmd),"\"%s\\at9tool.exe\" -d \"%s\" \"%s\"",path_bin,path_at9,path_wave);
	//	Process(cmd);
	return true;
}


//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include <stdio.h>
#include "clNode.h"
#ifndef _countof
#define _countof(o) (sizeof(o)/sizeof(o[0]))
#endif





bool UnpackL7CA(const char *path){
	FILE *fp;if(fopen_s(&fp,path,"rb"))return false;
	struct stHeader{
		char magic[4];
		unsigned int version;
		unsigned int size;
		unsigned int meta_offset;
		unsigned int meta_size;
		unsigned int unk1;
		unsigned int en_count;
		unsigned int dir_count;
		unsigned int file_count;
		unsigned int chunk_count;
		unsigned int string_size;
		unsigned int unk2;
	}h;fread(&h,sizeof(h),1,fp);
	if(*(unsigned int *)h.magic!=0x4143374C){fclose(fp);return false;}
	fseek(fp,h.meta_offset,SEEK_SET);
	for(unsigned int i=0;i<h.en_count;i++){
		//fseek(fp,h.meta_offset+i*sizeof(),SEEK_SET);
		struct{
			signed int id;
			unsigned int hash;
			unsigned int folder_offset;
			unsigned int filename_offset;
			unsigned long long int timestamp;
		}e;fread(&e,sizeof(e),1,fp);
		i=i;
	}
	fclose(fp);
	return true;
}



/*
class L7CAFileEntry
{
	public int compressedFilesize;
	public int rawFilesize;
	public int chunkIdx;
	public int chunkCount;
	public int offset;
	public uint crc32;
}

class L7CAChunkEntry
{
	public int chunkSize;
	public ushort unk = 0;
	public ushort chunkId;
}


		if(header.unk4 != 0x05)
		{
			Console.WriteLine("This archive type is unsupported and most likely won't unpack properly.");
		}

		// Read strings
		var baseOffset = reader.BaseStream.Length - header.stringTableSize;
		reader.BaseStream.Seek(baseOffset, SeekOrigin.Begin);

		Dictionary<int, string> strings = new Dictionary<int, string>();
		while (reader.PeekChar() != -1)
		{
			int offset = (int)(reader.BaseStream.Position - baseOffset);
			int len = reader.ReadByte();
			var str = Encoding.UTF8.GetString(reader.ReadBytes(len));

			strings.Add(offset, str);
		}


		// Read filesystem entries
		Dictionary<uint, L7CAFilesystemEntry> entries = new Dictionary<uint, L7CAFilesystemEntry>();
		reader.BaseStream.Seek(header.metadataOffset, SeekOrigin.Begin);
		for (int i = 0; i < header.filesystemEntries; i++)
		{
			L7CAFilesystemEntry entry = new L7CAFilesystemEntry();
			entry.id = reader.ReadUInt32();
			entry.hash = reader.ReadUInt32();
			entry.folderOffset = reader.ReadInt32();
			entry.filenameOffset = reader.ReadInt32();
			entry.timestamp = reader.ReadInt64();

			if(entry.id == 0xffffffff)
				entry.filename = String.Format("{0}", strings[entry.folderOffset]);
			else
				entry.filename = String.Format("{0}/{1}", strings[entry.folderOffset], strings[entry.filenameOffset]);

			//Console.WriteLine("{0:x8} {1:x8} {2:x8} {3:x8} {4:x16}", entry.id, entry.hash, entry.folderOffset, entry.filenameOffset, entry.timestamp);

			if (Crc32.CalculateNamco(entry.filename) != entry.hash)
			{
				Console.WriteLine("{0} did not match expected hash", entry.filename);
			}

			if (entry.id != 0xffffffff)
			{
				entries.Add(entry.id, entry);
			}
			else
			{
				// 0xffffffff is a folder.
				// Only create a folder and move on to next entry.
				// This step probably isn't needed,  but just for the sake of completeness I added it.
				// There might be some game out there that has blank folders but no actual data in it, so those will be accounted for as well.

				if (!Directory.Exists(entry.filename))
					Directory.CreateDirectory(entry.filename);
			}
		}

		// Read file information
		List<L7CAFileEntry> files = new List<L7CAFileEntry>();
		for (int i = 0; i < header.files; i++)
		{
			L7CAFileEntry entry = new L7CAFileEntry();
			entry.compressedFilesize = reader.ReadInt32();
			entry.rawFilesize = reader.ReadInt32();
			entry.chunkIdx = reader.ReadInt32();
			entry.chunkCount = reader.ReadInt32();
			entry.offset = reader.ReadInt32();
			entry.crc32 = reader.ReadUInt32();

			//var filename = entries[(uint)i].filename;
			//Console.WriteLine("{2}\noffset[{0:x8}] filesize[{1:x8}]\nreal_crc32[{3:x8}] crc32[{4:x8}] crc32[{5:x8}]\n", entry.offset, entry.compressedFilesize, filename, entries[(uint)i].hash, crc32.Value, entry.crc32);

			files.Add(entry);
		}

		// Read chunk information
		List<L7CAChunkEntry> chunks = new List<L7CAChunkEntry>();
		for (int i = 0; i < header.chunks; i++)
		{
			L7CAChunkEntry entry = new L7CAChunkEntry();
			entry.chunkSize = reader.ReadInt32();
			entry.unk = reader.ReadUInt16();
			entry.chunkId = reader.ReadUInt16();

			//Console.WriteLine("{3:x8} {0:x8} {1:x4} {2:x4}", entry.chunkSize, entry.unk, entry.chunkNum, i);

			chunks.Add(entry);
		}

		for (int i = 0; i < header.files; i++)
		{
			var file = files[i];
			var entry = entries[(uint)i];

			Console.WriteLine("Extracting {0}...", entry.filename);
			//Console.WriteLine("{0:x1} {1:x8} {2:x8} {3:x8} {4:x8} {5:x8}", file.chunkIdx, file.chunkCount, file.offset, file.compressedFilesize, file.rawFilesize, file.crc32);

			//var output = Path.Combine("output", entry.filename);
			var output = entry.filename;
			Directory.CreateDirectory(Path.GetDirectoryName(output));

			reader.BaseStream.Seek(file.offset, SeekOrigin.Begin);
			var orig_data = reader.ReadBytes(file.compressedFilesize);

			//MemoryStream data = new MemoryStream();
			List<byte> data = new List<byte>();

			using (BinaryReader datastream = new BinaryReader(new MemoryStream(orig_data)))
			{
				if (DebugDecompressionCode)
				{
					foreach (var f in Directory.EnumerateFiles(".", "output-chunk-*.bin"))
						File.Delete(f);
				}

				for (int x = 0; x < file.chunkCount; x++)
				{
					int len = chunks[file.chunkIdx + x].chunkSize & 0x7fffffff;
					bool isCompressed = (chunks[file.chunkIdx + x].chunkSize & 0x80000000) != 0;

					if (DebugDecompressionCode)
						Console.WriteLine("{0:x8} {1}", len, isCompressed);

					var d = datastream.ReadBytes(len);

					if (DebugDecompressionCode)
						File.WriteAllBytes(String.Format("output-chunk-{0}.bin", x), d);

					if (isCompressed)
					{
						try
						{
							// Decompress chunk
							d = Decompress(d, data.ToArray());
							data = new List<byte>(d);
						}
						catch
						{
							// Save compressed data
							Console.WriteLine("Could not decompress file.");
							data.AddRange(d);
						}
					}
					else
					{
						data.AddRange(d);
					}

					//Console.WriteLine(" {0:x8}", d.Length);

					if (DebugDecompressionCode)
						File.WriteAllBytes(String.Format("output-chunk-{0}-decomp.bin", x), d);
				}
			}

			var crc32 = Crc32.Calculate(data.ToArray());
			if (crc32 != file.crc32)
			{
				Console.WriteLine("Invalid CRC32: {0:x8} vs {1:x8}", crc32, file.crc32);
				Console.WriteLine();

				//File.WriteAllBytes("invalid.bin", data.ToArray());
				//Environment.Exit(1);
			}

			File.WriteAllBytes(output, data.ToArray());

			if (data.Count != file.rawFilesize)
			{
				Console.WriteLine("Invalid filesize: {0:x8} vs {1:x8}", data.Count, file.rawFilesize);
				//Environment.Exit(1);
			}
		}
	}
}

*/






int main(int argc,char *argv[]){
#ifdef _DEBUG
	clTaiko t;
	clTJA tja;
	if(1){
		t.Load("C:\\Users\\N\\Desktop\\太鼓解析\\DLC_VITA\\dlc\\TAIKOV1SONG048JH");
		//t.CreateJson("C:\\tmp\\test.txt");
		tja.Save(t.Get()->GetChild("songs")->GetChild(),t.Get()->GetChild("musics")->GetChild(),"C:\\tmp");
	}
	if(0){
		t.Load("C:\\Users\\N\\Desktop\\太鼓解析\\DLC_PSP\\NPJH50426\\SONG_DLC_015");
		//t.CreateJson("C:\\tmp\\test.txt");
		//tja.Save(t.Get(),"C:\\tmp",true);
	}
	if(0){
		t.Load("C:\\Users\\N\\Desktop\\太鼓解析\\DLC_VITA\\dlc\\TAIKOV1SONG005JJ");
		t.CreateJson("C:\\tmp\\test.txt");
	}
	return 0;
#endif

	// TODO:機能追加
	//   TJA -> DLC (songinfo出力,fumen出力,ogg対応)
	//   l7c 収録曲
	//   TJA -> DX DLC
	//   DX 収録曲
	//   DLC -> TJA 分岐譜面対応
	//   DLC -> TJA スコア対応
	//
	// TODO:修正
	//   DLC -> TJA ノート圧縮 (3/4とかは？)
	//   エラー処理は後回し。バッファオーバーフローもまだ修正しなくていい
	//   

	// コマンドライン解析
	unsigned int count=0;
	char *path_out=nullptr;
	bool genre=false;
	bool json=false;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'||argv[i][0]=='/'){
			switch(argv[i][1]){
			//case 'o':if(i+1<argc){path_out=argv[++i];}break;
			case 'g':genre=true;break;
			case 'j':json=true;break;
			}
		}
		else if(*argv[i]){
			argv[++count]=argv[i];
		}
	}

	char dir[0x200];
	GetDirectory(dir,_countof(dir),argv[0]);

	char path_bin[0x200];
	sprintf_s(path_bin,_countof(path_bin),"%s\\bin",dir);

	// exeチェック
	char path_bin_exe[0x200];
	sprintf_s(path_bin_exe,_countof(path_bin_exe),"%s\\at9tool.exe",path_bin);
	bool exe1=ExistFile(path_bin_exe);
	sprintf_s(path_bin_exe,_countof(path_bin_exe),"%s\\at3tool.exe",path_bin);
	bool exe2=ExistFile(path_bin_exe);
	if(!(exe1&&exe2)){
		printf("WARNING: 音声ファイルの変換には ");
		if(!exe1)printf("at9tool.exe");
		else if(!exe2)printf("at3tool.exe");
		else printf("at9tool.exe または at3tool.exe");
		printf(" が必要です。\n");
		printf("         ");
		if(!exe1)printf("at9tool.exe");
		else if(!exe2)printf("at3tool.exe");
		else printf("at9tool.exe または at3tool.exe");
		printf(" を bin フォルダに入れてください。\n");
	}

	// 入力チェック
	if(!count){
		printf("Error: 入力ファイルまたはフォルダを指定してください。\n");
		return -1;
	}

	//
	for(unsigned int i=1;i<=count;i++){

		// 2つ目以降のファイルは、出力ファイル名オプションが無効
		if(i)path_out=nullptr;

		// デフォルト出力ファイル名
		char path[0x200];
		if(!(path_out&&path_out[0])){
			strcpy_s(path,sizeof(path),argv[i]);
			char *d1=strrchr(path,'\\');
			char *d2=strrchr(path,'/');
			char *e=strrchr(path,'.');
			if(e&&d1<e&&d2<e)*e='\0';
			if(json)strcat_s(path,sizeof(path),".json");
			path_out=path;
		}

		if(json){
			clTaiko t;
			printf("%s を変換中...",argv[i]);
			if(t.Load(argv[i])&&t.CreateJson(path_out)){
				printf("OK\n");
			}else{
				printf("\nERROR: 変換に失敗しました。\n");
			}
		}else{
			if(_strrcmpi(argv[i],".tja")==0){
				clTaiko t;
				printf("%s を変換中...",argv[i]);
				char buf[0x200];GetDirectory(buf,_countof(buf),path_out);
				//if(t.LoadTJA(argv[i])&&t.SaveDLV_V(buf,path_bin)){
				//	printf("OK\n");
				//}else{
					printf("\nERROR: 変換に失敗しました。\n");
				//}
			}else{
				clTaiko t;clTJA tja;
				printf("%s を変換中...",argv[i]);
				char buf[0x200];GetDirectory(buf,_countof(buf),path_out);
				if(t.Load(argv[i])){
					for(clNode *s=t.Get()->GetChild("songs")->GetChild();!s->IsNull();s=s->GetNext()){
						char dir[0x200];strcpy_s(dir,_countof(dir),buf);
						if(genre){
							struct{
								int no;const char *name;
								const char *color;
								const char *color_font;
							}list[]={
								{1,"J-POP","#21a1ba","#ffffff"},//#87ceeb
								{3,"ボーカロイド曲","#d3daeb","#ffffff"},//#c0c0c0,#def2f2
								{5,"バラエティ","#8fd41f","#ffffff"},//#66cc51
								{8,"ナムコオリジナル","#ff5b14","#ffffff"},//#ff7733
								{7,"ゲームミュージック","#9d76bf","#ffffff"},//#9283db
								{6,"クラシック","#d1a314","#ffffff"},//#ffbd19
								{2,"アニメ","#ff9900","#ffffff"},//#ff9933
								{4,"どうよう","#fedc2d","#ffffff"},//#ff7fbf
								{9,"その他","#000000","#ffffff"},
							};
							int list_dx[]={6,0,1,5,4,3,2,7};
							int index=s->GetChild("genre")->GetValueInt();
							index=
								(index>=8)?8:
								(_strcmpi(t.Get()->GetChild("type")->GetValueString(),"v")==0)?index:list_dx[index];
							sprintf_s(dir,_countof(dir),"%s\\%d %s",buf,list[index].no,list[index].name);MakeDirectory(dir,false);
							char buf2[0x200];sprintf_s(buf2,_countof(buf2),"%s\\genre.ini",dir);
							if(!ExistFile(buf2)){
								FILE *fp;if(!fopen_s(&fp,buf2,"wb")){
									fprintf(fp,"[Genre]\r\n");
									fprintf(fp,"GenreName=%s\r\n",list[index].name);
									fprintf(fp,"GenreColor=%s\r\n",list[index].color);
									fprintf(fp,"FontColor=%s\r\n",list[index].color_font);
									fclose(fp);
								}
							}
						}
						const char *songname=s->GetChild("songname")->GetValueString();
						strcat_s(dir,_countof(dir),"\\");strcat_s(dir,_countof(dir),songname);MakeDirectory(dir,false);
						clNode *m,*path;
						for(m=t.Get()->GetChild("musics")->GetChild(),path=t.GetPaths()->GetChild();!m->IsNull();m=m->GetNext(),path=path->GetNext()){
							if(strcmp(m->GetChild("songname")->GetValueString(),songname)==0)break;
						}
						tja.Save(s,m,dir);
						char buf2[0x200];sprintf_s(buf2,_countof(buf2),"%s\\%s.wav",dir,songname);
						if(!ExistFile(buf2)){
							if(_strrcmpi(path->GetValueString(),".nus3bank")==0){
								ConvertNUS3toWAVE(path->GetValueString(),buf2,path_bin);
							}else if(_strrcmpi(path->GetValueString(),".EDAT")==0){
								ConvertEDATtoWAVE(path->GetValueString(),buf2,path_bin);
							}
						}
					}
					printf("OK\n");
				}else{
					printf("\nERROR: 変換に失敗しました。\n");
				}
			}
		}
	}

	return 0;
}


/*LZ11
int bit,indicator,len,disp;
d_len=0;
for(unsigned int i=0;i<s_len;){
	bit=s[i++];
	for(unsigned int j=1;j<0x100;j=j+j){
		if(bit&j){
			indicator=s[i]>>4;
			if(indicator>1){//A BCD
				len=indicator+0x1;
				//disp=(s[i]<<4)+(s[i+1]<<4)+1;
				i+=2;
			}else if(indicator==0){//BC DEF
				len=(s[i]<<4)+(s[i+1]>>4)+0x11;
				//disp=(s[i+1]<<4)+1;
				i+=3;
			}else{//BCDE FGH
				len=((int)(s[i]&0x0F)<<12)+((int)s[i+1]<<4)+((int)(s[i+2]&0xF0)>>4)+0x111;
				//disp=;
				i+=4;
			}
			if(d){
				for(int j=0;j<len;j++){
					if(j>=disp)d[d_len++]=0;
					else d[d_len++]=d[d_len-disp];
				}
			}else d_len+=len;
		}else{
			if(d)d[d_len++]=s[i++];else{d_len++;i++;}
		}
	}
}
*/