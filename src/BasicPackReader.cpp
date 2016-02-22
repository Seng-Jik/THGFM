#include "BasicPackReader.h"
#include<cstdlib>
#include <conio.h>
#include "Snow/Debug.h"

void BasicPackReader::OpenPkg(const char* pkg)
{
    m_pkg = fopen(pkg,"rb");
    PNT("OFFSET:"<<ftell(m_pkg));
    if(!m_pkg) exit(-666);

    m_infos.clear();
    fseek(m_pkg,-sizeof(uint32_t),SEEK_END);
    uint32_t fileNum=12;
    fread((char*)(&fileNum),1,sizeof(uint32_t),m_pkg);

    PNT("COUNT:"<<fileNum<<"  offset:"<<ftell(m_pkg));

    fseek(m_pkg,-sizeof(Uint32)-fileNum*sizeof(FileInfo),SEEK_END);
    FileInfo fi;
    for(Uint32 i = 0;i < fileNum;++i){
        fread((char*)&fi,sizeof(fi),1,m_pkg);
        m_infos[fi.fileNameHash] = fi;
        PNT("HASH:"<<fi.fileNameHash);
    }
}

char* BasicPackReader::GetFile(const std::string& s, Uint32& size)
{
    const FileInfo& fi = m_infos[hashStr(s.c_str())];
    size = fi.size;
    char* buf = new char [size];
    fseek(m_pkg,fi.beg,SEEK_SET);
    fread(buf,size,1,m_pkg);

    //TODO:Decode
    Uint32 encSize = 16384;
    if(size < encSize) encSize = size;
    decode(buf,encSize);
    return buf;
}

Uint64 BasicPackReader::hashStr(const char* str)
{
    Uint64 hash = 0;
    while (*str)
    {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

void BasicPackReader::decode(char* buf, int size)
{
    for(int i = 0;i < size;++i)
        buf[i] =~buf[i];
}
