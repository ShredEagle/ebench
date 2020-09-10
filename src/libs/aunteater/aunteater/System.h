#pragma once

#include "Engine.h"
#include "Timer.h"

namespace aunteater
{

    template <ENGINE_TMP_PARAMS>
    class Engine;

    class System
    {
    public:
        virtual void update(const Timer aTime)=0;
    };

} // namespace aunteater
