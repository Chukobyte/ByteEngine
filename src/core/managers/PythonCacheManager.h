#ifndef PYTHONCACHEMANAGER_H
#define PYTHONCACHEMANAGER_H

class PythonCacheManager {
  public:
    std::map<std::string, CPyObject> modules;
    std::map<std::string, CPyObject> classes;

    CPyObject GetModule(std::string modulePath) {
        return modules[modulePath];
    }

    bool HasModule(std::string modulePath) {
        return modules.count(modulePath) > 0;
    }

    CPyObject GenerateModuleClass(std::string modulePath, std::string className) {
        if(modules.count(modulePath) <= 0) {
            CPyObject pName = PyUnicode_FromString(modulePath.c_str());
            CPyObject pModule = PyImport_Import(pName);
            modules.emplace(modulePath, pModule);
        }
        std::string fullClassName = modulePath + className;
        if(classes.count(fullClassName) <= 0) {
            CPyObject moduleDict = PyModule_GetDict(modules[modulePath]);
            CPyObject classObj = PyDict_GetItemString(moduleDict, className.c_str());
            classes.emplace(fullClassName, classObj);
        }
        return classes[fullClassName];
    }

};

#endif
