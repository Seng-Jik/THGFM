#pragma once
#include <map>

namespace Snow{

class ResVal
{
    public:
        bool Load(const std::string&);
        Sint32 Int(std::string);
        float Float(std::string);
        const std::string& Str(std::string);
    private:
        std::map<std::string,Sint32> m_ints;
        std::map<std::string,float> m_flts;
        std::map<std::string,std::string> m_strs;

<<<<<<< HEAD
        std::string Trim(const std::string& s);
=======
        std::string Trim(const std::string& s);
>>>>>>> 720c0b59a97be74f02a7c85da646f3f62bece3a5
        const std::string m_nullstr;

};

}
