#include<PhysicsFS\physfs.h>

class MyClass
{
public:
	MyClass();
	~MyClass();

private:

};

MyClass::MyClass()
{
	PHYSFS_init(nullptr);
}

MyClass::~MyClass()
{
}