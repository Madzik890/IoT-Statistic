#include "API.hpp"
#include <dirent.h>
#include <exception>
#include <fstream>
#include <stdexcept>

int CheckServerStatus()
{
    int pid = -1;
    const std::string procName = SERVER_APP_NAME;

    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    return pid;
}
	
const std::vector<std::string> CheckDevices()
{
    std::vector<std::string> v_devices;
    DIR *m_dir; 
    struct dirent *m_diread;

    if ((m_dir = opendir(DEVICE_INFO_DIR)) != nullptr) 
    {
        while ((m_diread = readdir(m_dir)) != nullptr) {
            v_devices.push_back(m_diread->d_name);
        }
        closedir (m_dir);
        
        if(v_devices.size() > 0)
        {
            v_devices.erase(v_devices.begin() + (v_devices.size() - 1));
            v_devices.erase(v_devices.begin());
        }
    } 
    else 
        throw std::runtime_error("Cannot open the log's folder!");
    
    return v_devices;
}

const std::vector<std::string> CheckDevicesLogs()
{
    std::vector<std::string> v_devices;
    DIR *m_dir; 
    struct dirent *m_diread;

    if ((m_dir = opendir(DEVICE_LOG_DIR)) != nullptr) 
    {
        while ((m_diread = readdir(m_dir)) != nullptr) {
            v_devices.push_back(m_diread->d_name);
        }
        closedir (m_dir);
        
        if(v_devices.size() > 0)
        {
            v_devices.erase(v_devices.begin() + (v_devices.size() - 1));
            v_devices.erase(v_devices.begin());
        }
    } 
    else 
        throw std::runtime_error("Cannot open the log's folder!");
    
    return v_devices;
}

#include <iostream>
const Device GetDevice(const std::string guid)
{
    std::string s_guid;
    Core m_core;
    Disk m_disk;
    Memory m_memory;
    std::string::size_type sz = 0;
    std::fstream m_file;
    
    m_file.open(DEVICE_INFO_DIR + guid, std::ios_base::in);
    if(m_file.is_open())
    {
        std::string s_buffer;
        while(std::getline(m_file, s_buffer))
        {
            if(s_buffer.find("GUID: ") != std::string::npos)
                s_guid = std::string(s_buffer, 6);
            else
            if(s_buffer.find("MemorySpace Avaible: ") != std::string::npos)
                m_memory.u_memoryAvaible = std::stoull(std::string(s_buffer, 21), &sz, 0);
            else
            if(s_buffer.find("MemorySpace Free: ") != std::string::npos)
                m_memory.u_memoryFree = std::stoull(std::string(s_buffer, 18), &sz, 0);
            else
            if(s_buffer.find("DiskSpace Total: ") != std::string::npos)
                m_disk.u_diskTotal = std::stoull(std::string(s_buffer, 17), &sz, 0);
            else
            if(s_buffer.find("DiskSpace Free: ") != std::string::npos)
                m_disk.u_diskFree = std::stoull(std::string(s_buffer, 16), &sz, 0);
            else
            if(s_buffer.find("Core temp: ") != std::string::npos)
                m_core.i_coreTemp = std::atoi(std::string(s_buffer,11).c_str());
        }

        m_file.close();
        return Device(s_guid, m_core, m_memory, m_disk);
    }
    else
        throw std::runtime_error("Cannot open the info's file!");
}

const std::string GetDeviceLog(const std::string guid)
{
    std::string s_result;
    std::fstream m_file;
    m_file.open(DEVICE_LOG_DIR + guid, std::ios_base::in);
    if(m_file.is_open())
    {
        m_file.seekg(0, std::ios::end);
        size_t size = m_file.tellg();
        s_result = std::string(size, ' ');
        m_file.seekg(0);
        m_file.read(&s_result[0], size); 
        m_file.close();
    }
    else
        throw std::runtime_error("Cannot open the log's file!");
    
    
    return s_result;
}