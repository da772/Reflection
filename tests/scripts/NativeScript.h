#include "reflection/macros.h"

class NativeScript {
    public:
        UFUNCTION()
        virtual void Begin() {};
        UFUNCTION()
        virtual void Update() {};
        UFUNCTION()
        virtual void End() {};
};
