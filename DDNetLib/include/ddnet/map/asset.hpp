#pragma once

#include <QFileInfo>

namespace ddnet::map {

class Asset {
    QFileInfo file_info;
};

class ImageAsset : public Asset {
    // TODO
};

class SoundAsset : public Asset {
    // TODO
};

} // ddnet::map::
