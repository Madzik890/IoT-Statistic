#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "src/API.hpp"

using namespace std;

void GetDevices();
void GetDevicesLog();
void ShowDevice(const std::string guid);
void ShowAllDevices();
void ShowDeviceLog(const std::string guid);
void ShowAllDevicesLogs();

void HelpArgv()
{
    printf("Użycie ServerAPI [-gd] [-gDL] [-sd] [-sdl] [-sda] [-sdla]\n");

    printf("\nOpcje: \n");
    printf("   -cs               Sprawdza czy serwer jest wlaczony. \n");
    printf("   -gD               Sprawdza liste urzadzen ktore zapisaly informacje systemowe. \n");
    printf("   -gDL              Sprawdza liste urzadzen ktore zapisaly logi systemowe. \n");
    printf("   -sD               Wyswietla informacje systemowe podanego urzadzenia. \n");
    printf("   -sDL              Wyswietla logi systemowe podanego urzadzenia. \n");
    printf("   -sDA              Wyswietla informacje systemowe wszystkich urzadzen. \n");
    printf("   -sDLA             Wyswietla logi systemowe wszystkich urzadzen. \n");
}

int main(int argc, char** argv) 
{

    //HelpArgv();

    //GetDevices();
    //getDevicesLog();
    //std::cout << GetDeviceLog("4cbb4ef1-f36c-476d-bf50-ae04f65f8537") << std::endl;
    //ShowDevice("4cbb4ef1-f36c-476d-bf50-ae04f65f8537");
    //ShowAllDevices();
    ShowAllDevicesLogs();
    return 0;
}

void GetDevices()
{
    std::vector<std::string> devices;
            
    try
    {
        devices = CheckDevices();
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
        return;
    }

    std::cout << "Urzadzenia ktore zarejestrowaly informacje systemowe: " << std::endl;
    for(auto device : devices)
       std::cout << device << std::endl;
}

void GetDevicesLog()
{
    std::vector<std::string> devicesLog;
    
    try
    {
        devicesLog = CheckDevicesLogs();
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
        return;
    }
    
    std::cout << "Urzadzenia ktore zarejestrowaly logi systemowe: " << std::endl;
    for(auto device : devicesLog)
        std::cout << device << std::endl;
}

void ShowDevice(const std::string guid)
{
    try
    {
        Device m_device = GetDevice(guid);
        std::cout << "GUID: " << m_device.GetGuid() << std::endl;
        std::cout << "Dysk:" << std::endl;
        std::cout << "  Wolne miejsce: " << m_device.GetDisk().u_diskFree << " kB" << std::endl;
        std::cout << "  Dostepne miejsce: " << m_device.GetDisk().u_diskTotal << " kB" << std::endl;
        std::cout << "RAM:" << std::endl;
        std::cout << "  Wolne miejsce: " << m_device.GetMemory().u_memoryFree << " kB" << std::endl;
        std::cout << "  Dostepne miejsce: " << m_device.GetMemory().u_memoryAvaible << " kB" << std::endl;
        std::cout << "Procesor: " << std::endl;
        std::cout << "  Temperatura: " << m_device.GetCore().i_coreTemp << "*C" << std::endl;
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
    }
}

void ShowAllDevices()
{
    std::vector<std::string> devices;
            
    try
    {
        devices = CheckDevices();
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
        return;
    }

    std::cout << "Urzadzenia ktore zarejestrowaly informacje systemowe: " << std::endl;
    for(auto device : devices)
    {
        ShowDevice(device);
        std::cout << std::endl;
    }
    
}

void ShowDeviceLog(const std::string guid)
{
    try
    {
        std::string s_log = GetDeviceLog(guid);
        std::cout << "Log z urzadzenia: " << guid << std::endl;
        std::cout << s_log << std::endl;
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
    }
}

void ShowAllDevicesLogs()
{
    std::vector<std::string> devicesLog;
    
    try
    {
        devicesLog = CheckDevicesLogs();
    }
    catch(std::runtime_error error)
    {
        std::cout << "Wystapil blad: " << error.what() << std::endl;
        return;
    }
    
    std::cout << "Urzadzenia ktore zarejestrowaly logi systemowe: " << std::endl;
    for(auto device : devicesLog)
        ShowDeviceLog(device);
}
