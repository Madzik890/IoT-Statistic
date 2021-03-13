#ifndef LOGINFO_HPP
#define LOGINFO_HPP

#include <fstream>
#include "Info.hpp"

class LogInfo
{
public:
    LogInfo(const std::string guid);
    ~LogInfo();
    
    const std::string ToString();
    
    void SetLogSentID(const std::string id);
    const std::string GetLogSentID();
private:
    std::string getLog();
    std::string s_guid;
    std::string s_logSentID;
};


#endif /* LOGINFO_HPP */

