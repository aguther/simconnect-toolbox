#include "SimConnectInput.h"
#include "SimConnectSink.h"
#include "SimConnectSource.h"
#include "SimConnectSourceEvents.h"
#include "SimConnectSinkFbw.h"
#include "SimConnectSourceFbw.h"
#include "SimConnectSourceFbwFg.h"

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

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceEvents,
    simconnect::toolbox::blocks::SimConnectSourceEvents,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSinkFbw,
    simconnect::toolbox::blocks::SimConnectSinkFbw,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceFbw,
    simconnect::toolbox::blocks::SimConnectSourceFbw,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceFbwFg,
    simconnect::toolbox::blocks::SimConnectSourceFbwFg,
    blockfactory::core::Block
);
