#pragma once

#include <cstdint>
#include "../shared_config.h"

struct RoutingDecision {
    InputRoutingMode mode;
    InputRoutingOwner owner;
    InputRoutingReason reason;
    bool hookContextActive;
    bool tsfContextActive;
    bool blacklisted;
    uint32_t decisionSequence;
};

bool InitializeInputRoutingCoordinator();
void ShutdownInputRoutingCoordinator();

bool SetInputRoutingMode(InputRoutingMode mode, InputRoutingReason reason);
bool TryParseInputRoutingModeString(const wchar_t* text, InputRoutingMode* mode);
InputRoutingReason InputRoutingConfiguredReason(InputRoutingMode mode);

RoutingDecision RefreshHookRoutingDecision(bool hookContextActive, bool blacklisted);
RoutingDecision RefreshTsfRoutingDecision(bool tsfContextActive, bool blacklisted);
RoutingDecision GetInputRoutingDecisionSnapshot();

RoutingDecision ResolveRoutingDecision(
    InputRoutingMode mode,
    bool hookContextActive,
    bool tsfContextActive,
    bool blacklisted);

const wchar_t* InputRoutingModeToString(InputRoutingMode mode);
const wchar_t* InputRoutingOwnerToString(InputRoutingOwner owner);
const wchar_t* InputRoutingReasonToString(InputRoutingReason reason);
