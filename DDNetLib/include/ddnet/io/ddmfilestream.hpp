#pragma once

#include <ddnet/io/ddmdata.hpp>
#include <ddnet/debug/error_codes.hpp>

class QFileInfo;

namespace ddnet::io {

class DDMFileStream {
public:
    debug::ErrorCode loadFile(const QFileInfo& file_info);
    debug::ErrorCode saveFile(const QFileInfo& file_info);

public:
    DDMData data;
};

}
