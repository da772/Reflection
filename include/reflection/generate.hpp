#pragma once
#include "error.hpp"
#include "storage.hpp"

namespace refl {
	namespace gen {
		namespace impl {
			static std::string get_class(const std::string& s, ::refl::err::err_hndl* err);
			static std::pair<std::string, std::string> get_next_member(const std::string& s, size_t* pos);
		}
		class generator {
			public:
				inline generator(::refl::err::err_hndl* error) : err(error) {

				}

				inline void generate(const char* in, const char* out) {
					
				}

				inline const char* generateM(const char* _in) {
					char* ch = nullptr;
					std::string in(_in);
					std::string s = "#pragma once\n#include \"reflection/reflection.hpp\"\n#include \"";
					std::string cls = impl::get_class(in, err);
					s+= cls+".h\"\nclass " + cls + "_Generated : public refl::class_generation {\n ";
					s+= "\tpublic:\n\tinline virtual void Load() override {\n";
					s+= "\t\trefl::store::storage* storage = refl::reflector::Get()->GetStorage();\n";
					s+= "\t\tstorage->store(\""+cls+"\",{\""+cls+"\",{\n";
					size_t pos = in.find("UPROPERTY()");
					while (pos != std::string::npos) {
						std::pair<std::string, std::string> m = impl::get_next_member(in, &pos);
						s+= "\t\t{\""+m.second+"\",static_cast<refl::store::uproperty_type>("
						+std::to_string(static_cast<uint32_t>(store::GetTypeInt(m.first)))+"),refl::store::offsetOf(&" +cls+"::"+ m.second+") }";
						if (pos != std::string::npos) s+= ",\n";
					}
					s+="}, {}});\n";
					s += "\t}\n\tinline virtual void Unload() override {\n";
					s += "\t\trefl::store::storage* storage = refl::reflector::Get()->GetStorage();\n";
					s += "\t\tstorage->discard(\""+cls+"\");\n\t}\n";

					s += "};\n";

					ch = (char*)calloc(s.size()+1, sizeof(char));
					memcpy(ch, &s[0], s.size()*sizeof(char));
					return ch;
				}

			private:
				::refl::err::err_hndl* err;
		};

		namespace impl {
			static std::string get_class(const std::string& in, ::refl::err::err_hndl* err) {
					size_t uclassPos = in.find("UCLASS()");

					if (uclassPos == std::string::npos) {
						err->setErrorStr("Error 0: UCLASS not found");
						return nullptr;
					}

					size_t classPos = in.find("class ", uclassPos+7);

					if (classPos == std::string::npos) {
						err->setErrorStr("Error 1: class not found");
						return nullptr;
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

					return in.substr(clssStart, nextSpace-clssStart);
			}
			static std::pair<std::string, std::string> get_next_member(const std::string& in, size_t* pos) {
				size_t uProp = (*pos)+11;
				char cC = in[uProp];
				while (cC == ' ' || cC == '\n' || cC == ':' || cC == '\t' || cC == '=') {
					cC = in[++uProp];
				}
				size_t nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != ':' && cC != '\t' && cC != '{') {
					cC = in[++nextSpace];
				}
				size_t newL = in.find(";", uProp);
				std::string typeName = in.substr(uProp, nextSpace-uProp);
				if (typeName == "unsigned") {
					size_t _uProp = nextSpace;
					cC = in[uProp];
					while (cC == ' ' || cC == '\n' || cC == ':' || cC == '\t' || cC == '=') {
						cC = in[++_uProp];
					}
					nextSpace = _uProp;
					while (cC != ' ' && cC != '\n' && cC != ':' && cC != '\t' && cC != '{') {
						cC = in[++nextSpace];
					}
					typeName = in.substr(uProp, nextSpace-uProp);
				}
				uProp = nextSpace;
				while (cC == ' ' || cC == '\n' || cC == ':' || cC == '\t' || cC == '=' || cC == '*') {
					if (cC == '*') {
						typeName += "*";
					}
					cC = in[++uProp];
				}
				nextSpace = uProp;
				while (cC != ' ' && cC != '\n' && cC != ':' && cC != '\t' && cC != '{') {
					cC = in[++nextSpace];
				}
				std::string memName = in.substr(uProp, nextSpace-uProp);
				*pos = in.find("UPROPERTY()", newL);
				return {typeName,memName};
			}
		}

	}
}
