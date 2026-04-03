#include <gtest/gtest.h>

#include "engine/tsf_registration.h"

TEST(TsfDiagnosticsTests, MapsReadyStateToNoNextStep)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = true;
    observation.profilePresent = true;
    observation.profileActive = true;
    observation.dllHr = S_OK;
    observation.profileHr = S_OK;

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);

    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kReady);
    EXPECT_STREQ(report.nextStep, L"none");
}

TEST(TsfDiagnosticsTests, MapsMissingDllToActionableStatus)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = false;
    observation.profilePresent = false;
    observation.profileActive = false;
    observation.dllHr = S_OK;
    observation.profileHr = S_OK;

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);

    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kDllNotRegistered);
    EXPECT_STREQ(TsfDiagnosticsStatusToString(report.status), L"dll_not_registered");
    EXPECT_STREQ(report.nextStep, L"run_unikeytsf_/register");
}

TEST(TsfDiagnosticsTests, MapsMissingProfileToActionableStatus)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = true;
    observation.profilePresent = false;
    observation.profileActive = false;
    observation.dllHr = S_OK;
    observation.profileHr = S_OK;

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);

    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kProfileMissing);
    EXPECT_STREQ(TsfDiagnosticsStatusToString(report.status), L"profile_missing");
    EXPECT_STREQ(report.nextStep, L"run_unikeytsf_/register_then_reopen_windows_input_settings");
}

TEST(TsfDiagnosticsTests, MapsInactiveProfileToActionableStatus)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = true;
    observation.profilePresent = true;
    observation.profileActive = false;
    observation.dllHr = S_OK;
    observation.profileHr = S_OK;

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);

    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kProfileInactive);
    EXPECT_STREQ(TsfDiagnosticsStatusToString(report.status), L"profile_inactive");
    EXPECT_STREQ(report.nextStep, L"activate_unikey_tsf_profile_in_windows_input_settings");
}

TEST(TsfDiagnosticsTests, FormatsMachineReadableKeyValueContract)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = true;
    observation.profilePresent = true;
    observation.profileActive = false;
    observation.dllHr = S_OK;
    observation.profileHr = S_OK;

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);
    const std::wstring output = FormatTsfDiagnosticsKeyValue(report);

    EXPECT_NE(output.find(L"status=profile_inactive\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"dll_registered=1\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"profile_present=1\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"profile_active=0\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"dll_hr=0x00000000\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"profile_hr=0x00000000\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"next_step=activate_unikey_tsf_profile_in_windows_input_settings\n"), std::wstring::npos);
}

TEST(TsfDiagnosticsTests, PrefersCheckFailedStatusesWhenObservationFailed)
{
    TsfDiagnosticsObservation dllFailure = {};
    dllFailure.dllRegistered = false;
    dllFailure.profilePresent = false;
    dllFailure.profileActive = false;
    dllFailure.dllHr = E_FAIL;
    dllFailure.profileHr = S_OK;

    TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(dllFailure);
    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kDllCheckFailed);

    TsfDiagnosticsObservation profileFailure = {};
    profileFailure.dllRegistered = true;
    profileFailure.profilePresent = false;
    profileFailure.profileActive = false;
    profileFailure.dllHr = S_OK;
    profileFailure.profileHr = E_FAIL;

    report = BuildTsfDiagnosticsReport(profileFailure);
    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kProfileCheckFailed);
}

TEST(TsfDiagnosticsTests, FormatsCheckFailedStatusesAsMachineReadableOutput)
{
    TsfDiagnosticsObservation observation = {};
    observation.dllRegistered = true;
    observation.profilePresent = false;
    observation.profileActive = false;
    observation.dllHr = S_OK;
    observation.profileHr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);

    const TsfDiagnosticsReport report = BuildTsfDiagnosticsReport(observation);
    const std::wstring output = FormatTsfDiagnosticsKeyValue(report);

    EXPECT_EQ(report.status, TsfDiagnosticsStatus::kProfileCheckFailed);
    EXPECT_NE(output.find(L"status=profile_check_failed\n"), std::wstring::npos);
    EXPECT_NE(output.find(L"profile_hr=0x80070005\n"), std::wstring::npos);
}
