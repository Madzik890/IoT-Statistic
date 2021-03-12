#ifndef DISKSPACE_HPP
#define DISKSPACE_HPP

#include "Info.hpp"

class DiskSpace
:public Info
{
public:
    DiskSpace();
    ~DiskSpace();
    
    const bool CheckInfo();
    const std::string ToString();
    
    const ull GetDiskTotal();
    const ull GetDiskFree();
private:
    ull u_diskTotal;
    ull u_diskFree;
};

#endif /* DISKSPACE_HPP */

