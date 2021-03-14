#ifndef API_HPP
#define API_HPP

#include "Device/Device.hpp"
#include <vector>
#include <string>

bool checkServerStatus();

const std::vector<std::string> CheckDevices();
const std::vector<std::string> CheckDevicesLogs();

const Device GetDevice(const std::string guid);
const std::string GetDeviceLog(const std::string guid);

#endif /* API_HPP */

