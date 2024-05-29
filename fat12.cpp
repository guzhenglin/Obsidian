#define _CRT_SECURE_NO_WARNINGS     //禁用对fopen的警告
#include<iostream>
#include<cstring>
#include<fstream>

using namespace std;


typedef unsigned char u8;    //1字节
typedef unsigned short u16;  //2字节
typedef unsigned int u32;    //4字节

int BytsPerSec;     //每扇区字节数
int SecPerClus;     //每簇扇区数
int RsvdSecCnt;     //Boot扇区数
int NumFATs;        //FAT表个数
int RootEntCnt;     //根目录最大文件数
int FATSz;          //FAT扇区数
int filecount = 0;  //文件数

#pragma pack (1)     //1字节对齐

struct BPB
{
	u16 BPB_BytsPerSec;     //每扇区字节数
	u8  BPB_SecPerClus;     //每簇扇区数
	u16 BPB_RsvdSecCnt;     //Boot扇区数
	u8  BPB_NumFATs;        //FAT表个数
	u16 BPB_RootEntCnt;     //根目录最大文件数
	u16 BPB_TotSec16;       //逻辑扇区总数
	u8  BPB_Media;          //介质描述符
	u16 BPB_FATSz16;        //FAT扇区数
	u16 BPB_SecPerTrk;      //每磁道扇区数
	u16 BPB_NumHeads;       //磁头数
	u32 BPB_HiddSec;        //隐藏扇区数
	u32 BPB_TotSec32;       //逻辑扇区总数
};
//启动区 从11到35字节

struct RootEntry
{
	char DIR_Name[11];	  //文件名+拓展名
	u8   DIR_Attr;		  //文件属性
	char DIR_NTRes[10];	  //保留
	u16  DIR_WrtTime;	  //最后写入时间
	u16  DIR_WrtDate;	  //最后写入日期
	u16  DIR_FstClus;	  //起始簇号
	u32  DIR_FileSize;	  //文件大小
};
//根目录项 32字节，偏移量待计算

#pragma pack ()

void getBPBvalue(FILE* fat12,struct BPB* bpb_ptr)
{
	int flag;
	//指向BPB第11字节
	flag = fseek(fat12, 11, SEEK_SET);
	if(flag==-1)       //错误处理
	{
		cout << "fseek error! line:61" << endl;
		exit(0);
	}

	//读取25字节的数据
	flag = fread(bpb_ptr, 1, 25, fat12);
	if(flag==-1)       //错误处理
	{
		cout << "fread error! line:69" << endl;
		exit(0);
	}
}

void getFilename(FILE* fat12,struct RootEntry* rootEntry_ptr,int* filelocation)
{
	int offset = (RsvdSecCnt + NumFATs * FATSz) * BytsPerSec;//根目录偏移量
	int flag;
	int index = 0;
	for(int i=0;i<RootEntCnt;i++)
	{
		flag = fseek(fat12, offset+i*32, SEEK_SET);     //读取第i个条目
		if(flag==-1)              //错误处理
		{
			cout << "fseek error! line:84" << endl;
			exit(0);
		}
		flag = fread(rootEntry_ptr, 1, 32, fat12);
		if(flag!=32)
		{
			cout << "fread error! line:90" << endl;
			exit(0);
		}
		if(rootEntry_ptr->DIR_Name[0]=='\0')          //跳过无效条目
			continue;
		int sign = 0;
		for(int j=0;j<11;j++)                         //判断是否为文件
		{
			if (!(((rootEntry_ptr->DIR_Name[j] >= '0') && (rootEntry_ptr->DIR_Name[j] <= '9')) ||
				((rootEntry_ptr->DIR_Name[j] >= 'A') && (rootEntry_ptr->DIR_Name[j] <= 'Z')) ||
				((rootEntry_ptr->DIR_Name[j] >= 'a') && (rootEntry_ptr->DIR_Name[j] <= 'z')) ||
				rootEntry_ptr->DIR_Name[j] == ' '))
			{
				sign = 1;
				break;
			}
		}
		if(sign==1)
			continue;
		filelocation[index++] = i;
	}
}

u16 getValue(FILE* fat12,int num)            //从FAT表读取簇号
{
	int fat1_ptr = RsvdSecCnt * BytsPerSec;
	int fatpos = fat1_ptr + num * 3 / 2;
	int type = -1;
	if (num % 2 == 0)
		type = 0;
	else
		type = 1;

	u16 bytes;
	u16* bytes_ptr = &bytes;
	int flag;
	flag = fseek(fat12, fatpos, SEEK_SET);
	if(flag==-1)
	{
		cout << "fseek error! line:129" << endl;
		exit(0);
	}
	flag = fread(bytes_ptr, 1, 2, fat12);
	if(flag!=2)
	{
		cout << "fread error! ;line:135" << endl;
		exit(0);
	}
	u16 byte1 = bytes << 4;            
	byte1 = byte1 >> 4;           
	if (type == 0)
		return byte1;      //返回byte2的低4位和byte1
	else
		return (bytes >> 4)&0x0FFF;		//返回byte2和byte1的高4位
}

