#include <catch2.pch>

#include <vt/core/chunk.hpp>

using namespace vt;

TEST_CASE("core::Chunk") {
    SECTION("Initialize from position") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        CHECK(chunk.getID() == static_cast<quint32>(pos.y() * constants::_grid_size + pos.x()));

        REQUIRE(chunk.tiles.size() == constants::_chunk_size * constants::_chunk_size);
        
        chunk.tiles[3 * constants::_chunk_size + 7] = 37;
        CHECK(chunk.tileAt(7, 3) == 37);
    }

    SECTION("Editing tiles") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        CHECK(chunk.tiles[9 * constants::_chunk_size + 6] == 0);
        chunk.tileAt(6, 9) = 69;
        CHECK(chunk.tiles[9 * constants::_chunk_size + 6] == 69);
        CHECK(chunk.tileAt(6, 9) == 69);
    }
    
    SECTION("Erasing tiles") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        chunk.tileAt(6, 9) = 69;
        chunk.clearAt(6, 9);
        CHECK(chunk.tileAt(6, 9) == 0);

        for (qint32 x = 0; x < 4; ++x) {
            for (qint32 y = 0; y < 4; ++y) {
                chunk.tileAt(x, y) = 44;
            }
        }

        chunk.clearArea(QRect{ QPoint{ 1, 1 }, QSize{ 2, 2 } });

        for (qint32 x = 0; x < 4; ++x) {
            for (qint32 y = 0; y < 4; ++y) {
                if (x >= 1 && x <= 2 && y >= 1 && y <= 2)
                    CHECK(chunk.tileAt(x, y) == 0);
                else
                    CHECK(chunk.tileAt(x, y) == 44);
            }
        }
    }
}
