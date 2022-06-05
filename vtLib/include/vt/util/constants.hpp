#pragma once

#include <QtGlobal>

namespace vt::constants {

constexpr qint32 _notfound = -1;
constexpr qint32 _fractional_bits = 10;

constexpr qint32 _map_id = 0x41544144u; // DATA
constexpr qint32 _map_version = 4;
constexpr qint32 _map_item_version = 1;
constexpr qint32 _map_item_envelope_version = 2;
constexpr qint32 _map_item_group_version = 3;
constexpr qint32 _map_item_layer_tile_version = 3;
constexpr qint32 _map_item_layer_quad_version = 2;
constexpr qint32 _map_item_layer_sound_version = 2;

constexpr qint32 _map_max_author_length = 32;
constexpr qint32 _map_max_map_version_length = 16;
constexpr qint32 _map_max_credits_length = 128;
constexpr qint32 _map_max_license_length = 32;
constexpr qint32 _map_max_command_length = 256;
constexpr qint32 _map_max_item_envelope_name_length = 8;
constexpr qint32 _map_max_item_group_name_length = 3;
constexpr qint32 _map_max_item_layer_name_length = 3;
constexpr qint32 _map_max_envelope_name_length = _map_max_item_envelope_name_length * sizeof(qint32);
constexpr qint32 _map_max_group_name_length = _map_max_item_group_name_length * sizeof(qint32);
constexpr qint32 _map_max_layer_name_length = _map_max_item_layer_name_length * sizeof(qint32);

}
