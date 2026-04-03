cmake_minimum_required(VERSION 3.20)
project(UniKeyTSF LANGUAGES CXX)

# Use extraction timestamps for FetchContent downloads (CMP0135)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Unicode build
add_definitions(-DUNICODE -D_UNICODE)

# =============================================================================
# UniKeyTSF.exe -- Manager & Bootstrapper
# =============================================================================
include(FetchContent)
FetchContent_Declare(
    webview2
    URL "https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.2151.40"
)
FetchContent_MakeAvailable(webview2)

add_executable(UniKeyTSF WIN32
    src/main.cpp
    src/ui/tray_icon.cpp
    src/ui/settings_dialog.cpp
    src/ui/settings_message_contract.cpp
    src/ui/settings_webview.cpp
    src/config/ipc_manager.cpp
    src/config/blacklist.cpp
    src/engine/hook_manager.cpp
    src/engine/key_translate.cpp
    src/engine/input_routing.cpp
    src/engine/typing_settings.cpp
    src/engine/tsf_registration.cpp
    src/engine/vn_engine.cpp
    src/engine/vn_ortho.cpp
    src/engine/macro.cpp
    src/toolkit/clipboard.cpp
    src/UniKeyTSF.rc
)

target_include_directories(UniKeyTSF PRIVATE src)

target_link_libraries(UniKeyTSF PRIVATE
    user32
    shell32
    gdi32
    kernel32
    advapi32
    ole32
    oleaut32
    comdlg32
    comctl32
)

