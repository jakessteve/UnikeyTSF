#include <gtest/gtest.h>

#include "engine/input_routing.h"

TEST(RoutingTests, HookPrimaryPrefersHookWhenBothContextsPresent) {
    const RoutingDecision decision = ResolveRoutingDecision(
        ROUTING_HOOK_PRIMARY,
        true,
        true,
        false);

    EXPECT_EQ(decision.mode, ROUTING_HOOK_PRIMARY);
    EXPECT_EQ(decision.owner, ROUTING_OWNER_HOOK);
    EXPECT_EQ(decision.reason, ROUTING_REASON_HOOK_PRIMARY_DEFAULT);
}

TEST(RoutingTests, TsfPrimaryPrefersTsfWhenBothContextsPresent) {
    const RoutingDecision decision = ResolveRoutingDecision(
        ROUTING_TSF_PRIMARY,
        true,
        true,
        false);

    EXPECT_EQ(decision.mode, ROUTING_TSF_PRIMARY);
    EXPECT_EQ(decision.owner, ROUTING_OWNER_TSF);
    EXPECT_EQ(decision.reason, ROUTING_REASON_TSF_PRIMARY_DEFAULT);
}

TEST(RoutingTests, FallbackScopedChoosesOwnerByFocusedContext) {
    const RoutingDecision tsfContext = ResolveRoutingDecision(
        ROUTING_FALLBACK_SCOPED,
        true,
        true,
        false);
    EXPECT_EQ(tsfContext.owner, ROUTING_OWNER_TSF);
    EXPECT_EQ(tsfContext.reason, ROUTING_REASON_FALLBACK_SCOPED_TSF_CONTEXT);

    const RoutingDecision hookContext = ResolveRoutingDecision(
        ROUTING_FALLBACK_SCOPED,
        true,
        false,
        false);
    EXPECT_EQ(hookContext.owner, ROUTING_OWNER_HOOK);
    EXPECT_EQ(hookContext.reason, ROUTING_REASON_FALLBACK_SCOPED_HOOK_CONTEXT);
}

TEST(RoutingTests, DeterministicDiagnosticsAreStableForSameInputs) {
    const RoutingDecision a = ResolveRoutingDecision(
        ROUTING_FALLBACK_SCOPED,
        true,
        false,
        false);
    const RoutingDecision b = ResolveRoutingDecision(
        ROUTING_FALLBACK_SCOPED,
        true,
        false,
        false);

    EXPECT_EQ(a.mode, b.mode);
    EXPECT_EQ(a.owner, b.owner);
    EXPECT_EQ(a.reason, b.reason);

    EXPECT_STREQ(InputRoutingModeToString(a.mode), L"fallback_scoped");
    EXPECT_STREQ(InputRoutingOwnerToString(a.owner), L"hook");
    EXPECT_STREQ(InputRoutingReasonToString(a.reason), L"fallback_scoped_hook_context");
}

TEST(RoutingTests, CoordinatorMergesHookAndTsfContextAcrossRefreshOrder) {
    ASSERT_TRUE(InitializeInputRoutingCoordinator());

    ASSERT_TRUE(SetInputRoutingMode(ROUTING_FALLBACK_SCOPED, ROUTING_REASON_UNSPECIFIED));

    const RoutingDecision hookFirst = RefreshHookRoutingDecision(true, false);
    EXPECT_EQ(hookFirst.owner, ROUTING_OWNER_HOOK);
    EXPECT_EQ(hookFirst.reason, ROUTING_REASON_FALLBACK_SCOPED_HOOK_CONTEXT);

    const RoutingDecision tsfSecond = RefreshTsfRoutingDecision(true, false);
    EXPECT_EQ(tsfSecond.owner, ROUTING_OWNER_TSF);
    EXPECT_EQ(tsfSecond.reason, ROUTING_REASON_FALLBACK_SCOPED_TSF_CONTEXT);

    RoutingDecision snapshot = GetInputRoutingDecisionSnapshot();
    EXPECT_EQ(snapshot.owner, ROUTING_OWNER_TSF);
    EXPECT_TRUE(snapshot.hookContextActive);
    EXPECT_TRUE(snapshot.tsfContextActive);

    ASSERT_TRUE(SetInputRoutingMode(ROUTING_HOOK_PRIMARY, ROUTING_REASON_UNSPECIFIED));
    const RoutingDecision clearedHook = RefreshHookRoutingDecision(false, false);
    EXPECT_EQ(clearedHook.owner, ROUTING_OWNER_TSF);
    EXPECT_FALSE(clearedHook.hookContextActive);
    EXPECT_TRUE(clearedHook.tsfContextActive);

    const RoutingDecision tsfFirst = RefreshTsfRoutingDecision(true, false);
    EXPECT_EQ(tsfFirst.owner, ROUTING_OWNER_TSF);
    EXPECT_EQ(tsfFirst.reason, ROUTING_REASON_HOOK_PRIMARY_TSF_ONLY_CONTEXT);

    const RoutingDecision hookSecond = RefreshHookRoutingDecision(true, false);
    EXPECT_EQ(hookSecond.owner, ROUTING_OWNER_HOOK);
    EXPECT_EQ(hookSecond.reason, ROUTING_REASON_HOOK_PRIMARY_DEFAULT);

    snapshot = GetInputRoutingDecisionSnapshot();
    EXPECT_EQ(snapshot.owner, ROUTING_OWNER_HOOK);
    EXPECT_TRUE(snapshot.hookContextActive);
    EXPECT_TRUE(snapshot.tsfContextActive);

    ShutdownInputRoutingCoordinator();
}

TEST(RoutingTests, SetInputRoutingModeUsesConfiguredReasonWithoutFocusedContext) {
    ASSERT_TRUE(InitializeInputRoutingCoordinator());

    const RoutingDecision clearedHook = RefreshHookRoutingDecision(false, false);
    EXPECT_EQ(clearedHook.owner, ROUTING_OWNER_NONE);

    ASSERT_TRUE(SetInputRoutingMode(
        ROUTING_TSF_PRIMARY,
        ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY));

    const RoutingDecision snapshot = GetInputRoutingDecisionSnapshot();
    EXPECT_EQ(snapshot.mode, ROUTING_TSF_PRIMARY);
    EXPECT_EQ(snapshot.owner, ROUTING_OWNER_NONE);
    EXPECT_EQ(snapshot.reason, ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY);

    ShutdownInputRoutingCoordinator();
}

TEST(RoutingTests, ParsesRoutingModeStringsAndConfiguredReasons) {
    InputRoutingMode mode = ROUTING_HOOK_PRIMARY;
    ASSERT_TRUE(TryParseInputRoutingModeString(L"hook_primary", &mode));
    EXPECT_EQ(mode, ROUTING_HOOK_PRIMARY);
    EXPECT_EQ(InputRoutingConfiguredReason(mode), ROUTING_REASON_STARTUP_CONFIGURED_HOOK_PRIMARY);

    ASSERT_TRUE(TryParseInputRoutingModeString(L"tsf_primary", &mode));
    EXPECT_EQ(mode, ROUTING_TSF_PRIMARY);
    EXPECT_EQ(InputRoutingConfiguredReason(mode), ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY);

    ASSERT_TRUE(TryParseInputRoutingModeString(L"fallback_scoped", &mode));
    EXPECT_EQ(mode, ROUTING_FALLBACK_SCOPED);
    EXPECT_EQ(InputRoutingConfiguredReason(mode), ROUTING_REASON_STARTUP_CONFIGURED_FALLBACK_SCOPED);

    EXPECT_FALSE(TryParseInputRoutingModeString(L"invalid_mode", &mode));
}
