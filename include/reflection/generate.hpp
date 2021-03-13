#pragma once
#include <fstream>
#include <streambuf>
#include <sstream>  

#include "reflection.hpp"
#include "error.hpp"
#include "storage.hpp"

namespace refl {
	namespace gen {
		namespace impl {
			struct ufunction {
				std::string name;
				uint32_t ret_val;
				std::string ret_name;
				std::vector<std::pair<uint32_t, std::string>> args_val;
			};
			static std::string get_class(const std::string& s, size_t*, ::refl::err::err_hndl* err);
			static std::pair<std::string, std::string> get_next_member(const std::string& s, size_t* pos);
			static ufunction get_next_method(const std::string& s, const std::string& clss, size_t* pos,::refl::err::err_hndl* err);
			static void reload_generation_map(const std::string& outputDir, const std::vector<std::string>& map);
			static std::string set_method_args(const impl::ufunction& f);
			static ufunction get_constructor(const std::string& s, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err);
		}
		class generator {
			public:
				inline generator(::refl::err::err_hndl* error) : err(error) {}
				inline void clear() { clss.clear(); }
				inline void generate(const char* in) {
					std::ifstream t(in);
					std::stringstream buffer;
					buffer << t.rdbuf();
					t.close();
					std::pair<std::string, std::string> p = generateM(buffer.str().c_str());
					std::ofstream out(outputDir+p.first+std::string(".generated.h"));
    				out << p.second;
    				out.close();
					clss.push_back(p.first);
					impl::reload_generation_map(outputDir, clss);	
				}

				inline std::pair<std::string, std::string> generateM(const char* _in) {
					std::string in(_in);
					size_t pos = 0;
					std::string s = "#pragma once\n#include \"reflection/reflection.hpp\"\n#include \"../";
					std::string cls = impl::get_class(in, &pos, err);
					s+= cls+".h\"\nclass " + cls + "_Generated : public refl::class_generation {\n ";
					s+= "\tpublic:\n\tinline static void Load(::refl::store::storage* storage) {\n";
					s+= "\t\tstorage->store(\""+cls+"\",{\""+cls+"\",{\n";
					pos = in.find("UPROPERTY()");
					while (pos != std::string::npos) {
						std::pair<std::string, std::string> m = impl::get_next_member(in, &pos);
						s+= "\t\t{\""+m.second+"\",{\""+m.second+"\",\""+m.first+"\",static_cast<refl::store::uproperty_type>("
						+std::to_string(static_cast<uint32_t>(store::GetTypeInt(m.first)))+"),refl::store::offsetOf(&" +cls+"::"+ m.second+") }}";
						if (pos != std::string::npos) s+= ",\n";
					}
					s+="}, {\n";
					pos = in.find("UCONSTRUCTOR()");
					while (pos != std::string::npos) {
						impl::ufunction f = impl::get_constructor(in, cls, &pos, err);
						s+= "\t\t{\""+cls+"\",{\""+cls+"\",\""+f.ret_name+"\",static_cast<refl::store::uproperty_type>("
						+std::to_string(static_cast<uint32_t>(f.ret_val))+"),{";
						for (const std::pair<uint32_t, std::string>& p : f.args_val) {
							s+= "{static_cast<::refl::store::uproperty_type>("+std::to_string(p.first)+"),\""+p.second+"\"},";
						}
						if (f.args_val.size() > 0)
							s[s.size()-1] = ' ';
						s+= "},[](void* ptr, std::vector<void*> args) {";
						s+= "return (void*)new "+f.ret_name+"(";
						s+= impl::set_method_args(f);
						s +=");} }},\n";
						s+= "\t\t{\"~"+cls+"\",{\"~"+cls+"\",\"void\",static_cast<refl::store::uproperty_type>("+std::to_string(static_cast<uint32_t>(store::GetTypeInt("void")))+"),{},";
						s+= "[](void* ptr, std::vector<void*> args) {"+cls+"* p = ("+cls+"*)ptr; delete p; return nullptr;} }}";
					}
					pos = in.find("UFUNCTION()");
					if (pos != std::string::npos) s+= ",\n";
					while (pos != std::string::npos) {
						impl::ufunction f = impl::get_next_method(in, cls, &pos, err);
						s+= "\t\t{\""+f.name+"\",{\""+f.name+"\",\""+f.ret_name+"\",static_cast<refl::store::uproperty_type>("
						+std::to_string(static_cast<uint32_t>(f.ret_val))+"),{";
						for (const std::pair<uint32_t, std::string>& p : f.args_val) {
							s+= "{static_cast<::refl::store::uproperty_type>("+std::to_string(p.first)+"),\""+p.second+"\"},";
						}
						if (f.args_val.size() > 0)
							s[s.size()-1] = ' ';
						s+= "},[](void* ptr, std::vector<void*> args) {";
						if (static_cast<::refl::store::uproperty_type>(static_cast<uint32_t>(f.ret_val)) == ::refl::store::uproperty_type::_void) {
							s+= " (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							s +="); return nullptr; } }}";
						} else if (static_cast<::refl::store::uproperty_type>(static_cast<uint32_t>(f.ret_val)) == ::refl::store::uproperty_type::uclass_ptr) {
							s+= f.ret_name +" v = (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							s+= impl::set_method_args(f);
							s+="); return (void*)v; } }}";
						} else if (static_cast<::refl::store::uproperty_type>(static_cast<uint32_t>(f.ret_val)) == ::refl::store::uproperty_type::constructor) {
							s+= "return (void*)new "+f.ret_name+"(";
							s+= impl::set_method_args(f);
							s +=");} }},\n";
							s+= "\t\t{\"~constructor\",{\"~constructor\",\"void\",static_cast<refl::store::uproperty_type>("+std::to_string(static_cast<uint32_t>(store::GetTypeInt("void")))+"),{},";
							s+= "[](void* ptr, std::vector<void*> args) {"+cls+"* p = ("+cls+"*)ptr; delete p; return nullptr;} }}";
						}
						else {
							s+= f.ret_name +" v = (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							s+= impl::set_method_args(f);
							s+="); "+f.ret_name+"* _ptr = new "+f.ret_name+"(v); return (void*)_ptr; } }}";
						}
						
