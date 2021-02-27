#include "SimConnectInput.h"
#include "SimConnectSink.h"
#include "SimConnectSinkAutopilotLaws.h"
#include "SimConnectSinkAutopilotStateMachine.h"
#include "SimConnectSinkAutothrust.h"
#include "SimConnectSource.h"
#include "SimConnectSourceAutopilotEvents.h"
#include "SimConnectSourceAutopilotLaws.h"
#include "SimConnectSourceAutopilotStateMachine.h"
#include "SimConnectSourceLocalVariables.h"
#include "SimConnectSourceLocalVariablesAutothrust.h"

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
    SimConnectSinkAutopilotLaws,
    simconnect::toolbox::blocks::SimConnectSinkAutopilotLaws,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSinkAutopilotStateMachine,
    simconnect::toolbox::blocks::SimConnectSinkAutopilotStateMachine,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSinkAutothrust,
    simconnect::toolbox::blocks::SimConnectSinkAutothrust,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSource,
    simconnect::toolbox::blocks::SimConnectSource,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceAutopilotEvents,
    simconnect::toolbox::blocks::SimConnectSourceAutopilotEvents,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceAutopilotLaws,
    simconnect::toolbox::blocks::SimConnectSourceAutopilotLaws,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceAutopilotStateMachine,
    simconnect::toolbox::blocks::SimConnectSourceAutopilotStateMachine,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceLocalVariables,
    simconnect::toolbox::blocks::SimConnectSourceLocalVariables,
    blockfactory::core::Block
);

SHLIBPP_DEFINE_SHARED_SUBCLASS(
    SimConnectSourceLocalVariablesAutothrust,
    simconnect::toolbox::blocks::SimConnectSourceLocalVariablesAutothrust,
    blockfactory::core::Block
);
