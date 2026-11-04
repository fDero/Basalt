//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <exception>
#include <functional>
#include <string>

#include <gtest/gtest.h>

#include "errors/compilation_error.hpp"


inline ::testing::AssertionResult raises_compilation_error(
    const std::function<void()>& code,
    CompilationError::Kind expected_kind,
    const std::string& expected_message_fragment
) {
    try {
        code();
    }
    catch (const CompilationError& error) {
        if (error.kind != expected_kind) {
            return ::testing::AssertionFailure()
                << "a CompilationError of a different kind (" << static_cast<int>(error.kind)
                << " instead of " << static_cast<int>(expected_kind) << ") was raised: " << error.what();
        }
        if (std::string(error.what()).find(expected_message_fragment) == std::string::npos) {
            return ::testing::AssertionFailure()
                << "the error message \"" << error.what() << "\" "
                << "does not contain \"" << expected_message_fragment << "\"";
        }
        return ::testing::AssertionSuccess();
    }
    catch (const std::exception& error) {
        return ::testing::AssertionFailure()
            << "an exception that is not a CompilationError was raised: " << error.what();
    }
    return ::testing::AssertionFailure() << "no error was raised";
}

#define EXPECT_COMPILATION_ERROR(kind, expected_message_fragment, ...)         \
    EXPECT_TRUE(raises_compilation_error(                                      \
        [&]() { __VA_ARGS__; },                                                \
        CompilationError::Kind::kind,                                          \
        expected_message_fragment                                              \
    ))
