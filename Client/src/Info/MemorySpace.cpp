#include "MemorySpace.hpp"

MemorySpace::MemorySpace()
:u_memoryTotal(0), u_memoryAvaible(0), u_memoryFree(0)
{
}

MemorySpace::~MemorySpace()
{
    u_memoryTotal = 0;
    u_memoryAvaible = 0;
    u_memoryFree = 0;
}

const bool MemorySpace::CheckInfo()
{
    std::string s_buffer;
    std::string::size_type sz_stringSize = 0;
    m_file.open(MEMORY_COMMAND);
    if(m_file.is_open())
    {
        while (!m_file.eof()) 
        {
            m_file >> s_buffer;
            if(s_buffer.compare(MEMORY_PARAM_TOTAL) == 0)
            {
                m_file >> s_buffer;
                u_memoryTotal = std::stoull (s_buffer,&sz_stringSize,0);
            }
            else
            if(s_buffer.compare(MEMORY_PARAM_FREE) == 0)
            {
                m_file >> s_buffer;
                u_memoryFree = std::stoull (s_buffer,&sz_stringSize,0);
            }
            else
            if(s_buffer.compare(MEMORY_PARAM_AVAIL) == 0)
            {
                m_file >> s_buffer;
                u_memoryAvaible = std::stoull (s_buffer,&sz_stringSize,0);
                 
            }
        }
    }
    else
        return false;

    m_file.close(); 
    return true;
}

const std::string MemorySpace::ToString()
{
    std::string s_result = "";
    
    s_result += "   <MemorySpace>\n";
    s_result += "       <Total>" + std::to_string(u_memoryTotal) + "</Total>\n";
    s_result += "       <Avaible>" + std::to_string(u_memoryAvaible) + "</Avaible>\n";
    s_result += "       <Free>" + std::to_string(u_memoryFree) + "</Free>\n";
    s_result += "   </MemorySpace>\n";
    
    return s_result;
}

const ull MemorySpace::GetMemoryTotal()
{
    return u_memoryTotal;
}

const ull MemorySpace::GetMemoryAvaible()
{
    return u_memoryAvaible;
}

const ull MemorySpace::GetMemoryFree()
{
    return u_memoryFree;
}
