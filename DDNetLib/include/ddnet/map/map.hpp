#pragma once

#include <ddnet/map/envelope.hpp>
#include <ddnet/map/group.hpp>
#include <ddnet/map/layer.hpp>

#include <QString>

#include <vector>

namespace ddnet {

class Map {
public:
    QString author;
    QString credits;
    QString license;
    qint32 version;

    std::vector<Group> groups;

    QuadLayer skybox_layer;
    GameLayer game_layer;
    FrontLayer front_layer;
    TeleLayer tele_layer;
    TuneLayer tune_layer;
    SwitchLayer switch_layer;
    SpeedupLayer speedup_layer;

    std::vector<PositionEnvelope> position_envelopes;
    std::vector<ColorEnvelope> color_envelopes;
    std::vector<SoundEnvelope> sound_envelopes;

    std::vector<QString> server_settings;
};

}
