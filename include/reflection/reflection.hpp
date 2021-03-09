#pragma once
#include "generate.hpp"
#include "error.hpp"
#include "storage.hpp"
#include "generation.hpp"

namespace refl {
	
	class reflector* s_reflector = nullptr;

	class reflector {
		private:
			inline reflector() : gen(&err), st(&err) {
				s_reflector = this;
			}

		public:
			inline ~reflector() {
				s_reflector = nullptr;
			}
			inline void Generate(const char* in, const char* out){ return gen.generate(in, out);}
			inline const char* GenerateM(const char* in) { return gen.generateM(in);}
			inline bool HasError() const { return err.HasError(); }
			inline const char* GetError()  { return err.GetError();}
			inline store::storage* GetStorage() { return &st; }
			static reflector* Get() { return s_reflector ? s_reflector : new reflector(); }
		private:
			err::err_hndl err;
			gen::generator gen;
			store::storage st;

	};

}
