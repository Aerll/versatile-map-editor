#pragma once

#include <ddnet/debug/error_codes.hpp>

#include <QString>

namespace ddnet::debug {

struct Error {
    QString message;
    ErrorCode code;
};

} // ddnet::debug::
