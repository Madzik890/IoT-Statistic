#include "ServerCallback.hpp"
#include "Definitions.hpp"
#include <random>
#include <iostream>
#include <string.h>

GuidList g_guidList;

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

GuidList::GuidList()
:v_guid()
{
}

GuidList::~GuidList()
{
    v_guid.resize(0);
    v_guid.clear();
}

const std::string GuidList::generateID()
{
    unsigned int i_idLen = SERVER_LOG_ID_LEN;
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(i_idLen);

    while(i_idLen--)
        s += chrs[pick(rg)];

    return s;
}

bool GuidList::FindGuid(const std::string guid)
{
    for(int i = 0; i < v_guid.size(); i++)
    {
        if(!v_guid[i].compare(guid))
            return true;
    }
    return false;
}

const std::string GuidList::AddSession(const std::string guid)
{
    bool b_sessionAdded = false;

    if(FindGuid(guid) == false)
    {
        v_guid.push_back(guid);
        return generateID();
    }
    else
        return "";
}

std::string DeviceInfoReceived(std::string buffer)
{
    std::string s_result = "ERROR";
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
            s_result = "OK";
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
            
            if(!s_guid.empty())
            {
                if(!g_guidList.FindGuid(s_guid))
                {
                    callDataBaseLogSave(s_guid, s_content);
                    s_result = g_guidList.AddSession(s_guid);
                }
                else
                    s_result = SERVER_ERROR_OVERWRITE_STRING;
            }
            else
                s_result = SERVER_ERROR_GUID_STRING;
        }
        
    }

    return s_result;
}
