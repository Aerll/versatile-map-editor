#include <vt/util/ddnet/validate.hpp>

namespace vt::util::ddnet {
/*
bool canRotate(const Tile& tile, _T layer_type) noexcept {
    // TODO
    return {};
}

bool canMirror(const Tile& tile, _T layer_type) noexcept {
    // TODO
    return {};
}
*/
bool isGameEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case TILE_EMPTY:
        case TILE_HOOKABLE:
        case TILE_DEATH:
        case TILE_UNHOOKABLE:
        case TILE_LASER_BLOCKER:
        case TILE_HOOKTHROUGH:
        case TILE_HOOKTHROUGH_OLD:
        case TILE_FREEZE:
        case TILE_UNFREEZE:
        case TILE_DEEP_FREEZE:
        case TILE_DEEP_UNFREEZE:
        case TILE_WALLJUMP:
        case TILE_ENDLESS_HOOK_ON:
        case TILE_ENDLESS_HOOK_OFF:
        case TILE_PLAYER_HIT_ON:
        case TILE_PLAYER_HIT_OFF:
        case TILE_SOLO_ON:
        case TILE_SOLO_OFF:
        case TILE_REFILL_JUMPS:
        case TILE_START:
        case TILE_FINISH:
        case TILE_TIME_CHECK_1:
        case TILE_TIME_CHECK_2:
        case TILE_TIME_CHECK_3:
        case TILE_TIME_CHECK_4:
        case TILE_TIME_CHECK_5:
        case TILE_TIME_CHECK_6:
        case TILE_TIME_CHECK_7:
        case TILE_TIME_CHECK_8:
        case TILE_TIME_CHECK_9:
        case TILE_TIME_CHECK_10:
        case TILE_TIME_CHECK_11:
        case TILE_TIME_CHECK_12:
        case TILE_TIME_CHECK_13:
        case TILE_TIME_CHECK_14:
        case TILE_TIME_CHECK_15:
        case TILE_TIME_CHECK_16:
        case TILE_TIME_CHECK_17:
        case TILE_TIME_CHECK_18:
        case TILE_TIME_CHECK_19:
        case TILE_TIME_CHECK_20:
        case TILE_TIME_CHECK_21:
        case TILE_TIME_CHECK_22:
        case TILE_TIME_CHECK_23:
        case TILE_TIME_CHECK_24:
        case TILE_TIME_CHECK_25:
        case TILE_STOPPER_1:
        case TILE_STOPPER_2:
        case TILE_STOPPER_4:
        case TILE_MOVE_ARROW_SLOW:
        case TILE_MOVE_ARROW_FAST:
        case TILE_HOOK_BLOCKER:
        case TILE_HOOK_BLOCKER_HALF:
        case TILE_LASER_OLD:
        case TILE_COLLISION_OFF_OLD:
        case TILE_ENDLESS_HOOK_OLD:
        case TILE_HIT_OFF_OLD:
        case TILE_HOOK_OFF_OLD:
        case TILE_TEAM_UNLOCK:
        case TILE_PLAYER_COLLISION_OFF:
        case TILE_ENDLESS_JUMPS_OFF:
        case TILE_JETPACK_OFF:
        case TILE_PLAYER_HOOK_OFF:
        case TILE_TELE_GUN_ON:
        case TILE_TELE_GUN_OFF:
        case TILE_COLLISION_ON:
        case TILE_ENDLESS_JUMPS_ON:
        case TILE_JETPACK_ON:
        case TILE_PLAYER_HOOK_ON:
        case TILE_TELE_GRENADE_ON:
        case TILE_TELE_GRENADE_OFF:
        case TILE_TELE_LASER_ON:
        case TILE_TELE_LASER_OFF:
        case TILE_LIVE_FREEZE:
        case TILE_LIVE_UNFREEZE:
        case TILE_ENTITIES_OFF_L:
        case TILE_ENTITIES_OFF_R:
        case TILE_SPAWN_ALL:
        case TILE_SPAWN_RED:
        case TILE_SPAWN_BLUE:
        case TILE_FLAG_RED:
        case TILE_FLAG_BLUE:
        case TILE_ARMOR:
        case TILE_HEART:
        case TILE_WEAPON_SHOTGUN:
        case TILE_WEAPON_GRENADE:
        case TILE_WEAPON_NINJA:
        case TILE_WEAPON_LASER:
        case TILE_LASER_FAST_CCW:
        case TILE_LASER_MEDIUM_CCW:
        case TILE_LASER_SLOW_CCW:
        case TILE_LASER_STATIC:
        case TILE_LASER_SLOW_CW:
        case TILE_LASER_MEDIUM_CW:
        case TILE_LASER_FAST_CW:
        case TILE_LASER_3:
        case TILE_LASER_6:
        case TILE_LASER_9:
        case TILE_LASER_EXTEND_1:
        case TILE_LASER_EXTEND_2:
        case TILE_LASER_EXTEND_3:
        case TILE_LASER_SHORTEN_2:
        case TILE_LASER_SHORTEN_1:
        case TILE_LASER_SHORTEN_3:
        case TILE_PLASMA_EXPLODE:
        case TILE_PLASMA_FREEZE:
        case TILE_PLASMA_FREEZE_EXPLODE:
        case TILE_PLASMA_UNFREEZE:
        case TILE_BULLET_FREEZE_EXPLODE:
        case TILE_BULLET_FREEZE:
        case TILE_PULLER_WEAK:
        case TILE_PULLER_MEDIUM:
        case TILE_PULLER_STRONG:
        case TILE_PULLER_WEAK_WALL:
        case TILE_PULLER_MEDIUM_WALL:
        case TILE_PULLER_STRONG_WALL:
        case TILE_DOOR:
            return true;
    }
    return false;
}

