#pragma once
#include <fstream>
#include <streambuf>
#include <sstream>  

#include "reflection.hpp"
#include "error.hpp"
#include "storage.hpp"

namespace refl {
	class class_generation {};
	namespace gen {
		namespace impl {
			struct ufunction {
				std::string name = "NULL";
				uint32_t ret_val;
				std::string ret_name;
				std::vector<std::pair<uint32_t, std::string>> args_val;
				bool _static;
				bool _virtual;

				bool operator==(const ufunction& other) const {
					return other.name == name;
				}

			};

			struct umember {
				std::string type_name;
				std::string member_name;

				bool operator==(const umember& other) const {
					return other.member_name == member_name;
				}
			};

			struct uclass {
				std::string name;
				std::vector<ufunction> func;
				std::vector<umember> members;
				std::vector<std::string> parents;
				std::string fileName;
				ufunction constructor;
			};

	
			static std::pair < std::string, std::vector < std::string>> get_class(const std::string& s, size_t*, ::refl::err::err_hndl* err);
			static std::pair<std::string, std::string> get_next_member(const std::string& s, size_t* pos);
			static ufunction get_next_method(const std::string& s, const std::string& clss, size_t* pos,::refl::err::err_hndl* err, bool _static, bool _virtual);
			static void reload_generation_map(const std::string& outputDir, const std::vector<std::string>& map);
			static std::string set_method_args(const impl::ufunction& f);
			static ufunction get_constructor(const std::string& s, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err, bool _static);
		}
		class generator {
			public:
				inline generator(::refl::err::err_hndl* error) : err(error) {}
				std::unordered_map<std::string, impl::uclass> data;
				inline void clear() { clss.clear(); data.clear(); }
				inline void load_classes(const char* in) {
					std::string fileN(in);
					size_t p = fileN.find_last_of("/");
					fileN = fileN.substr(p+1, fileN.size());
					std::ifstream t(in);
					std::stringstream buffer;
					buffer << t.rdbuf();
					t.close();
					get_uclass_data(buffer.str().c_str(), fileN.c_str(), data);
				}

				inline void generate_files() {
					std::vector<std::pair<std::string, std::string>> _p = generate_classes(data);
					for (std::pair<std::string, std::string>& p : _p) {
						std::ofstream out(outputDir + p.first + std::string(".generated.h"));
						out << p.second;
						out.close();
						clss.push_back(p.first);
					}
					impl::reload_generation_map(outputDir, clss);
				}

				inline void get_uclass_data(const char* _in, const char* fileN, std::unordered_map<std::string, impl::uclass>& data) {
					std::string m_in(_in);
					size_t pos = 0;

					bool next = true;
					size_t lstClssPos = 0;
					
					while (next) {
						std::pair<std::string, std::vector<std::string>> clsPair = impl::get_class(m_in, &lstClssPos, err);
						std::string& cls = clsPair.first;
						pos = lstClssPos;
						if (cls.size() <= 0)
							break;
						size_t _pos = pos;
						size_t nextCls = impl::get_class(m_in, &_pos, err).first.size();
						std::string in;
						if (nextCls <= 0) {
							in = m_in.substr(pos, m_in.size() - pos);
							next = false;
						}
						else {
							in = m_in.substr(pos, _pos - pos);
						}
						std::vector < impl::umember > mems;
						pos = in.find("UPROPERTY()");
						while (pos != std::string::npos) {
							std::pair<std::string, std::string> m = impl::get_next_member(in, &pos);
							mems.push_back({ m.first, m.second });
						}
						impl::ufunction cons;
						pos = in.find("UCONSTRUCTOR()");
						bool constructorFound = false;
						while (pos != std::string::npos) {
							cons = impl::get_constructor(in, cls, &pos, err, false);
							constructorFound = true;
						}
						std::vector<impl::ufunction> func;
						pos = in.find("UFUNCTION()");
						while (pos != std::string::npos) {
							impl::ufunction f = impl::get_next_method(in, cls, &pos, err, false, false);
							func.push_back(f);
						}
						data[cls] = { cls, func, mems, clsPair.second, fileN, cons};
					}
				}

