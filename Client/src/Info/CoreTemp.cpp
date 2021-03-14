#include "CoreTemp.hpp"

CoreTemp::CoreTemp()
:i_coreTemp(0)
{
}

CoreTemp::~CoreTemp()
{
    i_coreTemp = 0;
}

const bool CoreTemp::CheckInfo()
{
    int i = 0;
    std::string s_buffer;
    m_file.open(CORE_COMMAND);
    if(m_file.is_open())
    {
        while (!m_file.eof()) 
        {
           m_file >> s_buffer;
           i_coreTemp = std::stoi(s_buffer) / 100;
           break;
        }
    }
    else
        return false;

    m_file.close(); 
    return true;
}

const std::string CoreTemp::ToString()
{
    std::string s_result = "";
    
    s_result += "   <CoreTemp>\n";
    s_result += "       <Temp>" + std::to_string(i_coreTemp) + "</Temp>\n";
    s_result += "   </CoreTemp>\n";
    
    return s_result;
}

const int CoreTemp::GetCoreTemp()
{
    return i_coreTemp;
}