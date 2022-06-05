#pragma once

#include <vt/io/ddldata.hpp>
#include <vt/debug/error.hpp>

#include <vector>

class QFileInfo;

namespace vt::io {

class DDLFileStream {
public:
    debug::ErrorCode loadFile(const QFileInfo& file_info);
    debug::ErrorCode saveFile(const QFileInfo& file_info);

public:
    DDLData data;
    std::vector<debug::Error> errors;
};

}
