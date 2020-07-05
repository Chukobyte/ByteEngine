#ifndef SCRIPTCLASSCOMPONENT_H
#define SCRIPTCLASSCOMPONENT_H

#include "./Component.h"
#include "TransformComponent.h"
#include "../helpers/pyhelper.hpp"
#include "../managers/PythonCacheManager.h"

class ScriptClassComponent : public Component {
  private:
    std::string modulePath;
    std::string className;
    CPyObject classInstance = NULL;
    PythonCacheManager* pythonCacheManager = NULL;
  public:
    ScriptClassComponent(PythonCacheManager* pythonCacheManager, const char* modulePath, const char* className) : pythonCacheManager(pythonCacheManager), modulePath(modulePath), className(className) {}

    ~ScriptClassComponent() {
        if(classInstance != NULL) {
            CPyObject destroyFunctionName = PyUnicode_FromString("__destroy__");
            if(PyObject_HasAttr(classInstance, destroyFunctionName)) {
                CPyObject destroyValue = PyObject_CallMethod(classInstance, "__destroy__", nullptr);
            }

            CPyObject deleteFunctionName = PyUnicode_FromString("__delete__");
            if(PyObject_HasAttr(classInstance, deleteFunctionName)) {
                CPyObject deleteValue = PyObject_CallMethod(classInstance, "__delete__", nullptr);
            }
        }
    }

    void Initialize() override {
        CPyObject cachedPythonClass = pythonCacheManager->GenerateModuleClass(modulePath, className);
        if(cachedPythonClass != NULL && PyCallable_Check(cachedPythonClass)) {
            int positionX = 0;
            int positionY = 0;
            if(owner->HasComponent<TransformComponent>()) {
                TransformComponent* transform = owner->GetComponent<TransformComponent>();
                positionX = transform->position.x;
                positionY = transform->position.y;
            }
            CPyObject arglist = Py_BuildValue("(sii)", owner->name.c_str(), positionX, positionY);
            classInstance = PyObject_CallObject(cachedPythonClass, arglist);
            classInstance.AddRef();
        } else {
            std::cout << "Error with loading cached class!" << std::endl;
            PyErr_Print();
        }
    }

    void Update(float deltaTime) override {
        CPyObject processFunctionName = PyUnicode_FromString("__process__");
        if(classInstance != NULL && PyObject_HasAttr(classInstance, processFunctionName)) {
            CPyObject processValue = PyObject_CallMethod(classInstance, "__process__", "(f)", deltaTime);
        }
        PyErr_Print();
    }

    void ExecuteFunction(std::string functionName) {
        const char* cPyFunctionName = functionName.c_str();
        CPyObject pyFunctionName = PyUnicode_FromString(cPyFunctionName);
        if(classInstance != NULL && PyObject_HasAttr(classInstance, pyFunctionName)) {
            CPyObject processValue = PyObject_CallMethod(classInstance, cPyFunctionName, nullptr);
            PyErr_Print();
        }
    }
};

#endif
