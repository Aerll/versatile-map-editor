#include <vt/util/ddnet/validate.hpp>

#include <vt/map/tile.hpp>
#include <vt/util/ddnet/enums.hpp>

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
        case enums::ddnet::TILE_GAME_EMPTY:
        case enums::ddnet::TILE_GAME_HOOKABLE:
        case enums::ddnet::TILE_GAME_DEATH:
        case enums::ddnet::TILE_GAME_UNHOOKABLE:
        case enums::ddnet::TILE_GAME_LASER_BLOCKER:
        case enums::ddnet::TILE_GAME_HOOKTHROUGH:
        case enums::ddnet::TILE_GAME_HOOKTHROUGH_OLD:
        case enums::ddnet::TILE_GAME_FREEZE:
        case enums::ddnet::TILE_GAME_UNFREEZE:
        case enums::ddnet::TILE_GAME_DEEP_FREEZE:
        case enums::ddnet::TILE_GAME_DEEP_UNFREEZE:
        case enums::ddnet::TILE_GAME_WALLJUMP:
        case enums::ddnet::TILE_GAME_ENDLESS_HOOK_ON:
        case enums::ddnet::TILE_GAME_ENDLESS_HOOK_OFF:
        case enums::ddnet::TILE_GAME_PLAYER_HIT_ON:
        case enums::ddnet::TILE_GAME_PLAYER_HIT_OFF:
        case enums::ddnet::TILE_GAME_SOLO_ON:
        case enums::ddnet::TILE_GAME_SOLO_OFF:
        case enums::ddnet::TILE_GAME_REFILL_JUMPS:
        case enums::ddnet::TILE_GAME_START:
        case enums::ddnet::TILE_GAME_FINISH:
        case enums::ddnet::TILE_GAME_TIME_CP1:
        case enums::ddnet::TILE_GAME_TIME_CP2:
        case enums::ddnet::TILE_GAME_TIME_CP3:
        case enums::ddnet::TILE_GAME_TIME_CP4:
        case enums::ddnet::TILE_GAME_TIME_CP5:
        case enums::ddnet::TILE_GAME_TIME_CP6:
        case enums::ddnet::TILE_GAME_TIME_CP7:
        case enums::ddnet::TILE_GAME_TIME_CP8:
        case enums::ddnet::TILE_GAME_TIME_CP9:
        case enums::ddnet::TILE_GAME_TIME_CP10:
        case enums::ddnet::TILE_GAME_TIME_CP11:
        case enums::ddnet::TILE_GAME_TIME_CP12:
        case enums::ddnet::TILE_GAME_TIME_CP13:
        case enums::ddnet::TILE_GAME_TIME_CP14:
        case enums::ddnet::TILE_GAME_TIME_CP15:
        case enums::ddnet::TILE_GAME_TIME_CP16:
        case enums::ddnet::TILE_GAME_TIME_CP17:
        case enums::ddnet::TILE_GAME_TIME_CP18:
        case enums::ddnet::TILE_GAME_TIME_CP19:
        case enums::ddnet::TILE_GAME_TIME_CP20:
        case enums::ddnet::TILE_GAME_TIME_CP21:
        case enums::ddnet::TILE_GAME_TIME_CP22:
        case enums::ddnet::TILE_GAME_TIME_CP23:
        case enums::ddnet::TILE_GAME_TIME_CP24:
        case enums::ddnet::TILE_GAME_TIME_CP25:
        case enums::ddnet::TILE_GAME_STOPPER_1:
        case enums::ddnet::TILE_GAME_STOPPER_2:
        case enums::ddnet::TILE_GAME_STOPPER_4:
        case enums::ddnet::TILE_GAME_MOVE_ARROW_SLOW:
        case enums::ddnet::TILE_GAME_MOVE_ARROW_FAST:
        case enums::ddnet::TILE_GAME_HOOK_BLOCKER:
        case enums::ddnet::TILE_GAME_HOOK_BLOCKER_HALF:
        case enums::ddnet::TILE_GAME_LASER_OLD:
        case enums::ddnet::TILE_GAME_COLLISION_OFF_OLD:
        case enums::ddnet::TILE_GAME_ENDLESS_HOOK_OLD:
        case enums::ddnet::TILE_GAME_HIT_OFF_OLD:
        case enums::ddnet::TILE_GAME_HOOK_OFF_OLD:
        case enums::ddnet::TILE_GAME_TEAM_UNLOCK:
        case enums::ddnet::TILE_GAME_PLAYER_COLLISION_OFF:
        case enums::ddnet::TILE_GAME_ENDLESS_JUMPS_OFF:
        case enums::ddnet::TILE_GAME_JETPACK_OFF:
        case enums::ddnet::TILE_GAME_PLAYER_HOOK_OFF:
        case enums::ddnet::TILE_GAME_TELE_GUN_ON:
        case enums::ddnet::TILE_GAME_TELE_GUN_OFF:
        case enums::ddnet::TILE_GAME_COLLISION_ON:
        case enums::ddnet::TILE_GAME_ENDLESS_JUMPS_ON:
        case enums::ddnet::TILE_GAME_JETPACK_ON:
        case enums::ddnet::TILE_GAME_PLAYER_HOOK_ON:
        case enums::ddnet::TILE_GAME_TELE_GRENADE_ON:
        case enums::ddnet::TILE_GAME_TELE_GRENADE_OFF:
        case enums::ddnet::TILE_GAME_TELE_LASER_ON:
        case enums::ddnet::TILE_GAME_TELE_LASER_OFF:
        case enums::ddnet::TILE_GAME_LIVE_FREEZE:
        case enums::ddnet::TILE_GAME_LIVE_UNFREEZE:
        case enums::ddnet::TILE_GAME_ENTITIES_OFF_L:
        case enums::ddnet::TILE_GAME_ENTITIES_OFF_R:
        case enums::ddnet::TILE_GAME_SPAWN_ALL:
        case enums::ddnet::TILE_GAME_SPAWN_RED:
        case enums::ddnet::TILE_GAME_SPAWN_BLUE:
        case enums::ddnet::TILE_GAME_FLAG_RED:
        case enums::ddnet::TILE_GAME_FLAG_BLUE:
        case enums::ddnet::TILE_GAME_ARMOR:
        case enums::ddnet::TILE_GAME_HEART:
        case enums::ddnet::TILE_GAME_WEAPON_SHOTGUN:
        case enums::ddnet::TILE_GAME_WEAPON_GRENADE:
        case enums::ddnet::TILE_GAME_WEAPON_NINJA:
        case enums::ddnet::TILE_GAME_WEAPON_LASER:
        case enums::ddnet::TILE_GAME_LASER_FAST_CCW:
        case enums::ddnet::TILE_GAME_LASER_MEDIUM_CCW:
        case enums::ddnet::TILE_GAME_LASER_SLOW_CCW:
        case enums::ddnet::TILE_GAME_LASER_STATIC:
        case enums::ddnet::TILE_GAME_LASER_SLOW_CW:
        case enums::ddnet::TILE_GAME_LASER_MEDIUM_CW:
        case enums::ddnet::TILE_GAME_LASER_FAST_CW:
        case enums::ddnet::TILE_GAME_LASER_3:
        case enums::ddnet::TILE_GAME_LASER_6:
        case enums::ddnet::TILE_GAME_LASER_9:
        case enums::ddnet::TILE_GAME_LASER_EXTEND_1:
        case enums::ddnet::TILE_GAME_LASER_EXTEND_2:
        case enums::ddnet::TILE_GAME_LASER_EXTEND_3:
        case enums::ddnet::TILE_GAME_LASER_SHORTEN_2:
        case enums::ddnet::TILE_GAME_LASER_SHORTEN_1:
        case enums::ddnet::TILE_GAME_LASER_SHORTEN_3:
        case enums::ddnet::TILE_GAME_PLASMA_EXPLODE:
        case enums::ddnet::TILE_GAME_PLASMA_FREEZE:
        case enums::ddnet::TILE_GAME_PLASMA_FREEZE_EXPLODE:
        case enums::ddnet::TILE_GAME_PLASMA_UNFREEZE:
        case enums::ddnet::TILE_GAME_BULLET_FREEZE_EXPLODE:
        case enums::ddnet::TILE_GAME_BULLET_FREEZE:
        case enums::ddnet::TILE_GAME_PULLER_WEAK:
        case enums::ddnet::TILE_GAME_PULLER_MEDIUM:
        case enums::ddnet::TILE_GAME_PULLER_STRONG:
        case enums::ddnet::TILE_GAME_PULLER_WEAK_WALL:
        case enums::ddnet::TILE_GAME_PULLER_MEDIUM_WALL:
        case enums::ddnet::TILE_GAME_PULLER_STRONG_WALL:
        case enums::ddnet::TILE_GAME_DOOR:
            return true;
    }
    return false;
}

