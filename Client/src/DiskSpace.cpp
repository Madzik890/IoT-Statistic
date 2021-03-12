#include "DiskSpace.hpp"
#include <sys/statvfs.h>

DiskSpace::DiskSpace()
:u_diskTotal(0), u_diskFree(0)
{
}

DiskSpace::~DiskSpace()
{
    u_diskTotal = 0;
    u_diskFree = 0;
}

const bool DiskSpace::CheckInfo()
{
    struct statvfs m_stat;
    if(!statvfs(DISK_COMMAND, &m_stat))
    {
        u_diskTotal = (ull)(m_stat.f_blocks * m_stat.f_frsize);
        u_diskFree = (ull)(m_stat.f_bfree * m_stat.f_frsize);
    }
    else
        return false;
    
    return true;
}

const std::string DiskSpace::ToString()
{
    std::string s_result = "";
    
    s_result += "<DiskSpace>\n";
    s_result += "   <Total>" + std::to_string(u_diskTotal) + "</Total>\n";
    s_result += "   <Free>" + std::to_string(u_diskFree) + "</Free>\n";
    s_result += "</DiskSpace>\n";
    
    return s_result;
}

const ull DiskSpace::GetDiskTotal()
{
    return u_diskTotal;
}

const ull DiskSpace::GetDiskFree()
{
    return u_diskFree;
}