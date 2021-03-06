#pragma once
#include "error.hpp"
#include "storage.hpp"
#include "generate.hpp"

#if defined(_MSC_VER)
    //  Microsoft 
    #define __REFLECTION__EXPORT__ extern "C" __declspec(dllexport)
    #define __REFLECTION__IMPORT__ extern "C" __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define __REFLECTION__EXPORT__ extern "C" __attribute__((visibility("default")))
    #define __REFLECTION__IMPORT__ extern "C"
#else
    #define __REFLECTION__EXPORT__
    #define __REFLECTION__IMPORT__
#endif

namespace refl {

	class reflector;

	class uClass {
		public:
		inline uClass() {};
		uClass(uClass&& other);
		uClass(void* p, const std::string& n, ::refl::reflector* r, bool destroy = false);
		~uClass();

			template <typename T, typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr>
			inline typename std::remove_reference<T>::type& __GetMember(const std::string& name) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				return (T&)(*(T*)((uint8_t*)ptr + o));
			}

			template <typename T, typename std::enable_if<std::is_reference<T>::value>::type* = nullptr>
			inline typename std::remove_reference<T>::type& __GetMember(const std::string& name) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				return (T&)(*(typename std::remove_reference<T>::type*)((uint8_t*)ptr + o));
			}

			template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
			inline T _GetMember(const std::string& name) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				return *(T*)((uint8_t*)ptr + o);
			}
			template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
			inline typename std::remove_reference<T>::type& _GetMember(const std::string& name) {
				return __GetMember<T>(name);
			}

			template <typename T, typename std::enable_if<!std::is_same<T, uClass>::value>::type* = nullptr>
			inline typename std::conditional<std::is_pointer<T>::value, T, typename std::remove_reference<T>::type&>::type GetMember(const std::string& name) {
				return _GetMember<T>(name);
			}

			template <typename T, typename std::enable_if<std::is_same<T, uClass>::value>::type* = nullptr>
			inline uClass GetMember(const std::string& name) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				return uClass((void*)((uint8_t*)ptr + o), map.at(clazz).property_map.at(name).type_name, this->ref);
			}

			template<typename T>
			inline void SetMember(const std::string& name, T value) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				*(T*)((uint8_t*)ptr + o) = value;
			}
			private:

			template <typename T, typename std::enable_if<std::is_reference<T>::value>::type* = nullptr>
			T ___CallFunction(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				void* _f = map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = (T)(*(typename std::remove_reference<T>::type*)_f);
				return f;
			}
			template <typename T, typename std::enable_if<!std::is_reference<T>::value>::type* = nullptr>
			T ___CallFunction(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				T* _f = (T*)map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = *_f;
				delete _f;
				return f;
			}
			template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
			T __CallFunction(const std::string& name, const std::vector<void*>& vec) {
				return ___CallFunction<T>(name, vec);
			}

			template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
			T __CallFunction(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				void* _f = map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = (T)_f;
				return f;
			}

			template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
			T __CallFunction_uClass(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				const ::refl::store::ufunction_struct& func = map.at(clazz).function_map.at(name);
				void* _f = func.function(ptr, vec);
				std::string ret_name = func.ret_name;
				ret_name.erase(ret_name.size()-1);
				return uClass(_f, ret_name, ref);
			}
			

			template <typename T, typename std::enable_if<std::is_same<T, uClass>::value>::type* = nullptr>
			T _CallFunction(const std::string& name, const std::vector<void*>& vec) {
				return __CallFunction_uClass<T>(name, vec);
			}

			template <typename T, typename std::enable_if<!std::is_same<T, uClass>::value>::type* = nullptr>
			T _CallFunction(const std::string& name, const std::vector<void*>& vec) {
				return __CallFunction<T>(name, vec);
			}

			public:
			template<typename T, typename ... Args>
			inline typename std::conditional< (std::is_void<T>::value), void*, T>::type CallFunction(const std::string& name, Args&& ... args) {
				std::vector<void*> vec = {(void*)&args...};
				return _CallFunction<typename std::conditional< (std::is_void<T>::value), void*, T>::type>(name, vec);
			}

			template<typename T, typename ... Args>
			inline static typename std::conditional< (std::is_void<T>::value), void*, T>::type CallStaticFunction(
				const std::string& clazz, const std::string& name, ::refl::reflector* r, const std::vector<void*>& vec) {
				
				uClass cls = uClass(nullptr, clazz, r);
				return cls._CallFunction<typename std::conditional< (std::is_void<T>::value), void*, T>::type>(name, vec);
			}

			template<typename ... Args>
			inline void CallVoidFunction(const std::string& name, Args&& ... args) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				std::vector<void*> vec = {(void*)&args...};
				void* f = map.at(clazz).function_map.at(name).function(ptr, vec);
				::refl::store::uproperty_type ret_type = map.at(clazz).function_map.at(name).ret_val;
				if (ret_type != ::refl::store::uproperty_type::_ptr && ret_type != ::refl::store::uproperty_type::uclass_ptr && ret_type != ::refl::store::uproperty_type::_void) {
					if (f != nullptr)
						free(f);
				}
				return;
			}
			inline void* data() const { return ptr; }
			template<typename T>
			inline T data_as() const { return *(T*)ptr; }

			inline uClass& operator=(uClass&& other) {
				this->clazz = other.clazz;
				this->destroy = other.destroy;
				this->ptr = other.ptr;
				this->ref = other.ref;
				this->store = other.store;
				other.destroy = false;
				return *this;
			}
		private:
			std::string clazz = "NULL";
			void* ptr = nullptr;
			::refl::reflector* ref;
			::refl::store::storage* store;
			bool destroy = false;
			friend class ::refl::reflector;

	};

	class reflector {
		public:
			inline reflector() : err(), gen(&err), st(&err) {}
			inline ~reflector() { }
			inline void LoadClasses(const char* in){ gen.load_classes(in);}
			inline void GenerateClasses() { gen.generate_files(); }
			inline void Clear(){ gen.clear();}
			inline void SetErrorCallback(void(*f)(const char*)) { err.setErrorCallback(f); }			
			inline bool HasError() const { return err.HasError(); }
			inline const char* GetError()  { return err.GetError();}
			inline store::storage* GetStorage() { return &st; }
			inline void SetOutputDir(const char* outputDir) { gen.set_output(outputDir); }
			inline void SetRelativeInclude(const char* includeDir) {gen.set_relative_include(includeDir);}
			//void LoadGeneratedFiles() { ::refl::impl::__loadGeneratedFiles(&st); }
			//void UnloadGeneratedFiles() { ::refl::impl::__unloadGeneratedFiles(&st); }
		private:
			err::err_hndl err;
			gen::generator gen;
			store::storage st;
		private:
			template <typename T>
			T __callfunc(void* ptr, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = st.get_map();
				void* _f = map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = (T)_f;
				return f;

			}

			void __callfunc(void* ptr, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = st.get_map();
				void* f = map.at(clazz).function_map.at(name).function(ptr, vec);
				::refl::store::uproperty_type ret_type = map.at(clazz).function_map.at(name).ret_val;
				if (ret_type != ::refl::store::uproperty_type::_ptr && ret_type != ::refl::store::uproperty_type::uclass_ptr && ret_type != ::refl::store::uproperty_type::_void) {
					if (f != nullptr)
						free(f);
				}
				return;
			}

		public:
			template<typename ... Args>
			inline uClass CreateUClass(const std::string& clazz, Args&& ... args) {
				std::vector<void*> vec = {(void*)&args...};
				void* t = __callfunc<void*>(nullptr, clazz, clazz,vec);
				return uClass(t, clazz, this, true);
			}

			template <typename T, typename ... Args>
			inline typename std::conditional< (std::is_void<T>::value), void*, T>::type CallStaticFunction(const std::string& clazz, const std::string& name, Args&& ... args) {
				std::vector<void*> vec = {(void*)&args...};
				return uClass::CallStaticFunction<typename std::conditional< (std::is_void<T>::value), void*, T>::type>(clazz, name, this, vec);
			}

			inline void DestroyUClass(uClass& c) {
				if (c.ptr == nullptr) { err.setErrorStr("REFL ERR: Attempted to destroy uninstantiated uClass"); return;}
				__callfunc(c.ptr, c.clazz,"~"+c.clazz, {});
				c.ptr = nullptr;
				c.clazz = "NULL";
			}
			
	};

	inline uClass::~uClass() {
		if (destroy && ptr != nullptr) {
			ref->DestroyUClass(*this);
		}
	}
	inline uClass::uClass(void* p, const std::string& n, ::refl::reflector* r, bool d) : clazz(n), ptr(p), ref(r), store(r->GetStorage()), destroy(d) {

	};

	inline uClass::uClass(uClass&& other) : clazz(other.clazz), ptr(other.ptr), ref(other.ref), store(other.ref->GetStorage()), destroy(other.destroy) {
		other.destroy = false;
	}



}
