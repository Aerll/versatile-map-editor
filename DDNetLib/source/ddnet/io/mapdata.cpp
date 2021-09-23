#include <ddnet/io/mapdata.hpp>

#include <ddnet/io/ddmdata.hpp>

namespace ddnet::io {

DDMData MAPData::toDDM() const {
    DDMData ddm;
    ddm.map.author = details.author;
    ddm.map.credits = details.credits;
    ddm.map.license = details.license;
    ddm.map.version = details.map_version.toInt();

    for (const auto& map_group : groups) {
        map::Group group = {
            .name = map_group.name,
            .is_visible = true
        };

        for (const auto& current_layer : map_group.layers) {
            switch (current_layer.index()) {
                case 0: { // tile layer
                    const auto& map_tile_layer = std::get<io::LayerTile>(current_layer);
                    if (map_tile_layer.special_type == map::SpecialLayerType::Game) {
                        map::GameLayer game_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == map::SpecialLayerType::Front) {
                        map::FrontLayer front_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == map::SpecialLayerType::Tele) {
                        map::TeleLayer tele_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == map::SpecialLayerType::Tune) {
                        map::TuneLayer tune_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == map::SpecialLayerType::Switch) {
                        map::SwitchLayer switch_layer;
                        // TODO
                    }
                    else if (map_tile_layer.special_type == map::SpecialLayerType::Speedup) {
                        map::SpeedupLayer speedup_layer;
                        // TODO
                    }
                    else {
                        map::TileLayer tile_layer;
                        // TODO
                    }

                    break;
                }

                case 1: { // quad layer
                    const auto& map_quad_layer = std::get<io::LayerQuad>(current_layer);
                    map::QuadLayer quad_layer;

                    // TODO
                    break;
                }

                case 2: { // sound layer
                    const auto& map_sound_layer = std::get<io::LayerSound>(current_layer);
                    map::SoundLayer sound_layer;

                    // TODO
                    break;
                }
            }
        }
    }

    // TODO
}

} // ddnet::io::
