#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include "../Device/Device.hpp"

int saveXML(const Device device);
int saveXMLLog(const std::string guid, const std::string log);

#endif /* DATABASE_HPP */

