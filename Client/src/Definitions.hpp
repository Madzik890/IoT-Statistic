#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

typedef unsigned long long ull;

#define MEMORY_COMMAND                  "/proc/meminfo"
#define MEMORY_PARAM_TOTAL              "MemTotal:"
#define MEMORY_PARAM_FREE               "MemFree:"
#define MEMORY_PARAM_AVAIL              "MemAvailable:"

#define DISK_COMMAND                    "/"

#define CORE_COMMAND                    "/sys/class/hwmon/hwmon0/device/temp1_input"

#define CLIENT_MAX_BUFFER               65535
#define CLIENT_SUCCESS                  0x00000
#define CLIENT_ERROR                    0xaaaaa
#define CLIENT_ERROR_OVERWRITE          0x10000       
#define CLIENT_ERROR_CONNECT            -1
#define CLIENT_ERROR_HOSTNAME           0xc0001
#define CLIENT_ERROR_GUID               0xc0002

#define CLIENT_ERROR_GUID_STRING        "GUID_EMPTY"
#define CLIENT_ERROR_OVERWRITE_STRING   "OVERWRITE"

#define LOG_COMMAND                     "dmesg --facility=syslog"

#endif /* DEFINITIONS_HPP */

