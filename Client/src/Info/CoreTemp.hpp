#ifndef CORETEMP_HPP
#define CORETEMP_HPP

#include "Info.hpp"
#include <fstream>

class CoreTemp
:public Info
{
protected:
    std::ifstream m_file;
public:
    CoreTemp();
    ~CoreTemp();
    
    const bool CheckInfo();
    const std::string ToString();
    
    const int GetCoreTemp();
private:
    int i_coreTemp;
};

#endif /* CORETEMP_HPP */

