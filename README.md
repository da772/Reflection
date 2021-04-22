Reflection
==========
> C++11 header only reflection library

A runtime reflection library for inspecting and modyfiying objects at runtime. Primarly created for hot-reloading C++ code and using C++ as a scripting language while still maintaining native C++ speeds.

---------
Usage
---------

Example Script.hpp
```cpp

UCLASS()
class MyScript {
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
refl::reflector reflect = refl::reflector();
refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
MyScript* myScriptPtr = (MyScript*)myScript.data();
// will auto destroy unless there are multiple instances
reflect.DestroyUClass(myScript)
```

### Get Members
```cpp
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
refl::uClass myScript = reflect.CreateUClass("MyScript", 1, "Hello World", nullptr);
// <return type>
myScript.CallFunction<void>("VoidFunc", 68, "My Str");
int i = myScript.CallFunction<int>("IntFunc", 70);
// get pointer as uClass
uClass c = myScript.CallFunction<uClass>("ScriptFunction");
```

---------
Platforms
---------
Tested and Compiled with
- GCC 4.2.1
- APPLE CLANG 12.0.0
- MSBUILD (VS2019,VS2018)
