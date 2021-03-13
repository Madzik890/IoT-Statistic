#include "LogInfo.hpp"

LogInfo::LogInfo(const std::string guid)
:s_guid(guid), s_logSentID("")
{
}

LogInfo::~LogInfo()
{
    
}

const std::string LogInfo::ToString()
{
    std::string s_result;
    s_result = "<DeviceLog>\n";
    s_result += "   <GUID>\n";
    s_result += "       <Value>" + s_guid + "</Value>\n";
    s_result += "   </GUID>\n";
    s_result += "   <Content>\"" + getLog() + "   \"</Content>\n";
    s_result += "</DeviceLog>\n";
    return s_result;
}

void LogInfo::SetLogSentID(const std::string id)
{
    s_logSentID = id;
}

const std::string LogInfo::GetLogSentID()
{
    return s_logSentID;
}

std::string LogInfo::getLog()
{
   std::string s_command = LOG_COMMAND;
    
   std::string s_data;
   FILE * m_stream;
   char s_buffer[CLIENT_MAX_BUFFER];
   s_command.append(" 2>&1");

   m_stream = popen(s_command.c_str(), "r");

   if (m_stream) 
   {
       while (!feof(m_stream))
        if (fgets(s_buffer, CLIENT_MAX_BUFFER, m_stream) != NULL) s_data.append(s_buffer);
       pclose(m_stream);
   }
   
   return s_data;
}

