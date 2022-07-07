#pragma once

#include <QtGlobal>

namespace vt::constants {

constexpr qint32 _notfound = -1;
constexpr qint32 _fractional_bits = 10;

constexpr quint16 _chunk_size = 128;
constexpr quint16 _grid_size = 65536 / _chunk_size;

}
