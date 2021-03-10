#pragma once
#include "error.hpp"
#include "storage.hpp"
#include "generation.hpp"
#include "generate.hpp"

namespace refl {

	namespace impl {
		static void* s_reflector = nullptr;
	}

	class reflector {
		private:
			inline reflector() : gen(&err), st(&err) {
				impl::s_reflector = this;
			}

		public:
			inline ~reflector() {
				impl::s_reflector = nullptr;
			}
			inline void Generate(const char* in){ return gen.generate(in);}
			inline std::pair<std::string, std::string> GenerateM(const char* in) { return gen.generateM(in);}
			inline bool HasError() const { return err.HasError(); }
			inline const char* GetError()  { return err.GetError();}
			inline store::storage* GetStorage() { return &st; }
			inline void SetOutputDir(const char* outputDir) { gen.set_output(outputDir); }
			static reflector* Get() { return impl::s_reflector != nullptr ? (reflector*)impl::s_reflector : new reflector(); }
			static void Destroy() { if (impl::s_reflector) delete (reflector*)impl::s_reflector; }
		private:
			err::err_hndl err;
			gen::generator gen;
			store::storage st;

	};

}
