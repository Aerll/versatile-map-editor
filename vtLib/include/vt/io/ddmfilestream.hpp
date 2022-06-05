#pragma once

#include <vt/io/ddmdata.hpp>
#include <vt/debug/error_code.hpp>

class QFileInfo;

namespace vt::io {

class DDMFileStream {
public:
    debug::ErrorCode loadFile(const QFileInfo& file_info);
    debug::ErrorCode saveFile(const QFileInfo& file_info);

public:
    DDMData data;
};

}