						if (pos != std::string::npos) s+= ",\n";
					}
					s+= "}});\n";
					s += "\t}\n\tinline static void Unload(::refl::store::storage* storage) {\n";
					s += "\t\tstorage->discard(\""+cls+"\");\n\t}\n";
					s += "};\n";

					return {cls, s};
				}

				inline void set_output(const char* c) {
					outputDir = std::string(c);
				}

			private:
				::refl::err::err_hndl* err;
				std::string outputDir;
				std::vector<std::string> clss;
		};

		namespace impl {

			static std::string set_method_args(const impl::ufunction& f) {
				std::string s = "";
				for (uint32_t i = 0; i < f.args_val.size(); i++) {
					std::pair<uint32_t, std::string> p = f.args_val[i];
					::refl::store::uproperty_type _type = static_cast<::refl::store::uproperty_type>(f.ret_val);
					if (_type == ::refl::store::uproperty_type::_ptr || _type == ::refl::store::uproperty_type::uclass_ptr) {
						s += "*("+p.second+"*)args["+std::to_string(i)+"]";
					} else {
						s += "*("+p.second+"*)args["+std::to_string(i)+"]";
					}
					if (i < f.args_val.size()-1)
						s += ", ";
				}
				return s;
			}

			static void reload_generation_map(const std::string& outputDir, const std::vector<std::string>& map) {
				std::string s = "#pragma once\n#include \"reflection/reflection.hpp\"\n";
				s += "\n";
				s += "\t__REFLECTION__EXPORT__ void __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage);\n";
				s += "\t__REFLECTION__EXPORT__ void __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage);\n";
				std::ofstream out(outputDir+std::string("Reflection.map.generated.h"));
				out << s;
				out.close();

				s = "#include \"Reflection.map.generated.h\"\n";
				for (const std::string& p : map) {
					s += std::string("#include \"")+p+std::string(".generated.h\"\n");
				}
				s += "\n\n";
				s += "\tvoid __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage)";
				s+= " {\n";
				for (const std::string& p : map) {
					s += std::string("\t\t")+p+std::string("_Generated::Load(storage);\n");
				}
				s += "\t}\n";
				s += "\tvoid __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage)";
				s+= " {\n";
				for (const std::string& p : map) {
					s += std::string("\t\t")+p+std::string("_Generated::Unload(storage);\n");
				}
				s += "\t}\n";
				s += "\n";
				out = std::ofstream(outputDir+std::string("Reflection.map.generated.cpp"));
				out << s;
				out.close();
			}
			static std::string get_class(const std::string& in, size_t* pos, ::refl::err::err_hndl* err) {
					size_t uclassPos = in.find("UCLASS()");

					if (uclassPos == std::string::npos) {
						err->setErrorStr("Error 0: UCLASS not found");
						return "";
					}

					size_t classPos = in.find("class ", uclassPos+7);

					if (classPos == std::string::npos) {
						err->setErrorStr("Error 1: class not found");
						return "";
					}

					size_t clssStart = classPos+6;
					char cC = in[clssStart]; 
					while (cC == ' ' || cC == '\n' || cC == '\t') {
						cC = in[++clssStart];
					}

					cC = in[clssStart]; 
					size_t nextSpace = clssStart;
					while (cC != ' ' && cC != '\n' && cC != ':' && cC != '\t' && cC != '{') {
						cC = in[++nextSpace];
					}
					*pos = nextSpace;
					return in.substr(clssStart, nextSpace-clssStart);
			}

			static std::pair<std::string, std::string> get_next_member(const std::string& in, size_t* pos) {
				size_t uProp = (*pos)+11;
				char cC = in[uProp];
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=') {
					cC = in[++uProp];
				}
				size_t nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{') {
					cC = in[++nextSpace];
				}
				size_t newL = in.find(";", uProp);
				std::string typeName = in.substr(uProp, nextSpace-uProp);
				if (typeName == "unsigned") {
					size_t _uProp = nextSpace;
					cC = in[uProp];
					while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=') {
						cC = in[++_uProp];
					}
					nextSpace = _uProp;
					while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{') {
						cC = in[++nextSpace];
					}
					typeName = in.substr(uProp, nextSpace-uProp);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=' || cC == '*') {
					if (cC == '*') {
						typeName += "*";
					}
					cC = in[++uProp];
				}
				nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != ':' && cC != '\t' && cC != '{' && cC != ';') {
					cC = in[++nextSpace];
				}
				std::string memName = in.substr(uProp, nextSpace-uProp);
				*pos = in.find("UPROPERTY()", newL);
				return {typeName, memName };
			}

			static ufunction get_constructor(const std::string& in, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err) {
				size_t uProp = *pos+14;
				if (uProp == std::string::npos) {
					err->setErrorStr("Refl::Error: Failed to find UCONSTRUCTOR");
					return {0};
				}
				char cC = in[uProp];
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=') {
					cC = in[++uProp];
				}
				size_t nextSpace = uProp;
				std::string typeName = "";
				while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{') {
					typeName = "";
					if (cC == '(') {
						std::string name = in.substr(uProp, nextSpace-uProp);
						if (name == clss) {
							typeName = name;
							break;
						}
					}
					cC = in[++nextSpace];
					
				}
				size_t newL = in.find(";", uProp);
				if (typeName.size() <= 0)
					typeName = in.substr(uProp, nextSpace-uProp);
				if (typeName == "inline" || typeName == "virtual") {
					*pos = nextSpace-14;
					return get_constructor(in, clss, pos, err);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=' || cC == '*' ) {
					if (cC == '*') {
						typeName += "*";
					}
					cC = in[++uProp];
				}
				nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{' && cC != ';' && cC != '(') {
					cC = in[++nextSpace];
				}
				std::string memName = in.substr(uProp, nextSpace-uProp);
				std::vector<std::pair<uint32_t, std::string>> v;
				uProp = nextSpace+1;
				uint32_t spacePos = 0;
				while (cC != ')') {
					if (cC == '&') {
						err->setErrorStr(std::string(memName + "Error: passing by reference not allowed use pointer!").c_str() );
						*pos = std::string::npos;
						return {};
					}
					if (cC != ' ' && cC != '\t' && cC != '\n' && spacePos != 0) {
						spacePos = 0;
						uProp = nextSpace;
					}
					if (cC == ' ' || cC == '\t' || cC == '\n' && spacePos == 0) {
						std::string varName = in.substr(uProp, nextSpace-uProp);
						v.push_back( {::refl::store::GetTypeInt(varName), varName });
						size_t commaPos = in.find(",", nextSpace);
						size_t parPos = in.find(")", nextSpace);
						uProp = (commaPos < parPos && commaPos != std::string::npos ? commaPos+1 : parPos-1);
						nextSpace = uProp;
						spacePos = 1;
					}
					

					cC = in[++nextSpace];
				}

				*pos = in.find("UCONSTRUCTOR()", newL);
				return {clss, ::refl::store::GetTypeInt(typeName), typeName, v};
			}

			static ufunction get_next_method(const std::string& in, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err) {
			    size_t uProp = (*pos)+11;
				char cC = in[uProp];
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=') {
					cC = in[++uProp];
				}
				size_t nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{') {
					cC = in[++nextSpace];
				}
				size_t newL = in.find(";", uProp);
				std::string typeName = in.substr(uProp, nextSpace-uProp);
				if (typeName == "inline" || typeName == "virtual") {
					*pos = nextSpace-11;
					return get_next_method(in, clss, pos, err);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=' || cC == '*' ) {
					if (cC == '*') {
						typeName += "*";
					}
					cC = in[++uProp];
				}
				nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != '\t' && cC != '{' && cC != ';' && cC != '(') {
					cC = in[++nextSpace];
				}
				std::string memName = in.substr(uProp, nextSpace-uProp);
				std::vector<std::pair<uint32_t, std::string>> v;
				uProp = nextSpace+1;
				uint32_t spacePos = 0;
				while (cC != ')') {
					if (cC == '&') {
						err->setErrorStr(std::string(memName + "Error: passing by reference not allowed use pointer!").c_str() );
						*pos = in.find("UFUNCTION()", newL);
						return {};
					}
					if (cC != ' ' && cC != '\t' && cC != '\n' && spacePos != 0) {
						spacePos = 0;
						uProp = nextSpace;
					}
					if (cC == ' ' || cC == '\t' || cC == '\n' && spacePos == 0) {
						std::string varName = in.substr(uProp, nextSpace-uProp);
						v.push_back( {::refl::store::GetTypeInt(varName), varName });
						size_t commaPos = in.find(",", nextSpace);
						size_t parPos = in.find(")", nextSpace);
						uProp = (commaPos < parPos && commaPos != std::string::npos ? commaPos+1 : parPos-1);
						nextSpace = uProp;
						spacePos = 1;
					}
					

					cC = in[++nextSpace];
				}

				*pos = in.find("UFUNCTION()", newL);

				return {memName, ::refl::store::GetTypeInt(typeName), typeName, v};
			}
		}

	}
}
