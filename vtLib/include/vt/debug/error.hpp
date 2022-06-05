#pragma once

#include <vt/debug/error_code.hpp>

#include <QString>

namespace vt::debug {

struct Error {
    QString message;
    ErrorCode code;
};

}



namespace vt::internal {

inline debug::Error error = {};

}



namespace vt::debug {

[[nodiscard]] inline const Error& getLastError() noexcept {
    return internal::error;
}

}
