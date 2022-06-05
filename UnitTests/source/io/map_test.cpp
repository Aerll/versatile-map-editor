#include <catch2.pch>

#include <vt/debug/error_code.hpp>
#include <vt/io/ddnet/mapdata.hpp>
#include <vt/io/ddnet/mapfilestream.hpp>
#include <vt/map/tile.hpp>
#include <vt/util/constants.hpp>
#include <vt/util/enums.hpp>
#include <vt/util/utility.hpp>

#include <QFileInfo>
#include <QString>

#include <algorithm>

using namespace vt;

TEST_CASE("io::ddnet::MAPData") {
    SECTION("Counting layers of each type") {
        io::ddnet::MAPData data;

        data.groups.push_back({}); // first group with 2 tile, 3 quad, 1 sound
        data.groups.back().layers.reserve(6);
        data.groups.back().layers.push_back(io::ddnet::MAPLayerQuad{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerTile{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerQuad{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerSound{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerQuad{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerTile{});

        data.groups.push_back({}); // second group with 3 tile, 1 quad, 2 sound
        data.groups.back().layers.reserve(6);
        data.groups.back().layers.push_back(io::ddnet::MAPLayerTile{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerQuad{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerSound{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerTile{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerSound{});
        data.groups.back().layers.push_back(io::ddnet::MAPLayerTile{});

        CHECK(data.tileLayersCount() == 5);
        CHECK(data.quadLayersCount() == 4);
        CHECK(data.soundLayersCount() == 3);
    }
}

TEST_CASE("io::ddnet::MAPFileStream") {
    SECTION("Reading data from a map file") {
        auto _TestMapData = [](QFileInfo path, QFileInfo output_path) {
            io::ddnet::MAPFileStream file_stream;
            REQUIRE_FALSE(util::failed(file_stream.loadFile(path)));

            { // file header
                const auto& header = file_stream.data.header;
                CHECK(header.id == constants::_map_id);
                CHECK(header.version == constants::_map_version);
            }

            { // map info
                const auto& details = file_stream.data.details;
                CHECK(details.author == QString{ "Pulsar" });
                CHECK(details.map_version == QString{ "4" });
                CHECK(details.credits == QString{ "Ravie" });
                CHECK(details.license == QString{ "CC" });
            }

            { // image assets
                const auto& image_assets = file_stream.data.assets.images;
                REQUIRE(image_assets.size() == 2);

                { // embedded
                    const auto& image = image_assets[0];
                    CHECK(image.name == QString{ "autofiller" });
                    CHECK(image.width == 1024);
                    CHECK(image.height == 1024);
                    CHECK_FALSE(image.is_external);
                }

                { // external
                    const auto& image = image_assets[1];
                    CHECK(image.name == QString{ "grass_main" });
                    CHECK(image.width == 1024);
                    CHECK(image.height == 1024);
                    CHECK(image.is_external);
                }
            }

            { // sound assets
                const auto& sound_assets = file_stream.data.assets.sounds;
                REQUIRE(sound_assets.size() == 1);

                const auto& sound = sound_assets[0];
                CHECK(sound.name == QString{ "ambient" });
                CHECK_FALSE(sound.is_external);
            }

            { // server settings
                const auto& settings = file_stream.data.settings;
                REQUIRE(settings.size() == 3);

                CHECK(settings[0] == QString{ "test_command 1" });
                CHECK(settings[1] == QString{ "test_command 2" });
                CHECK(settings[2] == QString{ "test_command 3" });
            }

            { // envelopes
                const auto& envelopes = file_stream.data.envelopes;
                REQUIRE(envelopes.size() == 6);
                REQUIRE(envelopes[0].points.size() == 2);
                REQUIRE(envelopes[1].points.size() == 2);
                REQUIRE(envelopes[2].points.size() == 2);
                REQUIRE(envelopes[3].points.size() == 2);
                REQUIRE(envelopes[4].points.size() == 2);
                REQUIRE(envelopes[5].points.size() == 2);

                CHECK(envelopes[0].name == QString{ "pos" });
                CHECK(envelopes[0].channels == 3);
                CHECK(envelopes[0].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[0].points;
                    CHECK(points[0].curve_type == enums::CurveType::Linear);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 55, 187, 357, 0 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 1000);
                    CHECK((points[1].values == std::array<qint32, 4>{ 296, 514, 817, 0 }));
                }

                CHECK(envelopes[1].name == QString{ "color" });
                CHECK(envelopes[1].channels == 4);
                CHECK_FALSE(envelopes[1].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[1].points;
                    CHECK(points[0].curve_type == enums::CurveType::Slow);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 262, 450, 840, 653 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 1000);
                    CHECK((points[1].values == std::array<qint32, 4>{ 104, 268, 466, 765 }));
                }

                CHECK(envelopes[2].name == QString{ "sound" });
                CHECK(envelopes[2].channels == 1);
                CHECK_FALSE(envelopes[2].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[2].points;
                    CHECK(points[0].curve_type == enums::CurveType::Fast);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 426, 0, 0, 0 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 1000);
                    CHECK((points[1].values == std::array<qint32, 4>{ 799, 0, 0, 0 }));
                }

                CHECK(envelopes[3].name == QString{ "pos2" });
                CHECK(envelopes[3].channels == 3);
                CHECK_FALSE(envelopes[3].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[3].points;
                    CHECK(points[0].curve_type == enums::CurveType::Smooth);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 0, 0, 0, 0 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 690);
                    CHECK((points[1].values == std::array<qint32, 4>{ 0, 0, 0, 0 }));
                }

                CHECK(envelopes[4].name == QString{ "color2" });
                CHECK(envelopes[4].channels == 4);
                CHECK_FALSE(envelopes[4].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[4].points;
                    CHECK(points[0].curve_type == enums::CurveType::Step);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 1024, 1024, 1024, 1024 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 695);
                    CHECK((points[1].values == std::array<qint32, 4>{ 1024, 1024, 1024, 1024 }));
                }

                CHECK(envelopes[5].name == QString{ "sound2" });
                CHECK(envelopes[5].channels == 1);
                CHECK_FALSE(envelopes[5].is_synchronized);
                { // envelope points
                    const auto& points = envelopes[5].points;
                    CHECK(points[0].curve_type == enums::CurveType::Linear);
                    CHECK(points[0].time == 0);
                    CHECK((points[0].values == std::array<qint32, 4>{ 0, 0, 0, 0 }));

                    CHECK(points[1].curve_type == enums::CurveType::Linear);
                    CHECK(points[1].time == 699);
                    CHECK((points[1].values == std::array<qint32, 4>{ 0, 0, 0, 0 }));
                }
            }

            { // groups
                const auto& groups = file_stream.data.groups;
                REQUIRE(groups.size() == 2);
                REQUIRE(groups[0].layers.size() == 6);
                REQUIRE(groups[1].layers.size() == 3);

                CHECK(groups[0].name == QString{ "Game" });
                CHECK(groups[0].offset_x == 0);
                CHECK(groups[0].offset_y == 0);
                CHECK(groups[0].parallax_x == 100);
                CHECK(groups[0].parallax_y == 100);
                CHECK(groups[0].clip_x == 0);
                CHECK(groups[0].clip_y == 0);
                CHECK(groups[0].clip_w == 0);
                CHECK(groups[0].clip_h == 0);
                CHECK_FALSE(groups[0].has_clipping);
                { // layers
                    { // game
                        const auto& layer_variant = groups[0].layers[0];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Game" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Game);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 114);
                        CHECK(layer.tiles_info.empty());
                    }

                    { // front
                        const auto& layer_variant = groups[0].layers[1];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Front" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Front);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 114);
                        CHECK(layer.tiles_info.empty());
                    }

                    { // tele
                        const auto& layer_variant = groups[0].layers[2];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Tele" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Tele);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 9);
                        CHECK(layer.tiles_info.size() == layer.width * layer.height);
                    }

                    { // switch
                        const auto& layer_variant = groups[0].layers[3];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Switch" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Switch);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 49);
                        CHECK(layer.tiles_info.size() == layer.width * layer.height);
                    }

                    { // speedup
                        const auto& layer_variant = groups[0].layers[4];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Speedup" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Speedup);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 1);
                        CHECK(layer.tiles_info.size() == layer.width * layer.height);
                    }

                    { // tune
                        const auto& layer_variant = groups[0].layers[5];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "Tune" });
                        CHECK(layer.asset_index == -1);
                        CHECK_FALSE(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::Tune);
                        CHECK(layer.color == util::Color{ 255, 255, 255, 255 });
                        CHECK(layer.color_envelope_index == -1);
                        CHECK(layer.color_envelope_offset == 0);

                        auto tile_count = std::ranges::count_if(layer.tiles, [](const auto& tile) { return tile.index != 0; });
                        CHECK(tile_count == 1);
                        CHECK(layer.tiles_info.size() == layer.width * layer.height);
                    }
                }

                CHECK(groups[1].name == QString{ "Test" });
                CHECK(groups[1].offset_x == -5);
                CHECK(groups[1].offset_y == 6);
                CHECK(groups[1].parallax_x == 69);
                CHECK(groups[1].parallax_y == 70);
                CHECK(groups[1].clip_x == 1);
                CHECK(groups[1].clip_y == 2);
                CHECK(groups[1].clip_w == 3);
                CHECK(groups[1].clip_h == 4);
                CHECK(groups[1].has_clipping);
                {
                    { // sound layer
                        const auto& layer_variant = groups[1].layers[0];
                        REQUIRE(layer_variant.index() == 2);

                        const auto& layer = std::get<io::ddnet::MAPLayerSound>(layer_variant);
                        REQUIRE(layer.sound_sources.size() == 2);

                        CHECK(layer.name == QString{ "SoundLayer" });
                        CHECK(layer.asset_index == 0);
                        CHECK(layer.is_detail);

                        { // rectangle sound source
                            const auto& sound_source = layer.sound_sources[0];
                            CHECK(sound_source.position == util::Point{ util::floatToFixed(505.0), util::floatToFixed(344.0) });
                            CHECK(sound_source.is_looped);
                            CHECK(sound_source.has_panning);
                            CHECK(sound_source.time_delay_sec == 0);
                            CHECK(sound_source.falloff == 80);
                            CHECK(sound_source.position_envelope_index == 0);
                            CHECK(sound_source.position_envelope_offset == 69);
                            CHECK(sound_source.sound_envelope_index == 2);
                            CHECK(sound_source.sound_envelope_offset == 0);

                            const auto& shape = sound_source.shape;
                            REQUIRE(shape.type == enums::SoundSourceShapeType::Rectangle);

                            const auto& size = std::get<util::RectangleSize>(shape.size);
                            CHECK(size == util::RectangleSize{ util::floatToFixed(970.0), util::floatToFixed(734.0) });
                        }

                        { // circle sound source
                            const auto& sound_source = layer.sound_sources[1];
                            CHECK(sound_source.position == util::Point{ util::floatToFixed(944.0), util::floatToFixed(368.0) });
                            CHECK_FALSE(sound_source.is_looped);
                            CHECK_FALSE(sound_source.has_panning);
                            CHECK(sound_source.time_delay_sec == 6969);
                            CHECK(sound_source.falloff == 80);
                            CHECK(sound_source.position_envelope_index == 3);
                            CHECK(sound_source.position_envelope_offset == 69);
                            CHECK(sound_source.sound_envelope_index == 5);
                            CHECK(sound_source.sound_envelope_offset == 0);

                            const auto& shape = sound_source.shape;
                            REQUIRE(shape.type == enums::SoundSourceShapeType::Circle);

                            const auto& radius = std::get<util::CircleRadius>(shape.size);
                            CHECK(radius == util::CircleRadius{ 1500 });
                        }
                    }

                    { // tile layer
                        const auto& layer_variant = groups[1].layers[1];
                        REQUIRE(layer_variant.index() == 0);

                        const auto& layer = std::get<io::ddnet::MAPLayerTile>(layer_variant);
                        CHECK(layer.name == QString{ "TileLayer" });
                        CHECK(layer.asset_index == 0);
                        CHECK(layer.is_detail);
                        CHECK(layer.width == 50);
                        CHECK(layer.height == 50);
                        CHECK(layer.special_type == enums::SpecialLayerType::None);
                        CHECK(layer.color == util::Color{ 189, 116, 141, 59 });
                        CHECK(layer.color_envelope_index == 1);
                        CHECK(layer.color_envelope_offset == -546);

                        REQUIRE(layer.tiles.size() == 2500);

                        CHECK(layer.tiles[0].index == 1);
                        CHECK(layer.tiles[0].flags == util::toUnderlying(enums::Rotation::N));

                        CHECK(layer.tiles[1].index == 1);
                        CHECK(layer.tiles[1].flags == util::toUnderlying(enums::Rotation::VHR));

                        CHECK(layer.tiles[2].index == 1);
                        CHECK(layer.tiles[2].flags == util::toUnderlying(enums::Rotation::VH));

                        CHECK(layer.tiles[3].index == 1);
                        CHECK(layer.tiles[3].flags == util::toUnderlying(enums::Rotation::R));

                        CHECK(layer.tiles[4].index == 1);
                        CHECK(layer.tiles[4].flags == util::toUnderlying(enums::Rotation::V));

                        CHECK(layer.tiles[5].index == 1);
                        CHECK(layer.tiles[5].flags == util::toUnderlying(enums::Rotation::HR));

                        CHECK(layer.tiles[6].index == 1);
                        CHECK(layer.tiles[6].flags == util::toUnderlying(enums::Rotation::H));

                        CHECK(layer.tiles[7].index == 1);
                        CHECK(layer.tiles[7].flags == util::toUnderlying(enums::Rotation::VR));
                    }

                    { // quad layer
                        const auto& layer_variant = groups[1].layers[2];
                        REQUIRE(layer_variant.index() == 1);

                        const auto& layer = std::get<io::ddnet::MAPLayerQuad>(layer_variant);
                        REQUIRE(layer.quads.size() == 1);

                        CHECK(layer.name == QString{ "QuadLayer" });
                        CHECK(layer.asset_index == 1);
                        CHECK(layer.is_detail);

                        const auto& quad = layer.quads[0];
                        CHECK(quad.position_envelope_index == 0);
                        CHECK(quad.position_envelope_offset == 46);
                        CHECK(quad.color_envelope_index == 1);
                        CHECK(quad.color_envelope_offset == 297);

                        CHECK(quad.points[0] == util::Point{ util::floatToFixed(640.0), util::floatToFixed(509.0) });
                        CHECK(quad.points[1] == util::Point{ util::floatToFixed(704.0), util::floatToFixed(509.0) });
                        CHECK(quad.points[2] == util::Point{ util::floatToFixed(640.0), util::floatToFixed(573.0) });
                        CHECK(quad.points[3] == util::Point{ util::floatToFixed(704.0), util::floatToFixed(573.0) });
                        CHECK(quad.points[4] == util::Point{ util::floatToFixed(672.0), util::floatToFixed(541.0) });

                        CHECK(quad.colors[0] == util::Color{ 210, 161, 149, 132 });
                        CHECK(quad.colors[1] == util::Color{ 153, 80, 188, 223 });
                        CHECK(quad.colors[2] == util::Color{ 94, 191, 225, 178 });
                        CHECK(quad.colors[3] == util::Color{ 194, 107, 37, 223 });

                        CHECK(quad.tex_coords[0] == util::Point{ util::floatToFixed(0.0), util::floatToFixed(0.0) });
                        CHECK(quad.tex_coords[1] == util::Point{ util::floatToFixed(1.0), util::floatToFixed(0.0) });
                        CHECK(quad.tex_coords[2] == util::Point{ util::floatToFixed(0.0), util::floatToFixed(1.0) });
                        CHECK(quad.tex_coords[3] == util::Point{ util::floatToFixed(1.0), util::floatToFixed(1.0) });
                    }
                }
            }

            if (!output_path.path().isEmpty())
                REQUIRE_FALSE(util::failed(file_stream.saveFile(output_path)));
        };

        _TestMapData(QFileInfo{ "data/map/input.map" }, QFileInfo{ "data/map/output.map" });
        _TestMapData(QFileInfo{ "data/map/output.map" }, QFileInfo{});
    }
}
