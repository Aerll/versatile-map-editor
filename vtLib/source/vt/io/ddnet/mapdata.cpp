#include <vt/io/ddnet/mapdata.hpp>

#include <vt/io/ddmdata.hpp>

namespace vt::io::ddnet {

io::DDMData MAPData::toDDM() const {
    io::DDMData ddm;
    ddm.map.author = details.author;
    ddm.map.credits = details.credits;
    ddm.map.license = details.license;
    ddm.map.version = details.map_version.toInt();

    for (const auto& map_group : groups) {
        Group group = {
            .name = map_group.name,
            .is_visible = true
        };

        for (const auto& current_layer : map_group.layers) {
            switch (current_layer.index()) {
                case 0: { // tile layer
                    const auto& map_tile_layer = std::get<io::ddnet::MAPLayerTile>(current_layer);
                    if (map_tile_layer.special_type == enums::SpecialLayerType::Game) {
                        GameLayer game_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == enums::SpecialLayerType::Front) {
                        FrontLayer front_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == enums::SpecialLayerType::Tele) {
                        TeleLayer tele_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == enums::SpecialLayerType::Tune) {
                        TuneLayer tune_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == enums::SpecialLayerType::Switch) {
                        SwitchLayer switch_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == enums::SpecialLayerType::Speedup) {
                        SpeedupLayer speedup_layer;
                        // TODO
                    }
                    else {
                        TileLayer tile_layer;
                        // TODO
                    }

                    break;
                }

                case 1: { // quad layer
                    const auto& map_quad_layer = std::get<io::ddnet::MAPLayerQuad>(current_layer);
                    QuadLayer quad_layer;

                    // TODO
                    break;
                }

                case 2: { // sound layer
                    const auto& map_sound_layer = std::get<io::ddnet::MAPLayerSound>(current_layer);
                    SoundLayer sound_layer;

                    // TODO
                    break;
                }
            }
        }
    }

    // TODO
    return {};
}

}
