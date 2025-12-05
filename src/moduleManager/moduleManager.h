#pragma once
#include "json.hpp"
#include <string>
#include <vector>
struct ModuleManifest
{
    std::string moduleName;
    std::string moduleVersion;
    std::vector<std::string> dependencies;
    std::vector<std::string> entrySrc;
};

class ModuleManager{
    public:
    void loadModule(size_t moduleId);
    void unloadModule(size_t moduleId);
    void moduleInfo(size_t moduleId);
    private:
    std::vector<std::string> modules;
};