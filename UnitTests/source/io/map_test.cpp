#include <catch2.pch>

#include <ddnet/io/mapdata.hpp>

using namespace ddnet;

TEST_CASE("io::MAPData") {
    SECTION("Counting layers of each type") {
        io::MAPData data;

        data.groups.push_back({}); // first group with 2 tile, 3 quad, 1 sound
        data.groups.back().layers.reserve(6);
        data.groups.back().layers.push_back(io::LayerQuad{});
        data.groups.back().layers.push_back(io::LayerTile{});
        data.groups.back().layers.push_back(io::LayerQuad{});
        data.groups.back().layers.push_back(io::LayerSound{});
        data.groups.back().layers.push_back(io::LayerQuad{});
        data.groups.back().layers.push_back(io::LayerTile{});

        data.groups.push_back({}); // second group with 3 tile, 1 quad, 2 sound
        data.groups.back().layers.reserve(6);
        data.groups.back().layers.push_back(io::LayerTile{});
        data.groups.back().layers.push_back(io::LayerQuad{});
        data.groups.back().layers.push_back(io::LayerSound{});
        data.groups.back().layers.push_back(io::LayerTile{});
        data.groups.back().layers.push_back(io::LayerSound{});
        data.groups.back().layers.push_back(io::LayerTile{});

        CHECK(data.tileLayersCount() == 5);
        CHECK(data.quadLayersCount() == 4);
        CHECK(data.soundLayersCount() == 3);
    }
}
