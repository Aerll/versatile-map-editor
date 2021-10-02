#pragma once

#include <ddnet/io/ddldata.hpp>
#include <ddnet/debug/error.hpp>

#include <vector>

class QFileInfo;

namespace ddnet::io {

class DDLFileStream {
public:
    debug::ErrorCode loadFile(const QFileInfo& file_info);
    debug::ErrorCode saveFile(const QFileInfo& file_info);

public:
    DDLData data;
    std::vector<debug::Error> errors;
};

}
