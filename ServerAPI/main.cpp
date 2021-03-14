#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <linux/kernel.h>
#include "src/API.hpp"

using namespace std;

void CheckServer();
void GetDevices();
void GetDevicesLog();
void ShowDevice(const std::string guid);
void ShowAllDevices();
void ShowDeviceLog(const std::string guid);
void ShowAllDevicesLogs();

void HelpArgv()
{
    printf("Użycie ServerAPI [-gd] [-gdl] [-sd] [-sdl] [-sda] [-sdla]\n");

    printf("\nOpcje: \n");
    printf("   -cs               Sprawdza czy serwer jest wlaczony. \n");
    printf("   -gd               Sprawdza liste urzadzen ktore zapisaly informacje systemowe. \n");
    printf("   -gdl              Sprawdza liste urzadzen ktore zapisaly logi systemowe. \n");
    printf("   -sd               Wyswietla informacje systemowe podanego urzadzenia. \n");
    printf("   -sdl              Wyswietla logi systemowe podanego urzadzenia. \n");
    printf("   -sda              Wyswietla informacje systemowe wszystkich urzadzen. \n");
    printf("   -sdla             Wyswietla logi systemowe wszystkich urzadzen. \n");
}

int main(int argc, char** argv) 
{
    
    if(argc > 1)
    {       
        for(int i = 1; i <= argc - 1;i++)
        {
            if(!strcmp( argv[ i ], "-help" ) || !strcmp( argv[ i ], "--help" ))
            {
                HelpArgv();
                break;
            }
            
            if(!strcmp( argv[ i ], "-cs" ))
                CheckServer();
            else
            if(!strcmp( argv[ i ], "-gd" ))
                GetDevices();
            else
            if(!strcmp( argv[ i ], "-gdl" ))
                GetDevicesLog();
            else
            if(!strcmp( argv[ i ], "-sd" ) && argc >= (i+1))
                ShowDevice(argv[i+1]);
            else
            if(!strcmp( argv[ i ], "-sdl" ) && argc >= (i+1))
                ShowDevice(argv[i+1]);
            else
            if(!strcmp( argv[ i ], "-sda" ))
                ShowAllDevices();
            else
            if(!strcmp( argv[ i ], "-sdla" ))
                ShowAllDevicesLogs();
        }
    }
    else
        HelpArgv();

    return EXIT_SUCCESS;
}

void CheckServer()
{
    const int status = CheckServerStatus();
    
    if(status != -1)
        std::cout << "Serwer jest wlaczony" << std::endl;
    else
        std::cout << "Server nie jest wlaczony" << std::endl;
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