				inline std::vector<std::pair<std::string, std::string>> generate_classes(const std::unordered_map<std::string, impl::uclass>& data) {
					std::vector<std::pair<std::string, std::string>> ret;

					for (const std::pair<std::string, impl::uclass>& _class : data) {
						std::string s = "#pragma once\n#include \"reflection/reflection.hpp\"\n#include \"" + relativeInclude;
						std::string cls = _class.first;
						
						s += _class.second.fileName + "\"\nclass " + cls + "_Generated : public refl::class_generation {\n ";
						s += "\tpublic:\n\tinline static void Load(::refl::store::storage* storage) {\n";
						s += "\t\tstorage->store(\"" + cls + "\",{\"" + cls + "\",{\n";

						std::vector<impl::umember> members = _class.second.members;

						std::unordered_map<std::string, std::vector<impl::umember>> parent_members;
						size_t totalCount = members.size();
						{
							std::vector<std::string> parents = _class.second.parents;

							for (int i = 0; i < parents.size(); i++) {
								std::string parent = parents[i];
								const auto& it = data.find(parent);
								if (it == data.end()) continue;
								parents.insert(parents.end(), it->second.parents.begin(), it->second.parents.end());
								for (const auto& _f : it->second.members) {
									if (std::find(_class.second.members.begin(), _class.second.members.end(), _f) == _class.second.members.end()) {
										parent_members[parent].push_back(_f);
										totalCount++;
									}
								}
							}
						}
						
						for (int i = 0; i < members.size(); i++) {
							const impl::umember& m = members[i];
							s += "\t\t{\"" + m.member_name + "\",{\"" + m.member_name+ "\",\"" + m.type_name + "\",static_cast<refl::store::uproperty_type>("
								+ std::to_string(static_cast<uint32_t>(store::GetTypeInt(m.type_name))) + "),refl::store::offsetOf(&" + cls + "::" + m.member_name+ ") }}";
							if (i != totalCount - 1) s += ",\n";
						}

						{
							int i = 0;

							for (const auto& p : parent_members) {
								for (const auto& m : p.second) {
									s += "\t\t{\"" + m.member_name + "\",{\"" + m.member_name + "\",\"" + m.type_name + "\",static_cast<refl::store::uproperty_type>("
										+ std::to_string(static_cast<uint32_t>(store::GetTypeInt(m.type_name))) + "),offsetof(" + cls+ "," + m.member_name + ") }}";
									if (i != totalCount - 1) s += ",\n";
									i++;
								}
							}
						}

						s += "}, {\n";

						bool constructorFound = false;
						if (_class.second.constructor.name != "NULL") {
							const impl::ufunction& f = _class.second.constructor;
							s += "\t\t{\"" + cls + "\",{\"" + cls + "\",\"" + f.ret_name + "\",static_cast<refl::store::uproperty_type>("
								+ std::to_string(static_cast<uint32_t>(f.ret_val)) + "),{";
							for (const std::pair<uint32_t, std::string>& p : f.args_val) {
								s += "{static_cast<::refl::store::uproperty_type>(" + std::to_string(p.first) + "),\"" + p.second + "\"},";
							}
							if (f.args_val.size() > 0)
								s[s.size() - 1] = ' ';
							s += "},[](void* ptr, std::vector<void*> args) {";
							s += "return (void*)new " + f.ret_name + "(";
							s += impl::set_method_args(f);
							s += ");} }},\n";
							s += "\t\t{\"~" + cls + "\",{\"~" + cls + "\",\"void\",static_cast<refl::store::uproperty_type>(" + std::to_string(static_cast<uint32_t>(store::GetTypeInt("void"))) + "),{},";
							s += "[](void* ptr, std::vector<void*> args) {" + cls + "* p = (" + cls + "*)ptr; delete p; return nullptr;} }}";
							constructorFound = true;
						}
						
						
						std::vector<impl::ufunction> funcs = _class.second.func;
						{
							std::vector<std::string> parents = _class.second.parents;
							for (int i = 0; i < parents.size(); i++) {
								std::string parent = parents[i];
								const auto& it = data.find(parent);
								if (it == data.end()) continue;
								parents.insert(parents.end(), it->second.parents.begin(), it->second.parents.end());
								for (const auto& _f : it->second.func) {
									if (std::find(_class.second.func.begin(), _class.second.func.end(), _f) == _class.second.func.end()) {
										funcs.push_back(_f);
									}
								}
							}
						}

						if (funcs.size() > 0 && constructorFound) s += ",\n";
						for (int i = 0; i < funcs.size(); i++) {
							const impl::ufunction& f = funcs[i];
							const ::refl::store::uproperty_type ret_ptype = static_cast<::refl::store::uproperty_type>(static_cast<uint32_t>(f.ret_val));
							s += "\t\t{\"" + f.name + "\",{\"" + f.name + "\",\"" + f.ret_name + "\",static_cast<refl::store::uproperty_type>("
								+ std::to_string(static_cast<uint32_t>(f.ret_val)) + "),{";
							for (const std::pair<uint32_t, std::string>& p : f.args_val) {
								s += "{static_cast<::refl::store::uproperty_type>(" + std::to_string(p.first) + "),\"" + p.second + "\"},";
							}

							if (f.args_val.size() > 0)
								s[s.size() - 1] = ' ';
							s += "},[](void* ptr, std::vector<void*> args) {";
							if (ret_ptype == ::refl::store::uproperty_type::_void) {
								if (!f._static) s += " (*((" + cls + "*)ptr).*(&" + cls + "::" + f.name + "))(";
								else s += cls + "::" + f.name + "(";
								s += impl::set_method_args(f);
								s += "); return nullptr; } }}";
							}
							else if (ret_ptype == ::refl::store::uproperty_type::uclass_ptr) {
								if (!f._static) s += f.ret_name + " v = (*((" + cls + "*)ptr).*(&" + cls + "::" + f.name + "))(";
								else s += f.ret_name + " v = " + cls + "::" + f.name + "(";
								s += impl::set_method_args(f);
								s += "); return (void*)v; } }}";
							}
							else if (ret_ptype == ::refl::store::uproperty_type::constructor) {
								s += "return (void*)new " + f.ret_name + "(";
								s += impl::set_method_args(f);
								s += ");} }},\n";
								s += "\t\t{\"~constructor\",{\"~constructor\",\"void\",static_cast<refl::store::uproperty_type>(" + std::to_string(static_cast<uint32_t>(store::GetTypeInt("void"))) + "),{},";
								s += "[](void* ptr, std::vector<void*> args) {" + cls + "* p = (" + cls + "*)ptr; delete p; return nullptr;} }}";
							}
							else if (ret_ptype == ::refl::store::uproperty_type::_ptr) {
								if (!f._static) s += f.ret_name + " v = (*((" + cls + "*)ptr).*(&" + cls + "::" + f.name + "))(";
								else s += f.ret_name + " v = " + cls + "::" + f.name + "(";
								s += impl::set_method_args(f);
								s += "); return v; } }}";
							}
							else if (ret_ptype == ::refl::store::uproperty_type::uclass_ref) {
								if (!f._static) s += "void* v = &(*((" + cls + "*)ptr).*(&" + cls + "::" + f.name + "))(";
								else s += "void* v = &" + cls + "::" + f.name + "(";
								s += impl::set_method_args(f);
								s += "); return (void*)v; } }}";
							}
							else {
								if (!f._static) s += f.ret_name + " v = (*((" + cls + "*)ptr).*(&" + cls + "::" + f.name + "))(";
								else s += f.ret_name + " v = " + cls + "::" + f.name + "(";
								s += impl::set_method_args(f);
								s += "); " + f.ret_name + "* _ptr = new " + f.ret_name + "(v); return (void*)_ptr; } }}";
							}

							if (i != funcs.size()-1) s += ",\n";
						}
						s += "}});\n";
						s += "\t}\n\tinline static void Unload(::refl::store::storage* storage) {\n";
						s += "\t\tstorage->discard(\"" + cls + "\");\n\t}\n";
						s += "};\n";
						ret.push_back({ cls,s });
					}
					return ret;
				}