void getfile(FILE* fat12,int startClus,string name)
{
	FILE* outfile;
	outfile = fopen(name.c_str(), "ab");
	//数据簇开始
	int data_ptr = BytsPerSec * (RsvdSecCnt + FATSz * NumFATs + (RootEntCnt * 32 + BytsPerSec - 1) / BytsPerSec);
	u16 nextClus;
	u16 Clus = startClus;
	while(Clus<0xFF8)
	{
		nextClus = getValue(fat12, Clus);
		if(nextClus==0xFF7)
		{
			cout << "bad cluster!" << endl;
			exit(1);
		}
		else
		{
			char* str = (char*)malloc(SecPerClus * BytsPerSec);
			char* content = str;
			int startByte = data_ptr + (Clus - 2) * SecPerClus * BytsPerSec;
			int flag;
			flag = fseek(fat12, startByte, SEEK_SET);
			if(flag==-1)
			{
				cout << "fseek error! line:171" << endl;
				exit(0);
			}
			flag = fread(content, 1, SecPerClus * BytsPerSec, fat12);
			if (flag != SecPerClus * BytsPerSec)
			{
				cout << "fread error! line:177" << endl;
				exit(0);
			}
			fwrite(str, 1, SecPerClus * BytsPerSec, outfile);
		}
		Clus = nextClus;
	}
	fclose(outfile);
}

void getother(FILE* fat12, int startClus, string name)
{
	ofstream outfile(name, ios::app | ios::binary);
	if (!outfile.is_open())
	{
		cout << "outfile open error!" << endl;
		exit(0);
	}
	//数据簇开始
	int data_ptr = BytsPerSec * (RsvdSecCnt + FATSz * NumFATs + (RootEntCnt * 32 + BytsPerSec - 1) / BytsPerSec);
	u16 nextClus;
	u16 Clus = startClus;
	while (Clus < 0xFF8)
	{
		nextClus = getValue(fat12, Clus);
		if (nextClus == 0xFF7)
		{
			cout << "bad cluster!" << endl;
			exit(1);
		}
		else
		{
			char* str = (char*)malloc(SecPerClus * BytsPerSec);
			char* content = str;
			int startByte = data_ptr + (Clus - 2) * SecPerClus * BytsPerSec;
			int flag;
			flag = fseek(fat12, startByte, SEEK_SET);
			if (flag == -1)
			{
				cout << "fseek error!" << endl;
				exit(0);
			}
			flag = fread(content, 1, SecPerClus * BytsPerSec, fat12);
			if (flag != SecPerClus * BytsPerSec)
			{
				cout << "fread error! ;line:222" << endl;
				exit(0);
			}
			cout << str << endl;
			outfile << str;
		}
		Clus = nextClus;
	}
	outfile.close();
}


int main()
{
	FILE* fat12;
	fat12 = fopen("fat12.img", "rb");

	struct BPB bpb;
	struct BPB* bpb_ptr = &bpb;
	struct RootEntry rootEntry;
	struct RootEntry* rootEntry_ptr = &rootEntry;

	getBPBvalue(fat12, bpb_ptr);
	//写入全局变量
	BytsPerSec = bpb_ptr->BPB_BytsPerSec;
	SecPerClus = bpb_ptr->BPB_SecPerClus;
	RsvdSecCnt = bpb_ptr->BPB_RsvdSecCnt;
	NumFATs = bpb_ptr->BPB_NumFATs;
	RootEntCnt = bpb_ptr->BPB_RootEntCnt;
	//FATSz可能出现在两处
	if (bpb_ptr->BPB_FATSz16 != 0)
	{
		FATSz = bpb_ptr->BPB_FATSz16;
	}
	else
	{
		FATSz = bpb_ptr->BPB_TotSec32;
	}

	string *filenames=(string*)new string[RootEntCnt];
	int *filelocation = (int*)new int[RootEntCnt];         //创建文件索引
	memset(filelocation, -1, RootEntCnt);              //初始化为-1

	getFilename(fat12,rootEntry_ptr,filelocation);

	int flag,idx = 0;
	
	while (filelocation[idx] != -1)
	{
		char* name = new char[12];
		flag = fseek(fat12, (RsvdSecCnt + NumFATs * FATSz) * BytsPerSec + filelocation[idx] * 32, SEEK_SET);
		if (flag == -1)              //错误处理
		{
			cout << "fseek error! line:275" << endl;
			exit(0);
		}
		flag = fread(rootEntry_ptr, 1, 32, fat12);
		if (flag != 32)
		{
			cout << "fread error! line:281" << endl;
			exit(0);
		}
		int temp = -1;
		for (int k = 0; k < 11; k++)
		{
			if (rootEntry_ptr->DIR_Name[k] != ' ') {
				temp++;
				name[temp] = rootEntry_ptr->DIR_Name[k];
			}
			else {
				temp++;
				name[temp] = '.';
				while (rootEntry_ptr->DIR_Name[k] == ' ')
					k++;
				k--;
			}
		}
		temp++;
		name[temp] = '\0';
		for (int i = 0; i < temp; i++)
		{
			filenames[idx] += name[i];
		}
		cout << idx<<":"<<name << endl;
		idx++;
		delete[]name;
	}
	filecount = idx;
	int select;
	cout << "Input the number of file you want to read:" << endl;
	cin >> select;
	while (select >= 0 && select < filecount) {
		flag = fseek(fat12, (RsvdSecCnt + NumFATs * FATSz) * BytsPerSec + filelocation[select] * 32, SEEK_SET);
		flag = fread(rootEntry_ptr, 1, 32, fat12);
		if (strstr(filenames[select].c_str(), "PDF"))
			getfile(fat12, rootEntry_ptr->DIR_FstClus, filenames[select]);
		else
			getother(fat12, rootEntry_ptr->DIR_FstClus, filenames[select]);
		cout << "Input the number of file you want to read:" << endl;
		cin >> select;
	}
	cout << "Input over!" << endl;
	delete[]filenames;
	return 0;
}