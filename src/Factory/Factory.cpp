#include "SimConnectInput.h"
#include "SimConnectSink.h"
#include "SimConnectSource.h"

#pragma warning(disable : 4267)
#pragma warning(disable : 4996)

// Class factory API
#include <shlibpp/SharedLibraryClassApi.h>

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectInput,
    simconnect::toolbox::blocks::SimConnectInput,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSink,
    simconnect::toolbox::blocks::SimConnectSink,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSource,
    simconnect::toolbox::blocks::SimConnectSource,
    blockfactory::core::Block
);
