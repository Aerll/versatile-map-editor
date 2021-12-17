#pragma once

#include <ddnet/debug/error_codes.hpp>

#include <QString>

namespace ddnet::debug {

struct Error {
    QString message;
    ErrorCode code;
};

}



namespace ddnet::internal {

inline debug::Error error = {};

}



namespace ddnet::debug {

[[nodiscard]] inline const Error& getLastError() noexcept {
    return internal::error;
}

}
