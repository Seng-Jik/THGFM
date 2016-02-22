#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cinttypes>
#include <conio.h>
#include <list>
#include <set>
#include <fstream>
#include <iostream>
#include <windows.h>
using namespace std;

struct FileInfo{
    uint64_t fileNameHash;
    uint32_t beg,size;
};


uint64_t HashString(const char* str){
    uint64_t hash = 0;
    while (*str)
    {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

bool FindAllFile(vector<string>& v,LPCTSTR Path)
{
    static char DePath[MAX_PATH]="|";
	char RelPath[MAX_PATH],						//store the RelPath,avoid the RelPath in the next function to be changed
		 FindRes[MAX_PATH],						//Path for dirs or files
		 AbsDir[MAX_PATH],						//the absolute path for dirs
		 CurFile[MAX_PATH];						//the relative path for files
	HANDLE hFindRes;
	WIN32_FIND_DATA wfd;

//----------------------------------------------------------------------------
//structure the FindRes and make up the absolute path which was made up of DePath and RelPath for dir.
//----------------------------------------------------------------------------
	if(DePath[0]=='|')
	{
		strcpy(DePath,Path);							//store the FirstPath
		strcpy(AbsDir,DePath);
		sprintf(FindRes,"%s\\*",DePath);
	}
	else {
		sprintf(AbsDir,"%s\\%s",DePath,Path);
		sprintf(FindRes,"%s\\*",AbsDir);
	}
	hFindRes=FindFirstFile(FindRes,&wfd);

	if(hFindRes==INVALID_HANDLE_VALUE) {
		printf("error:%d",GetLastError());
		return 1;
	}
//	puts(AbsDir);
	do {
		if(wfd.cFileName[0]=='.')
			continue;
//----------------------------------------------------------------------------------
//	Dir
//----------------------------------------------------------------------------------
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if(strcmp(AbsDir,DePath)==0)
				sprintf(RelPath,"%s",wfd.cFileName);
			else
			sprintf(RelPath,"%s\\%s",Path,wfd.cFileName);

			//puts(RelPath);
			FindAllFile(v,RelPath);
		}
//----------------------------------------------------------------------------------
//	File
//----------------------------------------------------------------------------------
		else {
			if(strcmp(AbsDir,DePath)!=0)
			sprintf(CurFile,"%s\\%s",Path,wfd.cFileName);
			else
			sprintf(CurFile,"%s",wfd.cFileName);

#ifdef Test
			file++;
#endif

			v.push_back(string(CurFile));
		}

	}while(FindNextFile(hFindRes,&wfd));
	FindClose(hFindRes);
	return 0;
}

void help(){
    printf("sdas _sourceDir targetDir\n");
    exit(0);
};

size_t GetSize(ifstream& in){   //取得文件大小
    in.seekg(0,ios::end);
    size_t ret = in.tellg();
    in.seekg(0,ios::beg);
    return ret;
}

void fixPath(string& path) //格式化路径字符串
{
    for(unsigned int i = 0;i < path.length();++i){
        if(path[i] == '/') path[i] = '\\';
        else if(path[i] >= 'a' && path[i] <= 'z') path[i] += 'A' - 'a';
    }
}

void EncodeBuf(char* buf,int size){
    //TODO:Encode here.
    for(int i = 0;i < size;++i)
        buf[i] = ~buf[i];
}

set<uint64_t> hashSet;
list<FileInfo> fileInfos;
void PushFile(string& dir,string& s,ofstream& out){ //处理单个文件
    printf("PushFile:%s...",s.c_str());
    ifstream in((dir+'\\'+s).c_str(),ios::binary);
    if(!in){
        printf("failed.\n");
        getch();
        exit(-1);
    }

    fixPath(s);
    uint64_t hash = HashString(s.c_str());
    if(hashSet.count(hash)){
        printf("Hash error!\n");
        getch();
        exit(-1);
    }
    cout<<"Hash:"<<hash<<endl;
    hashSet.insert(hash);
    FileInfo fileInfo = {hash,out.tellp(),GetSize(in)};
    fileInfos.push_back(fileInfo);

    static char buf[128*1024*1024];
    in.read(buf,fileInfo.size);
    in.close();

    int encSize = 16384;
    if(fileInfo.size < encSize) encSize = fileInfo.size;
    EncodeBuf(buf,encSize);

    out.write(buf,fileInfo.size);
    printf("OK.\n");
}

int main(int argc,char** argv)
{
    //TODO:判断参数是否合法，如果合法则进行文件打包，否则输出英文的使用说明。
    if(argc != 3) help();
    //查找文件
    vector<string> files;
    FindAllFile(files,argv[1]);

    //建立文件包
    ofstream out(argv[2],ios::binary);
    if(!out) exit(-1);

    string dir = argv[1];

    //创建数据区并创建文件表
    for(string& s:files)
        PushFile(dir,s,out);

    for(FileInfo& s:fileInfos)
        out.write((char*)&s,sizeof(FileInfo));

    uint32_t fileCount = fileInfos.size();
    out.write((char*)&fileCount,sizeof(uint32_t));
    out.close();
    cout<<"FileCount:"<<fileCount<<endl;
    return 0;
}
