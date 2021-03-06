/* 
 * File:   main.cpp
 * Author: bzhao
 *
 * Created on 10 August 2016, 23:03
 */

#include <iostream>
#include <map>

using namespace std;


class Model {
public:
    Model(){}
    virtual ~Model() {}

    virtual bool get_bool() = 0;
    
    virtual void test() = 0;
};

class ModelA : public Model {
public:
    bool get_bool() {
        return true;
    }
    
    void test() {
        cout << "ModelA" << endl;
    }
};

class ModelB : public Model {
public:
    bool get_bool() {
        return false;
    }
    
    void test() {
        cout << "ModelB" << endl;
    }
};

template <typename T>
class Creator
{
public:
    virtual ~Creator(){}
    virtual T * create_derived_model() = 0;
};

// DerivedCreator is Creator<BaseType> which creates a 
// DerivedType, not a Creator<DerivedType>
template <typename DerivedType, typename BaseType>
class DerivedCreator : public Creator<BaseType>
{
public:
    BaseType * create_derived_model() {
        return new DerivedType;
    }
};

template <typename Key, typename T>
class Factory
{
public:
    void reg(Key Id, Creator<T>* Fn) {
        _m_function_map[Id] = Fn;
    }

    T * create(Key Id) {
        return _m_function_map[Id]->create_derived_model();
    }
 
    ~Factory() {
        //typename std::map<Key, Creator<T>*>::iterator i = FunctionMap.begin();
        auto i = _m_function_map.begin();
        while (i != _m_function_map.end()) {
            delete (*i).second;
            ++i;
        }
    }
    
private:
    std::map<Key, Creator<T>*> _m_function_map;
};

int main(int argc, char ** argv) {
    // Register
    Factory<std::string, Model> modelFactory;
    modelFactory.reg("ModelA", new DerivedCreator<ModelA, Model>);
    modelFactory.reg("ModelB", new DerivedCreator<ModelB, Model>);

    //Pointer to base interface
    Model * pBase = nullptr;

    //Create and call
    pBase = modelFactory.create("ModelA");
    cout << "DerivedA " << pBase->get_bool() << endl;
    pBase->test();
    delete pBase;

    //Create and call
    pBase = modelFactory.create("ModelB");
    cout << "DerivedB " << pBase->get_bool() << endl;
    pBase->test();
    delete pBase;

    return 0;
}

