#include "InfoManager.hpp"

InfoManager::InfoManager(const char *GUID) 
:s_guid(GUID)
{

}

InfoManager::~InfoManager()
{
    m_infoVector.resize(0);
    m_infoVector.clear();
}

void InfoManager::AddInfo(Info* info)
{
    m_infoVector.push_back(info);
}

void InfoManager::GetInfo()
{
    for(int i = 0; i < m_infoVector.size(); i++)
        m_infoVector[i]->CheckInfo();
}

const std::string InfoManager::GetXML()
{
    std::string s_result;
    s_result = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    s_result += "<DeviceInfo>\n";
    s_result += "   <GUID>\n";
    s_result += "       <Value>" + s_guid + "</Value>\n";
    s_result += "   </GUID>\n";
    
    for(int i = 0; i < m_infoVector.size(); i++)
        s_result += m_infoVector[i]->ToString();
    
    s_result += "</DeviceInfo>\n";
    return s_result;
}

