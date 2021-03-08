#pragma once
#include "error.hpp"

namespace refl {
	namespace gen {
		class generator {
			public:
				inline generator(::refl::err::err_hndl* error) : err(error) {

				}

				inline void generate(const char* file, const char* out) {
					
				}

			private:
				::refl::err::err_hndl* err;
		};

	}
}
