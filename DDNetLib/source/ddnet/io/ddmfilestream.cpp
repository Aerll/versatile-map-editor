#include <ddnet/io/ddmfilestream.hpp>

#include <QFile>
#include <QFileInfo>

namespace ddnet::io {

debug::ErrorCode DDMFileStream::loadFile(const QFileInfo& file_info) {
    QFile file{ file_info.absoluteFilePath() };

    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToRead;

    QDataStream map_data_stream{ &file };
    if (map_data_stream.device()->size() == 0) [[unlikely]]
        return debug::ErrorCode::File_NoData;

    // TODO
    return debug::ErrorCode::NoError;
}

debug::ErrorCode DDMFileStream::saveFile(const QFileInfo& file_info) {
    QFile file{ file_info.absoluteFilePath() };

    file.open(QIODevice::WriteOnly);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToWrite;

    // TODO
    return debug::ErrorCode::NoError;
}

} // ddnet::io::
