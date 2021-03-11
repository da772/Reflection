#pragma once
#include "error.hpp"
#include "storage.hpp"
#include "generation.hpp"
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
    //  do nothing and hope for the best?
    #define __REFLECTION__EXPORT__
    #define __REFLECTION__IMPORT__
#endif


namespace refl {
/*
	namespace impl {
		extern void __loadGeneratedFiles(refl::store::storage* storage);
		extern void __unloadGeneratedFiles(refl::store::storage* storage);
	}
*/	
	class reflector;

	class uClass {
		public:
		uClass(void* p, const std::string& n, ::refl::reflector* r);
		~uClass();
		
			template<typename T>
			inline T GetMember(const std::string& name) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				return *(T*)((uint8_t*)ptr + o);
			}

			template<typename T>
			inline void SetMember(const std::string& name, T value) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				uintptr_t o = map.at(clazz).property_map.at(name).offset;
				*(T*)((uint8_t*)ptr + o) = value;
			}
			private:
			template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
			T _CallFunction(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				T* _f = (T*)map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = *_f;
				delete _f;
				return f;
			}

			template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
			T _CallFunction(const std::string& name, const std::vector<void*>& vec) {
				const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
				void* _f = map.at(clazz).function_map.at(name).function(ptr, vec);
				T f = (T)_f;
				return f;

			}
			public:

			template<typename T, typename ... Args>
			inline T CallFunction(const std::string& name, Args&& ... args) {
				std::vector<void*> vec = {(void*)&args...};
				return _CallFunction<T>(name, vec);
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
			inline void* data() const {return ptr;}
		private:
			std::string clazz = "NULL";
			void* ptr = nullptr;
			::refl::reflector* ref;
			::refl::store::storage* store;
			friend class ::refl::reflector;

	};

	class reflector {
		public:
			inline reflector() : gen(&err), st(&err) {}
			inline ~reflector() {}
			inline void Generate(const char* in){ return gen.generate(in);}
			inline void SetErrorCallback(void(*f)(const char*)) { err.setErrorCallback(f); }			
			inline bool HasError() const { return err.HasError(); }
			inline const char* GetError()  { return err.GetError();}
			inline store::storage* GetStorage() { return &st; }
			inline void SetOutputDir(const char* outputDir) { gen.set_output(outputDir); }
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
				return uClass(t, clazz, this);
			}

			inline void DestroyUClass(uClass& c) {
				if (c.ptr == nullptr) { err.setErrorStr("REFL ERR: Attempted to destroy uninstantiated uClass"); return;}
				__callfunc(c.ptr, c.clazz,"~"+c.clazz, {});
				c.ptr = nullptr;
				c.clazz = "NULL";
			}
			
	};

	inline uClass::~uClass() {
		if (ptr != nullptr) {
			ref->DestroyUClass(*this);
		}
	}
	inline uClass::uClass(void* p, const std::string& n, ::refl::reflector* r) : clazz(n), ptr(p), ref(r), store(r->GetStorage()) {

	};

}