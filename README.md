Reflection
==========
> C++11 header only reflection library

A runtime reflection library for inspecting and modyfiying objects at runtime. Primarly created for hot-reloading C++ code and using C++ as a scripting language while still maintaining native C++ speeds.


---------
Features
---------

* Auto Generated reflected header files
* Constructors
* Destructors
* Functions with any argument or return type (can be interpreted as generic uClass type)
* Members of any type (can be interpreted as generic uClass type)
* Inheritance

---------
Setup
---------

```cpp
  #include "reflection/reflection.hpp"

  // Create reflector object
  refl::reflector reflect = refl::reflector();
  // Set error callback
  reflect.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });
  // Set generated header output directory
  reflect.SetOutputDir((files::GetParentExecuteableDir(2) + std::string("tests/scripts/Generated/")).c_str());
  // Set source file relative path based on header output
  reflect.SetRelativeInclude("../");
  
  // Should clear classes from memory before each successive reload
  reflect.Clear();
  
  // load classes from header file
  reflect.LoadClasses(path.c_str());
  
  // Generate header files in specified location
  reflect.GenerateClasses();
  

```


---------
Usage
---------

Example Script.hpp
```cpp

UCLASS()
class MyBaseScript {
  public:
    UFUNCTION()
    void BaseFunction();
    UPROPERTY();
    int baseProperty = 1;
}

UCLASS()
class MyScript : public MyBaseScript {
  public:
   UCONSTRUCTOR()
   MyScript(int i, const char* s, void* ptr);
   
   UPROPERTY()
   int i = 10;
   UPROPERTY()
   char c = 'c';
   UPROPERTY()
   void* ptr = nullptr;
   UPROPERTY()
   const char* str = "hello world";
   UPROPERTY()
   std::vector<int> vec = {1,2,3,4,5,6};
   UPROPERTY()
   int& ref = i;
   
   UFUNCTION()
   void VoidFunc(int i, const char* str);
   
   UFUNCTION()
   int IntFunc(int i);
   
   UFUNCTION()
   MyScript* ScriptFunction();
    
}
```
### Contructing objects and Deleting Objects
```cpp
#include "reflection/reflection.hpp"

refl::reflector reflect = refl::reflector();
refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
MyScript* myScriptPtr = (MyScript*)myScript.data();
// will auto destroy unless there are multiple instances
reflect.DestroyUClass(myScript)
```

### Get Members
```cpp
#include "reflection/reflection.hpp"

refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
int i = myScript.GetMember<int>("i");
char c = myScript.GetMember<char>("c");
void* ptr = myScript.GetMember<void*>("ptr");
char* str = myScript.GetMember<const char*>("str");
std::vector<int> vector = myScript.GetMember<std::vector<int>>("vec");
int& ref = myScript.GetMember<int&>("ref");
// Can also get regular members as ptrs and refs
int* iPtr = &myScript.GetMember<int>("i");
int& iRef = myScript.GetMember<int>("i");
// Can also get member as uClass
refl::uClass uClazz = myScript.GetMember<uClass>("i");
int i = uClazz.data_as<int>();
```

### Set Members
```cpp
#include "reflection/reflection.hpp"

refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
// can set calling set member
myScript.SetMember<int>("i", 420);
myScript.SetMember<char>("c", 'f');
myScript.SetMember<void*>("ptr", nullptr);
myScript.SetMember<std::vector<int>>("vec", {1,2,3,4});
int i = 0;
myScript.SetMember<int&>("ref", i);
// or can set using references and get member
myScript.GetMember<char>("c") = 'j';
myScript.GetMember<int>("i") = 66;

```

### Call functions
```cpp
#include "reflection/reflection.hpp"

refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
// <return type>
myScript.CallFunction<void>("VoidFunc", 68, "My Str");
int i = myScript.CallFunction<int>("IntFunc", 70);
// get pointer as uClass
uClass c = myScript.CallFunction<uClass>("ScriptFunction");
```

### Inheritance
Classes will automatically load exposed inherited functions and members to their list of callables
```cpp
refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);

myScript.CallFunction<void>("BaseFunction");
myScript.GetMember<int>("baseProperty");

```

---------
Platforms
---------
Tested and Compiled with
- GCC 4.2.1
- APPLE CLANG 12.0.0
- UBUNTU CLANG 10.0.0
- MSBUILD (VS2019,VS2018)
