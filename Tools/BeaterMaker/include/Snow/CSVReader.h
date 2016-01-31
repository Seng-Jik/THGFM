<<<<<<< HEAD
#pragma once
#include "ResFile.h"
#include <string>

namespace Snow{
class CSVReader
{
    public:
        bool LoadCSV(const std::string&);
        void Close();
        void ResetPtr();
        bool PopStr(std::string&);
        bool PopInt(int&);
        bool PopFloat(double&);
        bool NextLine();
    protected:
    private:
        Snow::ResFile m_file;
        Uint32 m_ptr;
        std::string m_nowLine;
};
}

=======
#pragma once
#include "ResFile.h"
#include <string>

namespace Snow{
class CSVReader
{
    public:
        bool LoadCSV(const std::string&);
        void Close();
        void ResetPtr();
        bool PopStr(std::string&);
        bool PopInt(int&);
        bool PopFloat(double&);
        bool NextLine();
    protected:
    private:
        Snow::ResFile m_file;
        Uint32 m_ptr;
        std::string m_nowLine;
};
}

>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
