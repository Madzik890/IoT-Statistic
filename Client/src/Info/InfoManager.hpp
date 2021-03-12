#ifndef INFOMANAGER_HPP
#define INFOMANAGER_HPP

#include "Info.hpp"
#include <vector>

class InfoManager
{
public:
    InfoManager(const char *GUID);
    ~InfoManager();
    
    void AddInfo(Info* info);
    void GetInfo();
    const std::string GetXML();
private:
    std::vector<Info*> m_infoVector;
    std::string s_guid;
};

#endif /* INFOMANAGER_HPP */

