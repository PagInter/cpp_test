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

template <class T>
class Creator
{
public:
    virtual ~Creator(){}
    virtual T* Create() = 0;
};

// DerivedCreator is Creator<BaseType> which creates a 
// DerivedType, not a Creator<DerivedType>
template <class DerivedType, class BaseType>
class DerivedCreator : public Creator<BaseType>
{
public:
    BaseType* Create()
    {
        return new DerivedType;
    }
};

template <class T, class Key>
class Factory
{
public:
    void Register(Key Id, Creator<T>* Fn)
    {
        FunctionMap[Id] = Fn;
    }

    T* Create(Key Id)
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
    temp.Register("DA", new DerivedCreator<ModelA, Model>);
    temp.Register("DB", new DerivedCreator<ModelB, Model>);

    //Pointer to base interface
    Model * pBase = 0;

    //Create and call
    pBase = temp.Create("DA");
    cout << "DerivedA " << pBase->Get() << endl;
    delete pBase;

    //Create and call
    pBase = temp.Create("DB");
    cout << "DerivedB " << pBase->Get() << endl;
    delete pBase;

 return 0;
}

