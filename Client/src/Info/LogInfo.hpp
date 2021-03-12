#ifndef LOGINFO_HPP
#define LOGINFO_HPP

#include <fstream>
#include "Info.hpp"

class LogInfo
{
public:
    LogInfo();
    ~LogInfo();
    
    const std::string ToString(const std::string guid);
private:
    std::string getLog();
};


#endif /* LOGINFO_HPP */