bool isFrontEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case enums::ddnet::TILE_FRONT_EMPTY:
        case enums::ddnet::TILE_FRONT_DEATH:
        case enums::ddnet::TILE_FRONT_LASER_BLOCKER:
        case enums::ddnet::TILE_FRONT_HOOKTHROUGH:
        case enums::ddnet::TILE_FRONT_HOOKTHROUGH_OLD:
        case enums::ddnet::TILE_FRONT_FREEZE:
        case enums::ddnet::TILE_FRONT_UNFREEZE:
        case enums::ddnet::TILE_FRONT_DEEP_FREEZE:
        case enums::ddnet::TILE_FRONT_DEEP_UNFREEZE:
        case enums::ddnet::TILE_FRONT_WALLJUMP:
        case enums::ddnet::TILE_FRONT_ENDLESS_HOOK_ON:
        case enums::ddnet::TILE_FRONT_ENDLESS_HOOK_OFF:
        case enums::ddnet::TILE_FRONT_PLAYER_HIT_ON:
        case enums::ddnet::TILE_FRONT_PLAYER_HIT_OFF:
        case enums::ddnet::TILE_FRONT_SOLO_ON:
        case enums::ddnet::TILE_FRONT_SOLO_OFF:
        case enums::ddnet::TILE_FRONT_REFILL_JUMPS:
        case enums::ddnet::TILE_FRONT_START:
        case enums::ddnet::TILE_FRONT_FINISH:
        case enums::ddnet::TILE_FRONT_TIME_CP1:
        case enums::ddnet::TILE_FRONT_TIME_CP2:
        case enums::ddnet::TILE_FRONT_TIME_CP3:
        case enums::ddnet::TILE_FRONT_TIME_CP4:
        case enums::ddnet::TILE_FRONT_TIME_CP5:
        case enums::ddnet::TILE_FRONT_TIME_CP6:
        case enums::ddnet::TILE_FRONT_TIME_CP7:
        case enums::ddnet::TILE_FRONT_TIME_CP8:
        case enums::ddnet::TILE_FRONT_TIME_CP9:
        case enums::ddnet::TILE_FRONT_TIME_CP10:
        case enums::ddnet::TILE_FRONT_TIME_CP11:
        case enums::ddnet::TILE_FRONT_TIME_CP12:
        case enums::ddnet::TILE_FRONT_TIME_CP13:
        case enums::ddnet::TILE_FRONT_TIME_CP14:
        case enums::ddnet::TILE_FRONT_TIME_CP15:
        case enums::ddnet::TILE_FRONT_TIME_CP16:
        case enums::ddnet::TILE_FRONT_TIME_CP17:
        case enums::ddnet::TILE_FRONT_TIME_CP18:
        case enums::ddnet::TILE_FRONT_TIME_CP19:
        case enums::ddnet::TILE_FRONT_TIME_CP20:
        case enums::ddnet::TILE_FRONT_TIME_CP21:
        case enums::ddnet::TILE_FRONT_TIME_CP22:
        case enums::ddnet::TILE_FRONT_TIME_CP23:
        case enums::ddnet::TILE_FRONT_TIME_CP24:
        case enums::ddnet::TILE_FRONT_TIME_CP25:
        case enums::ddnet::TILE_FRONT_STOPPER_1:
        case enums::ddnet::TILE_FRONT_STOPPER_2:
        case enums::ddnet::TILE_FRONT_STOPPER_4:
        case enums::ddnet::TILE_FRONT_MOVE_ARROW_SLOW:
        case enums::ddnet::TILE_FRONT_MOVE_ARROW_FAST:
        case enums::ddnet::TILE_FRONT_HOOK_BLOCKER:
        case enums::ddnet::TILE_FRONT_HOOK_BLOCKER_HALF:
        case enums::ddnet::TILE_FRONT_LASER_OLD:
        case enums::ddnet::TILE_FRONT_COLLISION_OFF_OLD:
        case enums::ddnet::TILE_FRONT_ENDLESS_HOOK_OLD:
        case enums::ddnet::TILE_FRONT_HIT_OFF_OLD:
        case enums::ddnet::TILE_FRONT_HOOK_OFF_OLD:
        case enums::ddnet::TILE_FRONT_TEAM_UNLOCK:
        case enums::ddnet::TILE_FRONT_PLAYER_COLLISION_OFF:
        case enums::ddnet::TILE_FRONT_ENDLESS_JUMPS_OFF:
        case enums::ddnet::TILE_FRONT_JETPACK_OFF:
        case enums::ddnet::TILE_FRONT_PLAYER_HOOK_OFF:
        case enums::ddnet::TILE_FRONT_TELE_GUN_ON:
        case enums::ddnet::TILE_FRONT_TELE_GUN_OFF:
        case enums::ddnet::TILE_FRONT_TELE_WEAPON_YELLOW:
        case enums::ddnet::TILE_FRONT_TELE_WEAPON_BLUE:
        case enums::ddnet::TILE_FRONT_COLLISION_ON:
        case enums::ddnet::TILE_FRONT_ENDLESS_JUMPS_ON:
        case enums::ddnet::TILE_FRONT_JETPACK_ON:
        case enums::ddnet::TILE_FRONT_PLAYER_HOOK_ON:
        case enums::ddnet::TILE_FRONT_TELE_GRENADE_ON:
        case enums::ddnet::TILE_FRONT_TELE_GRENADE_OFF:
        case enums::ddnet::TILE_FRONT_TELE_LASER_ON:
        case enums::ddnet::TILE_FRONT_TELE_LASER_OFF:
        case enums::ddnet::TILE_FRONT_LIVE_FREEZE:
        case enums::ddnet::TILE_FRONT_LIVE_UNFREEZE:
        case enums::ddnet::TILE_FRONT_ENTITIES_OFF_L:
        case enums::ddnet::TILE_FRONT_ENTITIES_OFF_R:
        case enums::ddnet::TILE_FRONT_SPAWN_ALL:
        case enums::ddnet::TILE_FRONT_SPAWN_RED:
        case enums::ddnet::TILE_FRONT_SPAWN_BLUE:
        case enums::ddnet::TILE_FRONT_FLAG_RED:
        case enums::ddnet::TILE_FRONT_FLAG_BLUE:
        case enums::ddnet::TILE_FRONT_ARMOR:
        case enums::ddnet::TILE_FRONT_HEART:
        case enums::ddnet::TILE_FRONT_WEAPON_SHOTGUN:
        case enums::ddnet::TILE_FRONT_WEAPON_GRENADE:
        case enums::ddnet::TILE_FRONT_WEAPON_NINJA:
        case enums::ddnet::TILE_FRONT_WEAPON_LASER:
        case enums::ddnet::TILE_FRONT_LASER_FAST_CCW:
        case enums::ddnet::TILE_FRONT_LASER_MEDIUM_CCW:
        case enums::ddnet::TILE_FRONT_LASER_SLOW_CCW:
        case enums::ddnet::TILE_FRONT_LASER_STATIC:
        case enums::ddnet::TILE_FRONT_LASER_SLOW_CW:
        case enums::ddnet::TILE_FRONT_LASER_MEDIUM_CW:
        case enums::ddnet::TILE_FRONT_LASER_FAST_CW:
        case enums::ddnet::TILE_FRONT_LASER_3:
        case enums::ddnet::TILE_FRONT_LASER_6:
        case enums::ddnet::TILE_FRONT_LASER_9:
        case enums::ddnet::TILE_FRONT_LASER_EXTEND_1:
        case enums::ddnet::TILE_FRONT_LASER_EXTEND_2:
        case enums::ddnet::TILE_FRONT_LASER_EXTEND_3:
        case enums::ddnet::TILE_FRONT_LASER_SHORTEN_1:
        case enums::ddnet::TILE_FRONT_LASER_SHORTEN_2:
        case enums::ddnet::TILE_FRONT_LASER_SHORTEN_3:
        case enums::ddnet::TILE_FRONT_PLASMA_EXPLODE:
        case enums::ddnet::TILE_FRONT_PLASMA_FREEZE:
        case enums::ddnet::TILE_FRONT_PLASMA_FREEZE_EXPLODE:
        case enums::ddnet::TILE_FRONT_PLASMA_UNFREEZE:
        case enums::ddnet::TILE_FRONT_BULLET_FREEZE_EXPLODE:
        case enums::ddnet::TILE_FRONT_BULLET_FREEZE:
        case enums::ddnet::TILE_FRONT_PULLER_WEAK:
        case enums::ddnet::TILE_FRONT_PULLER_MEDIUM:
        case enums::ddnet::TILE_FRONT_PULLER_STRONG:
        case enums::ddnet::TILE_FRONT_PULLER_WEAK_WALL:
        case enums::ddnet::TILE_FRONT_PULLER_MEDIUM_WALL:
        case enums::ddnet::TILE_FRONT_DOOR:
            return true;
    }
    return false;
}

