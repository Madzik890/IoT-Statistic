#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

typedef unsigned long long ull;

#define MEMORY_COMMAND          "/proc/meminfo"
#define MEMORY_PARAM_TOTAL      "MemTotal:"
#define MEMORY_PARAM_FREE       "MemFree:"
#define MEMORY_PARAM_AVAIL      "MemAvailable:"

#define DISK_COMMAND            "/"

#define CORE_COMMAND            "/proc/acpi/thermal_zone/THRM/temperature"

#define CLIENT_MAX_BUFFER       65535
#define CLIENT_CONNECT_ERROR    -1

#define LOG_COMMAND             "dmesg"

#endif /* DEFINITIONS_HPP */