				inline std::vector<std::pair<std::string, std::string>> generateM(const char* _in, const char* fileN) {
					std::vector<std::pair<std::string, std::string>> ret;
					std::string m_in(_in);
					size_t pos = 0;
					
					bool next = true;
					size_t lstClssPos=  0;
					while (next) {
					std::string s = "#pragma once\n#include \"reflection/reflection.hpp\"\n#include \""+relativeInclude;
					std::pair < std::string, std::vector<std::string>> prs = impl::get_class(m_in, &lstClssPos, err);
					std::string cls = prs.first;
					pos = lstClssPos;
					if (cls.size() <= 0)
						break;
					size_t _pos = pos;
					size_t nextCls = impl::get_class(m_in, &_pos, err).first.size();
					std::string in;
					if (nextCls <= 0) {
						in = m_in.substr(pos, m_in.size()-pos);
						next = false;
					} else {
						in = m_in.substr(pos, _pos-pos);
					}
					s+= std::string(fileN)+"\"\nclass " + cls + "_Generated : public refl::class_generation {\n ";
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
					bool constructorFound = false;
					while (pos != std::string::npos) {
						impl::ufunction f = impl::get_constructor(in, cls, &pos, err, false);
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
						constructorFound = true;
					}
					pos = in.find("UFUNCTION()");
					if (pos != std::string::npos && constructorFound) s+= ",\n";
					while (pos != std::string::npos) {
						impl::ufunction f = impl::get_next_method(in, cls, &pos, err, false, false);
						const ::refl::store::uproperty_type ret_ptype = static_cast<::refl::store::uproperty_type>(static_cast<uint32_t>(f.ret_val));
						s+= "\t\t{\""+f.name+"\",{\""+f.name+"\",\""+f.ret_name+"\",static_cast<refl::store::uproperty_type>("
						+std::to_string(static_cast<uint32_t>(f.ret_val))+"),{";
						for (const std::pair<uint32_t, std::string>& p : f.args_val) {
							s+= "{static_cast<::refl::store::uproperty_type>("+std::to_string(p.first)+"),\""+p.second+"\"},";
						}
						
						if (f.args_val.size() > 0)
							s[s.size()-1] = ' ';
						s+= "},[](void* ptr, std::vector<void*> args) {";
						if (ret_ptype == ::refl::store::uproperty_type::_void) {
							if (!f._static) s+= " (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							else s+= cls+"::"+f.name+"(";
							s += impl::set_method_args(f);
							s +="); return nullptr; } }}";
						} else if (ret_ptype == ::refl::store::uproperty_type::uclass_ptr) {
							if (!f._static) s+= f.ret_name +" v = (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							else s+= f.ret_name +" v = " + cls+"::"+f.name+"(";
							s+= impl::set_method_args(f);
							s+="); return (void*)v; } }}";
						} else if (ret_ptype == ::refl::store::uproperty_type::constructor) {
							s+= "return (void*)new "+f.ret_name+"(";
							s+= impl::set_method_args(f);
							s +=");} }},\n";
							s+= "\t\t{\"~constructor\",{\"~constructor\",\"void\",static_cast<refl::store::uproperty_type>("+std::to_string(static_cast<uint32_t>(store::GetTypeInt("void")))+"),{},";
							s+= "[](void* ptr, std::vector<void*> args) {"+cls+"* p = ("+cls+"*)ptr; delete p; return nullptr;} }}";
						} else if (ret_ptype == ::refl::store::uproperty_type::_ptr) {
							if (!f._static) s+= f.ret_name +" v = (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							else s+= f.ret_name +" v = " + cls+"::"+f.name+"(";
							s+= impl::set_method_args(f);
							s+="); return v; } }}";
						}
						else if (ret_ptype == ::refl::store::uproperty_type::uclass_ref) {
							if (!f._static) s+= "void* v = &(*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							else s+= "void* v = &"+ cls+"::"+f.name+"(";
							s+= impl::set_method_args(f);
							s+="); return (void*)v; } }}";
						}
						else {
							if (!f._static) s+= f.ret_name +" v = (*(("+cls+"*)ptr).*(&"+cls+"::"+f.name+"))(";
							else s+= f.ret_name +" v = "+ cls+"::"+f.name+"(";
							s+= impl::set_method_args(f);
							s+="); "+f.ret_name+"* _ptr = new "+f.ret_name+"(v); return (void*)_ptr; } }}";
						}
						
						if (pos != std::string::npos) s+= ",\n";
					}
					s+= "}});\n";
					s += "\t}\n\tinline static void Unload(::refl::store::storage* storage) {\n";
					s += "\t\tstorage->discard(\""+cls+"\");\n\t}\n";
					s += "};\n";
					ret.push_back({cls,s});
					}
					return ret;
				}

				inline void set_output(const char* c) {
					outputDir = std::string(c);
				}

				inline void set_relative_include(const char* c) {
					relativeInclude = std::string(c);
				}

			private:
				::refl::err::err_hndl* err;
				std::string outputDir;
				std::vector<std::string> clss;
				std::string relativeInclude = "";
		};

		namespace impl {

			static std::string set_method_args(const impl::ufunction& f) {
				std::string s = "";
				for (uint32_t i = 0; i < f.args_val.size(); i++) {
					std::pair<uint32_t, std::string> p = f.args_val[i];
					::refl::store::uproperty_type _type = static_cast<::refl::store::uproperty_type>(p.first);
					if (_type == ::refl::store::uproperty_type::_ptr || _type == ::refl::store::uproperty_type::uclass_ptr) {
						s += "*("+p.second+"*)args["+std::to_string(i)+"]";
					} else if (_type == ::refl::store::uproperty_type::uclass_ref){
						std::string type = p.second;
						type.erase(type.size()-1);
						s += "("+type+"&)(*("+type+"*)args["+std::to_string(i)+"])";
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
			static std::pair<std::string, std::vector < std::string>> get_class(const std::string& in, size_t* pos, ::refl::err::err_hndl* err) {
					size_t uclassPos = in.find("UCLASS()", *pos);

					if (uclassPos == std::string::npos) {
						//err->setErrorStr("Error 0: UCLASS not found");
						return { "" , {} };
					}

					size_t classPos = in.find("class ", uclassPos+7);

					if (classPos == std::string::npos) {
						//err->setErrorStr("Error 1: class not found");
						return { "" , {} };
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

					std::string clssname = in.substr(clssStart, nextSpace - clssStart);

					bool hasParents = false;

					size_t lastInherit = nextSpace;

					while (cC != '{' && cC != ';') {
						cC = in[++nextSpace];
						if (cC == ':') {
							hasParents = true;
							lastInherit = nextSpace+1;
							break;
						}
					}

					bool isPublic = false;
					std::vector<std::string> parents = {};
					while (hasParents && cC != '{' && cC != ';') {
						cC = in[++nextSpace];
						if (cC != ' ' && cC != ',' && cC != '\n' && cC != '\t' && cC != '{' && cC != ';' && cC != ':')
							lastInherit = nextSpace;
						while (cC != ' ' && cC != ',' && cC != '\n' && cC != '\t' && cC != '{' && cC != ';') {
							cC = in[++nextSpace];
						}
						
						if (nextSpace - lastInherit <= 0) continue;
						std::string str = in.substr(lastInherit, nextSpace - lastInherit);
						if (str == "public" && !isPublic) {
							lastInherit = nextSpace + 1;
							isPublic = true;
						}
						else {
							isPublic = false;
							lastInherit = nextSpace + 1;
							parents.push_back(str);
						}
						

					}

					return { clssname, parents };
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

			static ufunction get_constructor(const std::string& in, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err, bool _static) {
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
				if (typeName == "inline" || typeName == "virtual" || typeName == "static" || typeName == "const") {
					*pos = nextSpace-14;
					if (typeName == "static") _static = true;
					return get_constructor(in, clss, pos, err, _static);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=' || cC == '*' || cC == '&') {
					if (cC == '*') {
						typeName += "*";
					}
					else if (cC == '&') {
						typeName += "&";
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
					/*
					if (cC == '&') {
						err->setErrorStr(std::string(memName + "Error: passing by reference not allowed use pointer!").c_str() );
						*pos = std::string::npos;
						return {};
					}
					*/
					if (cC != ' ' && cC != '\t' && cC != '\n' && spacePos != 0) {
						spacePos = 0;
						uProp = nextSpace;
					}
					if ( (cC == ' ' || cC == '\t' || cC == '\n') && spacePos == 0) {
						std::string varName = in.substr(uProp, nextSpace-uProp);
						if (varName == "const") {
							spacePos = 0;
							cC = in[++nextSpace];
							uProp = nextSpace;
							continue;
						}
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
				return {clss, ::refl::store::GetTypeInt(typeName), typeName, v, _static};
			}

			static ufunction get_next_method(const std::string& in, const std::string& clss, size_t* pos, ::refl::err::err_hndl* err, bool _static, bool _virtual) {
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
				if (typeName == "inline" || typeName == "virtual" || typeName == "static" || typeName == "const") {
					*pos = nextSpace-11;
					if (typeName == "static") _static = true;
					if (typeName == "virtual") _virtual = true;
					return get_next_method(in, clss, pos, err, _static, _virtual);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == '\t' || cC == '=' || cC == '*' || cC == '&' ) {
					if (cC == '*') {
						typeName += "*";
					}
					else if (cC == '&') {
						typeName += "&";
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
					/*
					if (cC == '&') {
						err->setErrorStr(std::string(memName + "Error: passing by reference not allowed use pointer!").c_str() );
						*pos = in.find("UFUNCTION()", newL);
						return {};
					}
					*/
					if (cC != ' ' && cC != '\t' && cC != '\n' && spacePos != 0) {
						spacePos = 0;
						uProp = nextSpace;
					}
					if ( (cC == ' ' || cC == '\t' || cC == '\n') && spacePos == 0) {
						std::string varName = in.substr(uProp, nextSpace-uProp);
						if (varName == "const") {
							spacePos = 0;
							cC = in[++nextSpace];
							uProp = nextSpace;
							continue;
						}
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

				return {memName, ::refl::store::GetTypeInt(typeName), typeName, v, _static, _virtual};
			}
		}

	}
}
