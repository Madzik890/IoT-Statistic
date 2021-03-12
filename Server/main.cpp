#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include "src/ServerSSL.hpp"
#include "src/Device/Device.hpp"
#include "src/DataBase/DataBase.hpp"
#include "lib/pugixml/pugixml.hpp"

#define SSL_CERT              "./example.crt"          // location of SSL cert
#define KEY                   "./key.key"            // location of SSL key

int (*dataBaseSave)(const Device device) = nullptr;
int (*dataBaseLogSave)(const std::string guid, const std::string log) = nullptr;

int callDataBaseSave(const Device device)
{
    if(dataBaseSave != nullptr)
        return (*dataBaseSave)(device);
    else
        return CALLBACK_FUNCT_EMPTY;
}

int callDataBaseLogSave(const std::string guid, const std::string log)
{
    if(dataBaseSave != nullptr)
        return (*dataBaseLogSave)(guid, log);
    else
        return CALLBACK_FUNCT_EMPTY;
}

void DeviceInfoReceived(std::string buffer)
{
    pugi::xml_document m_doc;
    pugi::xml_parse_result b_result = m_doc.load_string(buffer.c_str(), pugi::parse_default|pugi::parse_declaration);
    if (b_result)
    {
        pugi::xml_node data = m_doc.document_element();
        if(strcmp(data.name(),"DeviceInfo") == 0)
        {
            std::string s_guid;
            Core m_core;
            Disk m_disk;
            Memory m_memory;
            std::string::size_type sz = 0;

            pugi::xpath_node xpathNode = m_doc.select_single_node("/DeviceInfo/GUID/Value/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                s_guid = selectedNode.value();
            }

            xpathNode = m_doc.select_single_node("/DeviceInfo/MemorySpace/Total/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_memory.u_memoryTotal = std::stoull(selectedNode.value(),&sz,0);
            }
        
            xpathNode = m_doc.select_single_node("/DeviceInfo/MemorySpace/Avaible/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_memory.u_memoryAvaible = std::stoull(selectedNode.value(),&sz,0);
            }
        
            xpathNode = m_doc.select_single_node("/DeviceInfo/MemorySpace/Free/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_memory.u_memoryFree = std::stoull(selectedNode.value(),&sz,0);
            }
         
         
            xpathNode = m_doc.select_single_node("/DeviceInfo/DiskSpace/Total/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_disk.u_diskTotal = std::stoull(selectedNode.value(),&sz,0);
            }
        
            xpathNode = m_doc.select_single_node("/DeviceInfo/DiskSpace/Free/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_disk.u_diskFree = std::stoull(selectedNode.value(),&sz,0);
            }
         
            xpathNode = m_doc.select_single_node("/DeviceInfo/CoreTemp/Temp/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                m_core.i_coreTemp = std::atoi(selectedNode.value());
            }
            
            Device m_device(s_guid, m_core, m_memory, m_disk);
            callDataBaseSave(m_device);
        }
        else
        if(strcmp(data.name(),"DeviceLog") == 0)
        {
            std::string s_guid;
            std::string s_content;
            
            pugi::xpath_node xpathNode = m_doc.select_single_node("/DeviceLog/GUID/Value/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                s_guid = selectedNode.value();
            }
            xpathNode = m_doc.select_single_node("/DeviceLog/Content/text()");
            if (xpathNode)
            {
                pugi::xml_node selectedNode = xpathNode.node();
                s_content = selectedNode.value();
            }
            
            callDataBaseLogSave(s_guid, s_content);
        }
        
    }
    else
        std::cout << "Parse error: " << b_result.description() << ", character pos= " << b_result.offset;
}

int main(int argc, char** argv)
{   
    ServerSSL m_server;
    
    int i_port;
    std::cout << "Podaj numer portu:";
    std::cin >> i_port;
    
    if (m_server.Init(i_port, SSL_CERT, KEY) == EXIT_SUCCESS)
    {
        m_server.AttachCallback(&DeviceInfoReceived);
        dataBaseSave = &saveXML;
        dataBaseLogSave = &saveXMLLog;
        
        while(1)    
        {
            int i_status = m_server.WaitForRequestAndProcess();
            if (i_status != SERVER_SUCCESS && i_status != SERVER_ERROR_WOULDBLOCK && i_status != SERVER_SSL_ERROR_ACCEPT &&
                i_status != SERVER_SSL_ERROR_FDSET && i_status != SERVER_SSL_ERROR_NEW)
            {
                m_server.Close();
                std::cout << "Error while processing web request" << std::endl;
                return EXIT_FAILURE;
            }
            usleep(10000);
        }
        m_server.Close();
        
    }
    return EXIT_SUCCESS;
}

