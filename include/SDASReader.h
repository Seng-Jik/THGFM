#ifndef SDASREADER_H
#define SDASREADER_H

#include "Snow.h"
#include <inttypes.h>
#include <cstdio>
#include <map>

class SDASReader : public Snow::ResFileReader
{
    public:
        void OpenSDAS(const char* pkg);
        char* GetFile(const std::string&,Uint32& size);
    protected:
    private:
        Uint64 hashStr(const char*);
        struct FileInfo{
            uint64_t fileNameHash;
            uint32_t beg,size;
        };
        std::map<Uint64,FileInfo> m_infos;
        FILE* m_pkg;
};

#endif // SDASREADER_H
