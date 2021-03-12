#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <string>
#include "Definitions.hpp"

struct Info
{
public: 
    virtual const bool CheckInfo() = 0;
    virtual const std::string ToString() = 0;
};
#endif /* STATISTICS_HPP */

