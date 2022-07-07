#include <catch2.pch>

#include <vt/core/chunk.hpp>

using namespace vt;

TEST_CASE("core::Chunk") {
    SECTION("Initialize from position") {
        quint16 x = 7;
        quint16 y = 37;
        core::Chunk<qint32> chunk{ x, y };

        CHECK(chunk.getID() == static_cast<quint32>(y * constants::_grid_size + x));

        REQUIRE(chunk.tiles.size() == constants::_chunk_size * constants::_chunk_size);
        
        chunk.tiles[3 * constants::_chunk_size + 7] = 37;
        CHECK(chunk.tileAt(7, 3) == 37);
    }
}
