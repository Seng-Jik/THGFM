#ifndef BasicPacker_H
#define BasicPacker_H

#include "Snow.h"
#include <inttypes.h>
#include <cstdio>
#include <map>

class BasicPackReader : public Snow::ResFileReader
{
    public:
        void OpenPkg(const char* pkg);
        char* GetFile(const std::string&,Uint32& size);
    protected:
    private:
        void decode(char* buf,int size);
        Uint64 hashStr(const char*);
        struct FileInfo{
            uint64_t fileNameHash;
            uint32_t beg,size;
        };
        std::map<Uint64,FileInfo> m_infos;
        FILE* m_pkg;
};

#endif // BasicPacker_H
