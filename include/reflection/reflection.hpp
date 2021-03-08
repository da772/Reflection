#pragma once
#include "generate.hpp"
#include "error.hpp"


namespace refl {
	
	class reflector {
		public:
			inline reflector() : gen(&err) {

			}
			inline ~reflector() {

			}

			

			inline bool HasError() const { return err.HasError(); }
			inline const char* GetError()  { return err.GetError();}
		private:
			err::err_hndl err;
			gen::generator gen;

	};

}
