#pragma once
#include "error.hpp"
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>

namespace refl {

    namespace store {
	
        template<typename T, typename U> constexpr size_t offsetOf(U T::*member)
        {
            return (char*)&((T*)nullptr->*member) - (char*)nullptr;
        }
            
        enum class uproperty_type : uint32_t {
            NONE = 0, clss, _bool, _int, _uint, _char, _uchar, _ptr, _intptr, _int8_t, _int16_t, _int32_t, _int64_t, _uint8_t,
            _uint16_t, _uint32_t, _uint64_t
        };

        namespace impl {
            static std::unordered_map<std::string, uproperty_type> type_map = { {"int", uproperty_type::_int}, 
            {"unsigned int", uproperty_type::_uint}, {"char", uproperty_type::_char},{"unsigned char", uproperty_type::_uchar},
            {"void*", uproperty_type::_ptr}, {"int*", uproperty_type::_intptr}, {"int8_t", uproperty_type::_int8_t}, 
            {"int16_t", uproperty_type::_int16_t},{"int32_t", uproperty_type::_int32_t}, {"int64_t", uproperty_type::_int64_t},
            {"uint8_t", uproperty_type::_uint8_t}, {"uint16_t", uproperty_type::_uint16_t},{"uint32_t", uproperty_type::_uint32_t},
            {"uint64_t", uproperty_type::_uint64_t}, {"bool", uproperty_type::_bool}};   
        }


        uint32_t GetTypeInt(const std::string& s) {
            if (impl::type_map.find(s) != impl::type_map.end()) {
                return static_cast<uint32_t>(impl::type_map[s]);
            }
            return static_cast<uint32_t>(uproperty_type::clss);
        }

        struct uproperty_struct {
            std::string name;
            uproperty_type type;
            uintptr_t offset;
        };

        struct ufunction_struct {
            std::vector<uproperty_type> args_val;
            uproperty_type ret_val;
            uintptr_t offset;
        };

        struct uobject_struct {
            std::string name;
            std::vector<uproperty_struct> property_map;
            std::unordered_map<std::string, ufunction_struct> function_map;
        };

        class storage {
            public:
            inline storage(::refl::err::err_hndl* error) : err(error) {

            }
            inline ~storage() {

            }
            inline void store(const std::string& s, uobject_struct data) { map[s] = data;}
            inline void discard(const std::string& s) { map.erase(s);}
            private:
            std::unordered_map<std::string, uobject_struct> map;
            ::refl::err::err_hndl* err;
        };

    }



}