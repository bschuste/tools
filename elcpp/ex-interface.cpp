/*
 * ex-interface.cpp
 *
 *  Created on: May 26, 2017
 *      Author: brigitte
 */

class IBase {
public:
    virtual ~IBase() {}; // destructor, use it to call destructor of the inherit classes
    virtual void Describe() = 0; // pure virtual method
};

class Tester : public IBase {
public:
    Tester(std::string name);
    virtual ~Tester();
    virtual void Describe();
private:
    std::string privatename;
};

Tester::Tester(std::string name) {
    std::cout << "Tester constructor" << std::endl;
    this->privatename = name;
}

Tester::~Tester() {
    std::cout << "Tester destructor" << std::endl;
}

void Tester::Describe() {
    std::cout << "I'm Tester [" << this->privatename << "]" << std::endl;
}


void descriptor(IBase * obj) {
    obj->Describe();
}

int main(int argc, char** argv) {

    std::cout << std::endl << "Tester Testing..." << std::endl;
    Tester * obj1 = new Tester("Declared with Tester");
    descriptor(obj1);
    delete obj1;

    std::cout << std::endl << "IBase Testing..." << std::endl;
    IBase * obj2 = new Tester("Declared with IBase");
    descriptor(obj2);
    delete obj2;

    // this is a bad usage of the object since it is created with "new" but there are no "delete"
    std::cout << std::endl << "Tester not defined..." << std::endl;
    descriptor(new Tester("Not defined"));


    return 0;
}


