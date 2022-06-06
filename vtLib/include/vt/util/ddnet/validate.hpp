#pragma once

#include <vt/map/tile.hpp>

#include <QString>

namespace vt::util::ddnet {

// TODO: bool canRotate(const Tile& tile, _T layer_type) noexcept;
// TODO: bool canMirror(const Tile& tile, _T layer_type) noexcept;
bool isGameEntity(const Tile& tile) noexcept;
bool isFrontEntity(const Tile& tile) noexcept;
bool isTeleEntity(const Tile& tile) noexcept;
bool isTuneEntity(const Tile& tile) noexcept;
bool isSwitchEntity(const Tile& tile) noexcept;
bool isSpeedupEntity(const Tile& tile) noexcept;
bool isServerSetting(const QString& string) noexcept;

enum GameLayerTiles : quint8 {
	TILE_EMPTY                 = 0,
	TILE_HOOKABLE              = 1,
	TILE_DEATH                 = 2,
	TILE_UNHOOKABLE            = 3,
	TILE_LASER_BLOCKER         = 4,
	TILE_HOOKTHROUGH           = 5,
	TILE_HOOKTHROUGH_OLD       = 6,
	TILE_FREEZE                = 9,
	TILE_UNFREEZE              = 11,
	TILE_DEEP_FREEZE           = 12,
	TILE_DEEP_UNFREEZE         = 13,
	TILE_WALLJUMP              = 16,
	TILE_ENDLESS_HOOK_ON       = 17,
	TILE_ENDLESS_HOOK_OFF      = 18,
	TILE_PLAYER_HIT_ON         = 19,
	TILE_PLAYER_HIT_OFF        = 20,
	TILE_SOLO_ON               = 21,
	TILE_SOLO_OFF              = 22,
	TILE_REFILL_JUMPS          = 32,
	TILE_START                 = 33,
	TILE_FINISH                = 34,
	TILE_TIME_CHECK_1          = 35,
	TILE_TIME_CHECK_2          = 36,
	TILE_TIME_CHECK_3          = 37,
	TILE_TIME_CHECK_4          = 38,
	TILE_TIME_CHECK_5          = 39,
	TILE_TIME_CHECK_6          = 40,
	TILE_TIME_CHECK_7          = 41,
	TILE_TIME_CHECK_8          = 42,
	TILE_TIME_CHECK_9          = 43,
	TILE_TIME_CHECK_10         = 44,
	TILE_TIME_CHECK_11         = 45,
	TILE_TIME_CHECK_12         = 46,
	TILE_TIME_CHECK_13         = 47,
	TILE_TIME_CHECK_14         = 48,
	TILE_TIME_CHECK_15         = 49,
	TILE_TIME_CHECK_16         = 50,
	TILE_TIME_CHECK_17         = 51,
	TILE_TIME_CHECK_18         = 52,
	TILE_TIME_CHECK_19         = 53,
	TILE_TIME_CHECK_20         = 54,
	TILE_TIME_CHECK_21         = 55,
	TILE_TIME_CHECK_22         = 56,
	TILE_TIME_CHECK_23         = 57,
	TILE_TIME_CHECK_24         = 58,
	TILE_TIME_CHECK_25         = 59,
	TILE_STOPPER_1             = 60,
	TILE_STOPPER_2             = 61,
	TILE_STOPPER_4             = 62,
	TILE_MOVE_ARROW_SLOW       = 64,
	TILE_MOVE_ARROW_FAST       = 65,
	TILE_HOOK_BLOCKER          = 66,
	TILE_HOOK_BLOCKER_HALF     = 67,
	TILE_LASER_OLD             = 71,
	TILE_COLLISION_OFF_OLD     = 72,
	TILE_ENDLESS_HOOK_OLD      = 73,
	TILE_HIT_OFF_OLD           = 74,
	TILE_HOOK_OFF_OLD          = 75,
	TILE_TEAM_UNLOCK           = 76,
	TILE_PLAYER_COLLISION_OFF  = 88,
	TILE_ENDLESS_JUMPS_OFF     = 89,
	TILE_JETPACK_OFF           = 90,
	TILE_PLAYER_HOOK_OFF       = 91,
	TILE_TELE_GUN_ON           = 96,
	TILE_TELE_GUN_OFF          = 97,
	TILE_COLLISION_ON          = 104,
	TILE_ENDLESS_JUMPS_ON      = 105,
	TILE_JETPACK_ON            = 106,
	TILE_PLAYER_HOOK_ON        = 107,
	TILE_TELE_GRENADE_ON       = 112,
	TILE_TELE_GRENADE_OFF      = 113,
	TILE_TELE_LASER_ON         = 128,
	TILE_TELE_LASER_OFF        = 129,
	TILE_LIVE_FREEZE           = 144,
	TILE_LIVE_UNFREEZE         = 145,
	TILE_ENTITIES_OFF_L        = 190,
	TILE_ENTITIES_OFF_R        = 191,
	TILE_SPAWN_ALL             = 192,
	TILE_SPAWN_RED             = 193,
	TILE_SPAWN_BLUE            = 194,
	TILE_FLAG_RED              = 195,
	TILE_FLAG_BLUE             = 196,
	TILE_ARMOR                 = 197,
	TILE_HEART                 = 198,
	TILE_WEAPON_SHOTGUN        = 199,
	TILE_WEAPON_GRENADE        = 200,
	TILE_WEAPON_NINJA          = 201,
	TILE_WEAPON_LASER          = 202,
	TILE_LASER_FAST_CCW        = 203,
	TILE_LASER_MEDIUM_CCW      = 204,
	TILE_LASER_SLOW_CCW        = 205,
	TILE_LASER_STATIC          = 206,
	TILE_LASER_SLOW_CW         = 207,
	TILE_LASER_MEDIUM_CW       = 208,
	TILE_LASER_FAST_CW         = 209,
	TILE_LASER_3               = 210,
	TILE_LASER_6               = 211,
	TILE_LASER_9               = 212,
	TILE_LASER_EXTEND_1        = 213,
	TILE_LASER_EXTEND_2        = 214,
	TILE_LASER_EXTEND_3        = 215,
	TILE_LASER_SHORTEN_1       = 216,
	TILE_LASER_SHORTEN_2       = 217,
	TILE_LASER_SHORTEN_3       = 218,
	TILE_PLASMA_EXPLODE        = 220,
	TILE_PLASMA_FREEZE         = 221,
	TILE_PLASMA_FREEZE_EXPLODE = 222,
	TILE_PLASMA_UNFREEZE       = 223,
	TILE_BULLET_FREEZE_EXPLODE = 224,
	TILE_BULLET_FREEZE         = 225,
	TILE_PULLER_WEAK           = 233,
	TILE_PULLER_MEDIUM         = 234,
	TILE_PULLER_STRONG         = 235,
	TILE_PULLER_WEAK_WALL      = 236,
	TILE_PULLER_MEDIUM_WALL    = 237,
	TILE_PULLER_STRONG_WALL    = 238,
	TILE_DOOR                  = 240
};

enum FrontLayerTiles : quint8 {
	TILE_EMPTY                 = 0,
	TILE_DEATH                 = 2,
	TILE_LASER_BLOCKER         = 4,
	TILE_HOOKTHROUGH           = 5,
	TILE_HOOKTHROUGH_OLD       = 6,
	TILE_FREEZE                = 9,
	TILE_UNFREEZE              = 11,
	TILE_DEEP_FREEZE           = 12,
	TILE_DEEP_UNFREEZE         = 13,
	TILE_WALLJUMP              = 16,
	TILE_ENDLESS_HOOK_ON       = 17,
	TILE_ENDLESS_HOOK_OFF      = 18,
	TILE_PLAYER_HIT_ON         = 19,
	TILE_PLAYER_HIT_OFF        = 20,
	TILE_SOLO_ON               = 21,
	TILE_SOLO_OFF              = 22,
	TILE_REFILL_JUMPS          = 32,
	TILE_START                 = 33,
	TILE_FINISH                = 34,
	TILE_TIME_CHECK_1          = 35,
	TILE_TIME_CHECK_2          = 36,
	TILE_TIME_CHECK_3          = 37,
	TILE_TIME_CHECK_4          = 38,
	TILE_TIME_CHECK_5          = 39,
	TILE_TIME_CHECK_6          = 40,
	TILE_TIME_CHECK_7          = 41,
	TILE_TIME_CHECK_8          = 42,
	TILE_TIME_CHECK_9          = 43,
	TILE_TIME_CHECK_10         = 44,
	TILE_TIME_CHECK_11         = 45,
	TILE_TIME_CHECK_12         = 46,
	TILE_TIME_CHECK_13         = 47,
	TILE_TIME_CHECK_14         = 48,
	TILE_TIME_CHECK_15         = 49,
	TILE_TIME_CHECK_16         = 50,
	TILE_TIME_CHECK_17         = 51,
	TILE_TIME_CHECK_18         = 52,
	TILE_TIME_CHECK_19         = 53,
	TILE_TIME_CHECK_20         = 54,
	TILE_TIME_CHECK_21         = 55,
	TILE_TIME_CHECK_22         = 56,
	TILE_TIME_CHECK_23         = 57,
	TILE_TIME_CHECK_24         = 58,
	TILE_TIME_CHECK_25         = 59,
	TILE_STOPPER_1             = 60,
	TILE_STOPPER_2             = 61,
	TILE_STOPPER_4             = 62,
	TILE_MOVE_ARROW_SLOW       = 64,
	TILE_MOVE_ARROW_FAST       = 65,
	TILE_HOOK_BLOCKER          = 66,
	TILE_HOOK_BLOCKER_HALF     = 67,
	TILE_LASER_OLD             = 71,
	TILE_COLLISION_OFF_OLD     = 72,
	TILE_ENDLESS_HOOK_OLD      = 73,
	TILE_HIT_OFF_OLD           = 74,
	TILE_HOOK_OFF_OLD          = 75,
	TILE_TEAM_UNLOCK           = 76,
	TILE_PLAYER_COLLISION_OFF  = 88,
	TILE_ENDLESS_JUMPS_OFF     = 89,
	TILE_JETPACK_OFF           = 90,
	TILE_PLAYER_HOOK_OFF       = 91,
	TILE_TELE_GUN_ON           = 96,
	TILE_TELE_GUN_OFF          = 97,
	TILE_TELE_WEAPON_YELLOW    = 98,
	TILE_TELE_WEAPON_BLUE      = 99,
	TILE_COLLISION_ON          = 104,
	TILE_ENDLESS_JUMPS_ON      = 105,
	TILE_JETPACK_ON            = 106,
	TILE_PLAYER_HOOK_ON        = 107,
	TILE_TELE_GRENADE_ON       = 112,
	TILE_TELE_GRENADE_OFF      = 113,
	TILE_TELE_LASER_ON         = 128,
	TILE_TELE_LASER_OFF        = 129,
	TILE_LIVE_FREEZE           = 144,
	TILE_LIVE_UNFREEZE         = 145,
	TILE_ENTITIES_OFF_L        = 190,
	TILE_ENTITIES_OFF_R        = 191,
	TILE_SPAWN_ALL             = 192,
	TILE_SPAWN_RED             = 193,
	TILE_SPAWN_BLUE            = 194,
	TILE_FLAG_RED              = 195,
	TILE_FLAG_BLUE             = 196,
	TILE_ARMOR                 = 197,
	TILE_HEART                 = 198,
	TILE_WEAPON_SHOTGUN        = 199,
	TILE_WEAPON_GRENADE        = 200,
	TILE_WEAPON_NINJA          = 201,
	TILE_WEAPON_LASER          = 202,
	TILE_LASER_FAST_CCW        = 203,
	TILE_LASER_MEDIUM_CCW      = 204,
	TILE_LASER_SLOW_CCW        = 205,
	TILE_LASER_STATIC          = 206,
	TILE_LASER_SLOW_CW         = 207,
	TILE_LASER_MEDIUM_CW       = 208,
	TILE_LASER_FAST_CW         = 209,
	TILE_LASER_3               = 210,
	TILE_LASER_6               = 211,
	TILE_LASER_9               = 212,
	TILE_LASER_EXTEND_1        = 213,
	TILE_LASER_EXTEND_2        = 214,
	TILE_LASER_EXTEND_3        = 215,
	TILE_LASER_SHORTEN_1       = 216,
	TILE_LASER_SHORTEN_2       = 217,
	TILE_LASER_SHORTEN_3       = 218,
	TILE_PLASMA_EXPLODE        = 220,
	TILE_PLASMA_FREEZE         = 221,
	TILE_PLASMA_FREEZE_EXPLODE = 222,
	TILE_PLASMA_UNFREEZE       = 223,
	TILE_BULLET_FREEZE_EXPLODE = 224,
	TILE_BULLET_FREEZE         = 225,
	TILE_PULLER_WEAK           = 233,
	TILE_PULLER_MEDIUM         = 234,
	TILE_PULLER_STRONG         = 235,
	TILE_PULLER_WEAK_WALL      = 236,
	TILE_PULLER_MEDIUM_WALL    = 237,
	TILE_PULLER_STRONG_WALL    = 238,
	TILE_DOOR                  = 240
};

enum TeleLayerTiles : quint8 {
	TILE_TELE_RED_IN     = 10,
	TILE_TELE_WEAPON     = 14,
	TILE_TELE_HOOK       = 15,
	TILE_TELE_BLUE_IN    = 26,
	TILE_TELE_OUT        = 27,
	TILE_CHECKPOINT      = 29,
	TILE_CP_TELE_OUT     = 30,
	TILE_CP_TELE_BLUE_IN = 31,
	TILE_CP_TELE_RED_IN  = 63
};

enum TuneLayerTiles : quint8 {
	TILE_TUNE = 68
};

enum SwitchLayerTiles : quint8 {
	TILE_EMPTY = 0,
	TILE_JUMP_AMOUNT = 7,
	TILE_FREEZE = 9,
	TILE_DEEP_FREEZE = 12,
	TILE_DEEP_UNFREEZE = 13,
	TILE_PLAYER_HIT_ON = 19,
	TILE_PLAYER_HIT_OFF = 20,
	TILE_SWITCH_TIMED_ON = 22,
	TILE_SWITCH_TIMED_OFF = 23,
	TILE_SWITCH_ON = 24,
	TILE_SWITCH_OFF = 25,
	TILE_TIME_PENALTY = 79,
	TILE_TIME_BONUS = 95,
	TILE_TELE_WEAPON_YELLOW = 98,
	TILE_TELE_WEAPON_BLUE = 99,
	TILE_LIVE_FREEZE = 144,
	TILE_LIVE_UNFREEZE = 145,
	TILE_ARMOR = 197,
	TILE_HEART = 198,
	TILE_WEAPON_SHOTGUN = 199,
	TILE_WEAPON_GRENADE = 200,
	TILE_WEAPON_NINJA = 201,
	TILE_WEAPON_LASER = 202,
	TILE_LASER_FAST_CCW = 203,
	TILE_LASER_MEDIUM_CCW = 204,
	TILE_LASER_SLOW_CCW = 205,
	TILE_LASER_STATIC = 206,
	TILE_LASER_SLOW_CW = 207,
	TILE_LASER_MEDIUM_CW = 208,
	TILE_LASER_FAST_CW = 209,
	TILE_LASER_3 = 210,
	TILE_LASER_6 = 211,
	TILE_LASER_9 = 212,
	TILE_LASER_EXTEND_1 = 213,
	TILE_LASER_EXTEND_2 = 214,
	TILE_LASER_EXTEND_3 = 215,
	TILE_LASER_SHORTEN_1 = 216,
	TILE_LASER_SHORTEN_2 = 217,
	TILE_LASER_SHORTEN_3 = 218,
	TILE_PLASMA_EXPLODE = 220,
	TILE_PLASMA_FREEZE = 221,
	TILE_PLASMA_FREEZE_EXPLODE = 222,
	TILE_PLASMA_UNFREEZE = 223,
	TILE_BULLET_FREEZE_EXPLODE = 224,
	TILE_BULLET_FREEZE = 225,
	TILE_PULLER_WEAK = 233,
	TILE_PULLER_MEDIUM = 234,
	TILE_PULLER_STRONG = 235,
	TILE_PULLER_WEAK_WALL = 236,
	TILE_PULLER_MEDIUM_WALL = 237,
	TILE_PULLER_STRONG_WALL = 238,
	TILE_DOOR = 240
};

enum SpeedupLayerTiles : quint8 {
	TILE_SPEEDUP = 28
};

}