target_include_directories(UniKeyTSF PRIVATE ${webview2_SOURCE_DIR}/build/native/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries(UniKeyTSF PRIVATE ${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib)
else()
    target_link_libraries(UniKeyTSF PRIVATE ${webview2_SOURCE_DIR}/build/native/x86/WebView2Loader.dll.lib)
endif()

# =============================================================================
# uktsf_core64.dll -- TSF COM Server
# =============================================================================
add_library(uktsf_core64 SHARED
    src/tsf/dllmain.cpp
    src/tsf/class_factory.cpp
    src/tsf/text_service.cpp
    src/tsf/composition.cpp
    src/tsf/key_event_sink.cpp
    src/engine/reconversion_word.cpp
    src/engine/input_routing.cpp
    src/engine/typing_settings.cpp
    src/engine/vn_engine.cpp
    src/engine/vn_ortho.cpp
    src/engine/macro.cpp
    src/config/blacklist.cpp
    src/tsf/uktsf_core.def
)

target_include_directories(uktsf_core64 PRIVATE src src/tsf src/engine)

target_link_libraries(uktsf_core64 PRIVATE
    user32
    kernel32
    advapi32
    ole32
    oleaut32
    uuid
)

# =============================================================================
# uktsf_core32.dll -- TSF COM Server (32-bit companion)
# =============================================================================
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_library(uktsf_core32 SHARED
        src/tsf/dllmain.cpp
        src/tsf/class_factory.cpp
        src/tsf/text_service.cpp
        src/tsf/composition.cpp
        src/tsf/key_event_sink.cpp
        src/engine/reconversion_word.cpp
        src/engine/input_routing.cpp
        src/engine/typing_settings.cpp
        src/engine/vn_engine.cpp
        src/engine/vn_ortho.cpp
        src/engine/macro.cpp
        src/config/blacklist.cpp
        src/tsf/uktsf_core32.def
    )

    target_include_directories(uktsf_core32 PRIVATE src src/tsf src/engine)

    target_link_libraries(uktsf_core32 PRIVATE
        user32
        kernel32
        advapi32
        ole32
        oleaut32
        uuid
    )

    if(MSVC)
        target_compile_options(uktsf_core32 PRIVATE /W4 /WX- /utf-8)
    endif()
endif()

# =============================================================================
# Compiler options
# =============================================================================
if(MSVC)
    target_compile_options(UniKeyTSF PRIVATE /W4 /WX- /utf-8)
    target_compile_options(uktsf_core64 PRIVATE /W4 /WX- /utf-8)
endif()

# =============================================================================
# GoogleTest -- Unit Tests
# =============================================================================
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(UniKeyTests
    tests/engine_test.cpp
    tests/spellcheck_test.cpp
    src/engine/key_translate.cpp
    src/engine/reconversion_word.cpp
    src/engine/vn_engine.cpp
    src/engine/vn_ortho.cpp
    src/engine/macro.cpp
)

target_include_directories(UniKeyTests PRIVATE src)
target_link_libraries(UniKeyTests PRIVATE GTest::gtest_main user32)

if(MSVC)
    target_compile_options(UniKeyTests PRIVATE /W4 /WX- /utf-8)
endif()

include(GoogleTest)
gtest_discover_tests(UniKeyTests)

# IPC & Share Memory Tests
add_executable(IPCTests
    tests/ipc_test.cpp
    src/config/ipc_manager.cpp
)

target_include_directories(IPCTests PRIVATE src)
target_link_libraries(IPCTests PRIVATE GTest::gtest_main)

if(MSVC)
    target_compile_options(IPCTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(IPCTests)

# Clipboard & Charset Conversion Tests
add_executable(ClipboardTests
    tests/clipboard_test.cpp
)

target_include_directories(ClipboardTests PRIVATE src)
target_link_libraries(ClipboardTests PRIVATE GTest::gtest_main)

if(MSVC)
    target_compile_options(ClipboardTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(ClipboardTests)

# Settings UI E2E Acceptance Tests
add_executable(SettingsUITests
    tests/e2e/settings_ui_test.cpp
    src/ui/settings_dialog.cpp
    src/ui/settings_message_contract.cpp
    src/ui/settings_webview.cpp
    src/config/ipc_manager.cpp
    src/config/blacklist.cpp
    src/UniKeyTSF.rc
)

target_include_directories(SettingsUITests PRIVATE src)
target_link_libraries(SettingsUITests PRIVATE 
    GTest::gtest_main
    comctl32
    uxtheme
    dwmapi
    user32
    kernel32
    advapi32
    ole32
    shell32
    comdlg32
)

target_include_directories(SettingsUITests PRIVATE ${webview2_SOURCE_DIR}/build/native/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries(SettingsUITests PRIVATE ${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib)
else()
    target_link_libraries(SettingsUITests PRIVATE ${webview2_SOURCE_DIR}/build/native/x86/WebView2Loader.dll.lib)
endif()

if(MSVC)
    target_compile_options(SettingsUITests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(SettingsUITests)

# Settings WebView/native contract tests
add_executable(SettingsMessageContractTests
    tests/settings_message_contract_test.cpp
    src/ui/settings_message_contract.cpp
)

target_include_directories(SettingsMessageContractTests PRIVATE src)
target_link_libraries(SettingsMessageContractTests PRIVATE GTest::gtest_main)

if(MSVC)
    target_compile_options(SettingsMessageContractTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(SettingsMessageContractTests)

# Characterization-first frozen behavior contract tests
add_executable(CharacterizationTests
    tests/characterization_test.cpp
    tests/characterization_harness.cpp
    src/engine/typing_settings.cpp
    src/engine/reconversion_word.cpp
    src/engine/vn_engine.cpp
    src/engine/vn_ortho.cpp
    src/engine/macro.cpp
)

target_include_directories(CharacterizationTests PRIVATE src tests)
target_link_libraries(CharacterizationTests PRIVATE GTest::gtest_main)

if(MSVC)
    target_compile_options(CharacterizationTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(CharacterizationTests)

# Routing ownership seam arbitration tests
add_executable(RoutingTests
    tests/routing_test.cpp
    src/engine/input_routing.cpp
)

target_include_directories(RoutingTests PRIVATE src)
target_link_libraries(RoutingTests PRIVATE GTest::gtest_main)

if(MSVC)
    target_compile_options(RoutingTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(RoutingTests)

# TSF activation/onboarding diagnostics contract tests
add_executable(TsfDiagnosticsTests
    tests/tsf_diagnostics_test.cpp
    src/engine/tsf_registration.cpp
)

target_include_directories(TsfDiagnosticsTests PRIVATE src)
target_link_libraries(TsfDiagnosticsTests PRIVATE
    GTest::gtest_main
    advapi32
    ole32
    oleaut32
    uuid
)

if(MSVC)
    target_compile_options(TsfDiagnosticsTests PRIVATE /W4 /WX- /utf-8)
endif()

gtest_discover_tests(TsfDiagnosticsTests)