bool isTeleEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case enums::ddnet::TILE_TELE_RED_FROM:
        case enums::ddnet::TILE_TELE_WEAPON:
        case enums::ddnet::TILE_TELE_HOOK:
        case enums::ddnet::TILE_TELE_BLUE_FROM:
        case enums::ddnet::TILE_TELE_TO:
        case enums::ddnet::TILE_TELE_CP:
        case enums::ddnet::TILE_TELE_CTO:
        case enums::ddnet::TILE_TELE_CFRM_BLUE:
        case enums::ddnet::TILE_TELE_CFRM_RED:
            return true;
    }
    return false;
}

bool isTuneEntity(const Tile& tile) noexcept {
    return tile.index == enums::ddnet::TILE_TUNE_ZONE;
}

bool isSwitchEntity(const Tile& tile) noexcept {
    switch (tile.index) {
        case enums::ddnet::TILE_SWITCH_EMPTY:
        case enums::ddnet::TILE_SWITCH_JUMP_AMOUNT:
        case enums::ddnet::TILE_SWITCH_FREEZE:
        case enums::ddnet::TILE_SWITCH_DEEP_FREEZE:
        case enums::ddnet::TILE_SWITCH_DEEP_UNFREEZE:
        case enums::ddnet::TILE_SWITCH_PLAYER_HIT_ON:
        case enums::ddnet::TILE_SWITCH_PLAYER_HIT_OFF:
        case enums::ddnet::TILE_SWITCH_BUTTON_TIMED_ON:
        case enums::ddnet::TILE_SWITCH_BUTTON_TIMED_OFF:
        case enums::ddnet::TILE_SWITCH_BUTTON_ON:
        case enums::ddnet::TILE_SWITCH_BUTTON_OFF:
        case enums::ddnet::TILE_SWITCH_TIME_PENALTY:
        case enums::ddnet::TILE_SWITCH_TIME_BONUS:
        case enums::ddnet::TILE_SWITCH_TELE_WEAPON_YELLOW:
        case enums::ddnet::TILE_SWITCH_TELE_WEAPON_BLUE:
        case enums::ddnet::TILE_SWITCH_LIVE_FREEZE:
        case enums::ddnet::TILE_SWITCH_LIVE_UNFREEZE:
        case enums::ddnet::TILE_SWITCH_ARMOR:
        case enums::ddnet::TILE_SWITCH_HEART:
        case enums::ddnet::TILE_SWITCH_WEAPON_SHOTGUN:
        case enums::ddnet::TILE_SWITCH_WEAPON_GRENADE:
        case enums::ddnet::TILE_SWITCH_WEAPON_NINJA:
        case enums::ddnet::TILE_SWITCH_WEAPON_LASER:
        case enums::ddnet::TILE_SWITCH_LASER_FAST_CCW:
        case enums::ddnet::TILE_SWITCH_LASER_MEDIUM_CCW:
        case enums::ddnet::TILE_SWITCH_LASER_SLOW_CCW:
        case enums::ddnet::TILE_SWITCH_LASER_STATIC:
        case enums::ddnet::TILE_SWITCH_LASER_SLOW_CW:
        case enums::ddnet::TILE_SWITCH_LASER_MEDIUM_CW:
        case enums::ddnet::TILE_SWITCH_LASER_FAST_CW:
        case enums::ddnet::TILE_SWITCH_LASER_3:
        case enums::ddnet::TILE_SWITCH_LASER_6:
        case enums::ddnet::TILE_SWITCH_LASER_9:
        case enums::ddnet::TILE_SWITCH_LASER_EXTEND_1:
        case enums::ddnet::TILE_SWITCH_LASER_EXTEND_2:
        case enums::ddnet::TILE_SWITCH_LASER_EXTEND_3:
        case enums::ddnet::TILE_SWITCH_LASER_SHORTEN_1:
        case enums::ddnet::TILE_SWITCH_LASER_SHORTEN_2:
        case enums::ddnet::TILE_SWITCH_LASER_SHORTEN_3:
        case enums::ddnet::TILE_SWITCH_PLASMA_EXPLODE:
        case enums::ddnet::TILE_SWITCH_PLASMA_FREEZE:
        case enums::ddnet::TILE_SWITCH_PLASMA_FREEZE_EXPLODE:
        case enums::ddnet::TILE_SWITCH_PLASMA_UNFREEZE:
        case enums::ddnet::TILE_SWITCH_BULLET_FREEZE_EXPLODE:
        case enums::ddnet::TILE_SWITCH_BULLET_FREEZE:
        case enums::ddnet::TILE_SWITCH_PULLER_WEAK:
        case enums::ddnet::TILE_SWITCH_PULLER_MEDIUM:
        case enums::ddnet::TILE_SWITCH_PULLER_STRONG:
        case enums::ddnet::TILE_SWITCH_PULLER_WEAK_WALL:
        case enums::ddnet::TILE_SWITCH_PULLER_MEDIUM_WALL:
        case enums::ddnet::TILE_SWITCH_PULLER_STRONG_WALL:
        case enums::ddnet::TILE_SWITCH_DOOR:
            return true;
    }
    return false;
}

bool isSpeedupEntity(const Tile& tile) noexcept {
    return tile.index == enums::ddnet::TILE_SPEEDUP_ARROW;
}

bool isServerSetting(const QString& string) noexcept {
    // TODO
    return false;
}

}
