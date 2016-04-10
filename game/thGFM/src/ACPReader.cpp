#include "ACPReader.h"
#include <cstdlib>
#include <iostream>


/*********** cpp **********/
using namespace std;

static Uint32 BKDRHash(const char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*str)
          hash = hash * seed + (*str++);
    return (hash & 0x7FFFFFFF);
}

static void decode(char* data,Uint32 size){
    Uint8* pData = (Uint8*)data;
    if(size > 65536) size = 65536;
    Uint8 init = 0x84;
    for(Uint32 i = 0;i < size;++i){
        pData[i]^=init;
        init = pData[i];
    }
}

bool ACPReader::OpenPkg(const char* pkg){
     m_pkg.open(pkg,ios::binary|ios::in);
     printf("OpenPkg:Package Opened:%d\n",!m_pkg.fail());
     Uint32 fileNum = 1;
     m_pkg.read((char*)&fileNum,sizeof(Uint32));
     printf("OpenPkg:Got File Nums:%d\n",fileNum);
     //m_pkg.seekg(0,ios::end);
     //m_pkg.seekg(-fileNum*sizeof(FileInfo),ios::cur);
     for(Uint32 i = 0;i < fileNum;++i){
                FileInfo info;
                m_pkg.read((char*)&info,sizeof(info));
                //printf("OpenPkg:Open File Size:%d\n",info.size);
                m_hashes[info.nameHash] = info;
     }
     return true;
}

char* ACPReader::GetFile(const std::string& fileName,Uint32& size){
      Uint32 nameHash = BKDRHash(fileName.c_str());
      if(m_hashes.count(nameHash) == 0) return nullptr;
      //printf("Debug 0\n");
      FileInfo& info = m_hashes[nameHash];
      size = info.size;
      m_pkg.seekg(info.beg,ios::beg);
      char* mem = new char[info.size];
      m_pkg.read(mem,info.size);
      decode(mem,info.size);
      return mem;
}

