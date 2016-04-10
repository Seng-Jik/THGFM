#include "Snow/ResFile.h"
//#include "Snow/Error.h"
#include <fstream>
#include "Snow/Debug.h"

using namespace Snow;
using namespace std;

LocalFileReader Snow::ResFile::m_localLoader;
std::vector<ResFileReader*> Snow::ResFile::m_readers;

void ResFile::fixPath(string& path)
{
    for(unsigned int i = 0;i < path.length();++i){
        if(path[i] == '/') path[i] = '\\';
        else if(path[i] >= 'a' && path[i] <= 'z') path[i] += 'A' - 'a';
    }
}


char* LocalFileReader::GetFile(const string& f,Uint32& size)
{
    ifstream in(f.c_str(),ios::binary);
    if(!in) return nullptr;

    in.seekg(0,ios::end);
    size = in.tellg();
    in.seekg(0,ios::beg);

    char* mem = nullptr;
    try{
        mem = new char[size];
    }catch(bad_alloc){
        return nullptr;
    }

    in.read(mem,size);
    return mem;
}

void ResFile::InstallReader(ResFileReader* r){
    m_readers.push_back(r);
}

ResFile::ResFile(){}

ResFile::ResFile(const std::string& s)
{
    Load(s);
}

ResFile::~ResFile()
{
    Free();
}

bool ResFile::Load(std::string f)
{
    fixPath(f);
    Uint32 size;
    char* mem;
    for(auto p = m_readers.begin();p != m_readers.end();++p){
        mem = (*p) -> GetFile(f,size);
        if(mem != nullptr){
            m_mem = mem;
            m_size =  size;
            return true;
        }
    }

    mem = m_localLoader.GetFile(f,size);
    if(mem != nullptr){
        m_mem = mem;
        m_size = size;
        return true;
    }

    PNT(string("Snow::ResFile::Load:Can't Open ResFile ")+f);
    return false;
}

void ResFile::Free()
{
    if(m_rw != nullptr)
    {
        SDL_FreeRW(m_rw);
        m_rw = nullptr;
        //PNT("SDLRWFree:"<<dbg_info<<std::endl);
    }
    if(m_mem != nullptr)
    {
        delete [] m_mem;
        m_mem = nullptr;
       // PNT("SDLSPACEFree:"<<dbg_info<<std::endl);
    }
    m_size = 0;
}

Uint32 ResFile::Size() const
{
    return m_size;
}

bool ResFile::Success() const
{
    return m_mem != nullptr;
}

ResFile::operator Uint8*() const
{
    return (Uint8*)m_mem;
}

ResFile::operator SDL_RWops*()
{
    if(m_rw == nullptr){
        m_rw = SDL_RWFromMem((void*)m_mem,m_size);
    }
    return m_rw;
}

ResFile::operator void*() const
{
    return (void*)m_mem;
}

ResFile::operator char*() const
{
    return m_mem;
}

Uint8& ResFile::operator[](Uint32 s) const
{
    return *((Uint8*)m_mem+s);
}
