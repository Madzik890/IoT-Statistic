#ifndef SERVERCALLBACK_HPP
#define SERVERCALLBACK_HPP

#include <string>
#include <vector>
#include "../lib/pugixml/pugixml.hpp"
#include "./Device/Device.hpp"

extern int (*dataBaseSave)(const Device device);
extern int (*dataBaseLogSave)(const std::string guid, const std::string log);
int callDataBaseSave(const Device device);
int callDataBaseLogSave(const std::string guid, const std::string log);

class GuidList
{
protected:
    const std::string generateID();
public:
    GuidList();
    ~GuidList();
    
    bool FindGuid(const std::string guid);
    const std::string AddSession(const std::string guid);
private:
    std::vector<std::string> v_guid;
};

std::string DeviceInfoReceived(std::string buffer);//callback function

#endif /* SERVERCALLBACK_HPP */

