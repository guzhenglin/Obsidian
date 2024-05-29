#define _CRT_SECURE_NO_WARNINGS     //���ö�fopen�ľ���
#include<iostream>
#include<cstring>
#include<fstream>

using namespace std;


typedef unsigned char u8;    //1�ֽ�
typedef unsigned short u16;  //2�ֽ�
typedef unsigned int u32;    //4�ֽ�

int BytsPerSec;     //ÿ�����ֽ���
int SecPerClus;     //ÿ��������
int RsvdSecCnt;     //Boot������
int NumFATs;        //FAT�����
int RootEntCnt;     //��Ŀ¼����ļ���
int FATSz;          //FAT������
int filecount = 0;  //�ļ���

#pragma pack (1)     //1�ֽڶ���

struct BPB
{
	u16 BPB_BytsPerSec;     //ÿ�����ֽ���
	u8  BPB_SecPerClus;     //ÿ��������
	u16 BPB_RsvdSecCnt;     //Boot������
	u8  BPB_NumFATs;        //FAT�����
	u16 BPB_RootEntCnt;     //��Ŀ¼����ļ���
	u16 BPB_TotSec16;       //�߼���������
	u8  BPB_Media;          //����������
	u16 BPB_FATSz16;        //FAT������
	u16 BPB_SecPerTrk;      //ÿ�ŵ�������
	u16 BPB_NumHeads;       //��ͷ��
	u32 BPB_HiddSec;        //����������
	u32 BPB_TotSec32;       //�߼���������
};
//������ ��11��35�ֽ�

struct RootEntry
{
	char DIR_Name[11];	  //�ļ���+��չ��
	u8   DIR_Attr;		  //�ļ�����
	char DIR_NTRes[10];	  //����
	u16  DIR_WrtTime;	  //���д��ʱ��
	u16  DIR_WrtDate;	  //���д������
	u16  DIR_FstClus;	  //��ʼ�غ�
	u32  DIR_FileSize;	  //�ļ���С
};
//��Ŀ¼�� 32�ֽڣ�ƫ����������

#pragma pack ()

void getBPBvalue(FILE* fat12,struct BPB* bpb_ptr)
{
	int flag;
	//ָ��BPB��11�ֽ�
	flag = fseek(fat12, 11, SEEK_SET);
	if(flag==-1)       //������
	{
		cout << "fseek error! line:61" << endl;
		exit(0);
	}

	//��ȡ25�ֽڵ�����
	flag = fread(bpb_ptr, 1, 25, fat12);
	if(flag==-1)       //������
	{
		cout << "fread error! line:69" << endl;
		exit(0);
	}
}

void getFilename(FILE* fat12,struct RootEntry* rootEntry_ptr,int* filelocation)
{
	int offset = (RsvdSecCnt + NumFATs * FATSz) * BytsPerSec;//��Ŀ¼ƫ����
	int flag;
	int index = 0;
	for(int i=0;i<RootEntCnt;i++)
	{
		flag = fseek(fat12, offset+i*32, SEEK_SET);     //��ȡ��i����Ŀ
		if(flag==-1)              //������
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
		if(rootEntry_ptr->DIR_Name[0]=='\0')          //������Ч��Ŀ
			continue;
		int sign = 0;
		for(int j=0;j<11;j++)                         //�ж��Ƿ�Ϊ�ļ�
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

u16 getValue(FILE* fat12,int num)            //��FAT���ȡ�غ�
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
		return byte1;      //����byte2�ĵ�4λ��byte1
	else
		return (bytes >> 4)&0x0FFF;		//����byte2��byte1�ĸ�4λ
}

void getfile(FILE* fat12,int startClus,string name)
{
	FILE* outfile;
	outfile = fopen(name.c_str(), "ab");
	//���ݴؿ�ʼ
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
	//���ݴؿ�ʼ
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
	//д��ȫ�ֱ���
	BytsPerSec = bpb_ptr->BPB_BytsPerSec;
	SecPerClus = bpb_ptr->BPB_SecPerClus;
	RsvdSecCnt = bpb_ptr->BPB_RsvdSecCnt;
	NumFATs = bpb_ptr->BPB_NumFATs;
	RootEntCnt = bpb_ptr->BPB_RootEntCnt;
	//FATSz���ܳ���������
	if (bpb_ptr->BPB_FATSz16 != 0)
	{
		FATSz = bpb_ptr->BPB_FATSz16;
	}
	else
	{
		FATSz = bpb_ptr->BPB_TotSec32;
	}

	string *filenames=(string*)new string[RootEntCnt];
	int *filelocation = (int*)new int[RootEntCnt];         //�����ļ�����
	memset(filelocation, -1, RootEntCnt);              //��ʼ��Ϊ-1

	getFilename(fat12,rootEntry_ptr,filelocation);

	int flag,idx = 0;
	
	while (filelocation[idx] != -1)
	{
		char* name = new char[12];
		flag = fseek(fat12, (RsvdSecCnt + NumFATs * FATSz) * BytsPerSec + filelocation[idx] * 32, SEEK_SET);
		if (flag == -1)              //������
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