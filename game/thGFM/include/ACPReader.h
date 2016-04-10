#ifndef ACPREADER_H
#define ACPREADER_H

#include <map>
#include <fstream>
#include <string>
#include "Snow.h"

typedef unsigned int Uint32;

class ACPReader:public Snow::ResFileReader{
public:
       bool OpenPkg(const char*);
       char* GetFile(const std::string&,Uint32& size);
private:
        struct FileInfo{
               Uint32 nameHash;
               Uint32 beg,size;
        };
        std::map<Uint32,FileInfo> m_hashes;
        std::ifstream m_pkg;
};


#endif // ACPREADER_H