bool isFrontEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case TILE_EMPTY:
        case TILE_DEATH:
        case TILE_LASER_BLOCKER:
        case TILE_HOOKTHROUGH:
        case TILE_HOOKTHROUGH_OLD:
        case TILE_FREEZE:
        case TILE_UNFREEZE:
        case TILE_DEEP_FREEZE:
        case TILE_DEEP_UNFREEZE:
        case TILE_WALLJUMP:
        case TILE_ENDLESS_HOOK_ON:
        case TILE_ENDLESS_HOOK_OFF:
        case TILE_PLAYER_HIT_ON:
        case TILE_PLAYER_HIT_OFF:
        case TILE_SOLO_ON:
        case TILE_SOLO_OFF:
        case TILE_REFILL_JUMPS:
        case TILE_START:
        case TILE_FINISH:
        case TILE_TIME_CHECK_1:
        case TILE_TIME_CHECK_2:
        case TILE_TIME_CHECK_3:
        case TILE_TIME_CHECK_4:
        case TILE_TIME_CHECK_5:
        case TILE_TIME_CHECK_6:
        case TILE_TIME_CHECK_7:
        case TILE_TIME_CHECK_8:
        case TILE_TIME_CHECK_9:
        case TILE_TIME_CHECK_10:
        case TILE_TIME_CHECK_11:
        case TILE_TIME_CHECK_12:
        case TILE_TIME_CHECK_13:
        case TILE_TIME_CHECK_14:
        case TILE_TIME_CHECK_15:
        case TILE_TIME_CHECK_16:
        case TILE_TIME_CHECK_17:
        case TILE_TIME_CHECK_18:
        case TILE_TIME_CHECK_19:
        case TILE_TIME_CHECK_20:
        case TILE_TIME_CHECK_21:
        case TILE_TIME_CHECK_22:
        case TILE_TIME_CHECK_23:
        case TILE_TIME_CHECK_24:
        case TILE_TIME_CHECK_25:
        case TILE_STOPPER_1:
        case TILE_STOPPER_2:
        case TILE_STOPPER_4:
        case TILE_MOVE_ARROW_SLOW:
        case TILE_MOVE_ARROW_FAST:
        case TILE_HOOK_BLOCKER:
        case TILE_HOOK_BLOCKER_HALF:
        case TILE_LASER_OLD:
        case TILE_COLLISION_OFF_OLD:
        case TILE_ENDLESS_HOOK_OLD:
        case TILE_HIT_OFF_OLD:
        case TILE_HOOK_OFF_OLD:
        case TILE_TEAM_UNLOCK:
        case TILE_PLAYER_COLLISION_OFF:
        case TILE_ENDLESS_JUMPS_OFF:
        case TILE_JETPACK_OFF:
        case TILE_PLAYER_HOOK_OFF:
        case TILE_TELE_GUN_ON:
        case TILE_TELE_GUN_OFF:
        case TILE_TELE_WEAPON_YELLOW:
        case TILE_TELE_WEAPON_BLUE:
        case TILE_COLLISION_ON:
        case TILE_ENDLESS_JUMPS_ON:
        case TILE_JETPACK_ON:
        case TILE_PLAYER_HOOK_ON:
        case TILE_TELE_GRENADE_ON:
        case TILE_TELE_GRENADE_OFF:
        case TILE_TELE_LASER_ON:
        case TILE_TELE_LASER_OFF:
        case TILE_LIVE_FREEZE:
        case TILE_LIVE_UNFREEZE:
        case TILE_ENTITIES_OFF_L:
        case TILE_ENTITIES_OFF_R:
        case TILE_SPAWN_ALL:
        case TILE_SPAWN_RED:
        case TILE_SPAWN_BLUE:
        case TILE_FLAG_RED:
        case TILE_FLAG_BLUE:
        case TILE_ARMOR:
        case TILE_HEART:
        case TILE_WEAPON_SHOTGUN:
        case TILE_WEAPON_GRENADE:
        case TILE_WEAPON_NINJA:
        case TILE_WEAPON_LASER:
        case TILE_LASER_FAST_CCW:
        case TILE_LASER_MEDIUM_CCW:
        case TILE_LASER_SLOW_CCW:
        case TILE_LASER_STATIC:
        case TILE_LASER_SLOW_CW:
        case TILE_LASER_MEDIUM_CW:
        case TILE_LASER_FAST_CW:
        case TILE_LASER_3:
        case TILE_LASER_6:
        case TILE_LASER_9:
        case TILE_LASER_EXTEND_1:
        case TILE_LASER_EXTEND_2:
        case TILE_LASER_EXTEND_3:
        case TILE_LASER_SHORTEN_1:
        case TILE_LASER_SHORTEN_2:
        case TILE_LASER_SHORTEN_3:
        case TILE_PLASMA_EXPLODE:
        case TILE_PLASMA_FREEZE:
        case TILE_PLASMA_FREEZE_EXPLODE:
        case TILE_PLASMA_UNFREEZE:
        case TILE_BULLET_FREEZE_EXPLODE:
        case TILE_BULLET_FREEZE:
        case TILE_PULLER_WEAK:
        case TILE_PULLER_MEDIUM:
        case TILE_PULLER_STRONG:
        case TILE_PULLER_WEAK_WALL:
        case TILE_PULLER_MEDIUM_WALL:
        case TILE_DOOR:
            return true;
    }
    return false;
}

