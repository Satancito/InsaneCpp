#include <Insane/Insane.h>

USING_NS_INSANE;

static StdUniquePtr<bool> _Debug = nullptr;

bool DebugExtensions::Debug(bool debug)
{
    if(!_Debug)
    {
        _Debug = std::make_unique<bool>(debug);
    }
    return *_Debug;
}

bool DebugExtensions::IsDebug()
{
    if(!_Debug)
    {
        _Debug = std::make_unique<bool>(false);
    }
    return *_Debug;
}
