#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include "../Device/Device.hpp"

int saveXML(Device device);
int saveXMLLog(std::string guid, std::string log);

#endif /* DATABASE_HPP */

