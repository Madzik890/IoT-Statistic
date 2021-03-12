#ifndef MEMORYSPACE_HPP
#define MEMORYSPACE_HPP

#include <fstream>
#include "Info.hpp"

class MemorySpace
:public Info
{
protected:
    std::ifstream m_file;
public:
    MemorySpace();  
    ~MemorySpace();
    
    const bool CheckInfo();
    const std::string ToString();
    
    const ull GetMemoryTotal();
    const ull GetMemoryAvaible();
    const ull GetMemoryFree();
private:
    ull u_memoryTotal;
    ull u_memoryAvaible;
    ull u_memoryFree;
};


#endif /* MEMORYSPACE_HPP */