bool isTeleEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case TILE_TELE_RED_IN:
        case TILE_TELE_WEAPON:
        case TILE_TELE_HOOK:
        case TILE_TELE_BLUE_IN:
        case TILE_TELE_OUT:
        case TILE_CHECKPOINT:
        case TILE_CP_TELE_OUT:
        case TILE_CP_TELE_BLUE_IN:
        case TILE_CP_TELE_RED_IN:
            return true;
    }
    return false;
}

bool isTuneEntity(const Tile& tile) noexcept {
    return tile.index == TILE_TUNE;
}

bool isSwitchEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case TILE_EMPTY:
        case TILE_JUMP_AMOUNT:
        case TILE_FREEZE:
        case TILE_DEEP_FREEZE:
        case TILE_DEEP_UNFREEZE:
        case TILE_PLAYER_HIT_ON:
        case TILE_PLAYER_HIT_OFF:
        case TILE_SWITCH_TIMED_ON:
        case TILE_SWITCH_TIMED_OFF:
        case TILE_SWITCH_ON:
        case TILE_SWITCH_OFF:
        case TILE_TIME_PENALTY:
        case TILE_TIME_BONUS:
        case TILE_TELE_WEAPON_YELLOW:
        case TILE_TELE_WEAPON_BLUE:
        case TILE_LIVE_FREEZE:
        case TILE_LIVE_UNFREEZE:
        case TILE_ARMOR:
        case TILE_HEART:
        case TILE_WEAPON_SHOTGUN:
        case TILE_WEAPON_GRENADE:
        case TILE_WEAPON_NINJA:
        case TILE_WEAPON_LASER:
        case TILE_LASER_FAST_CCW:
        case TILE_LASER_MEDIUM_CCW:
        case TILE_LASER_SLOW_CCW:
        case TILE_LASER_STATIC:
        case TILE_LASER_SLOW_CW:
        case TILE_LASER_MEDIUM_CW:
        case TILE_LASER_FAST_CW:
        case TILE_LASER_3:
        case TILE_LASER_6:
        case TILE_LASER_9:
        case TILE_LASER_EXTEND_1:
        case TILE_LASER_EXTEND_2:
        case TILE_LASER_EXTEND_3:
        case TILE_LASER_SHORTEN_1:
        case TILE_LASER_SHORTEN_2:
        case TILE_LASER_SHORTEN_3:
        case TILE_PLASMA_EXPLODE:
        case TILE_PLASMA_FREEZE:
        case TILE_PLASMA_FREEZE_EXPLODE:
        case TILE_PLASMA_UNFREEZE:
        case TILE_BULLET_FREEZE_EXPLODE:
        case TILE_BULLET_FREEZE:
        case TILE_PULLER_WEAK:
        case TILE_PULLER_MEDIUM:
        case TILE_PULLER_STRONG:
        case TILE_PULLER_WEAK_WALL:
        case TILE_PULLER_MEDIUM_WALL:
        case TILE_PULLER_STRONG_WALL:
        case TILE_DOOR:
            return true;
    }
    return false;
}

bool isSpeedupEntity(const Tile& tile) noexcept {
    return tile.index == TILE_SPEEDUP;
}

bool isServerSetting(const QString& string) noexcept {
    // TODO
    return false;
}

}
