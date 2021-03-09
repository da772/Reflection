#pragma once
#include <cstddef>
#include <cstdint>

namespace refl {
        class class_generation {
            public:
                virtual void Load() = 0;
                virtual void Unload() = 0;
        };
        
}

