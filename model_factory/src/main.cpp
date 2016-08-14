/* 
 * File:   main.cpp
 * Author: bzhao
 *
 * Created on 10 August 2016, 23:03
 */

#include <iostream>
#include <map>

using namespace std;


class Model
{
public:
    Model(){}
    virtual ~Model() {}

    virtual bool Get() = 0;
};

class ModelA : public Model
{
public:
    bool Get()
    {
        return true;
    }
};

class ModelB : public Model
{
public:
    bool Get()
    {
        return false;
    }
};

template <typename T>
class Creator
{
public:
    virtual ~Creator(){}
    virtual T* Create() = 0;
};

// DerivedCreator is Creator<BaseType> which creates a 
// DerivedType, not a Creator<DerivedType>
template <typename DerivedType, typename BaseType>
class DerivedCreator : public Creator<BaseType>
{
public:
    BaseType* Create()
    {
        return new DerivedType;
    }
};

template <typename T, typename Key>
class Factory
{
public:
    void reg(Key Id, Creator<T>* Fn)
    {
        FunctionMap[Id] = Fn;
    }

    T* create(Key Id)
    {
        return FunctionMap[Id]->Create();
    }

    ~Factory()
    {
        typename std::map<Key, Creator<T>*>::iterator i = FunctionMap.begin();
        while (i != FunctionMap.end())
        {
            delete (*i).second;
            ++i;
        }
    }
private:
    std::map<Key, Creator<T>*> FunctionMap;
};

int main(int argc, char ** argv) {
    // Register
    Factory<Model, std::string> temp;
    temp.reg("ModelA", new DerivedCreator<ModelA, Model>);
    temp.reg("ModelB", new DerivedCreator<ModelB, Model>);

    //Pointer to base interface
    Model * pBase = 0;

    //Create and call
    pBase = temp.create("ModelA");
    cout << "DerivedA " << pBase->Get() << endl;
    delete pBase;

    //Create and call
    pBase = temp.create("ModelB");
    cout << "DerivedB " << pBase->Get() << endl;
    delete pBase;

    return 0;
}

