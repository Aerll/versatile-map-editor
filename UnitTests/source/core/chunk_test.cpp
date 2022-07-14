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

    SECTION("Editing") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        CHECK(chunk.tiles[9 * constants::_chunk_size + 6] == 0);
        chunk.tileAt({ 6, 9 }) = 69;
        CHECK(chunk.tiles[9 * constants::_chunk_size + 6] == 69);
        CHECK(chunk.tileAt({ 6, 9 }) == 69);

        CHECK(chunk.tileAt({ 3, 7 }) == 0);
        chunk.replaceAt({ 3, 7 }, 37);
        CHECK(chunk.tileAt({ 3, 7 }) == 37);

        std::ranges::fill(chunk.tiles, 0);
        chunk.replaceArea(QRect{ QPoint{ 1, 1 }, QSize{ 2, 3 } }, 37);
        for (qint32 x = 0; x < 4; ++x) {
            for (qint32 y = 0; y < 4; ++y) {
                if (x >= 1 && x <= 2 && y >= 1 && y <= 3)
                    CHECK(chunk.tileAt(x, y) == 37);
                else
                    CHECK(chunk.tileAt(x, y) == 0);
            }
        }
    }
    
    SECTION("Erasing") {
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

        chunk.clearArea(QRect{ QPoint{ 1, 1 }, QSize{ 2, 3 } });

        for (qint32 x = 0; x < 4; ++x) {
            for (qint32 y = 0; y < 4; ++y) {
                if (x >= 1 && x <= 2 && y >= 1 && y <= 3)
                    CHECK(chunk.tileAt(x, y) == 0);
                else
                    CHECK(chunk.tileAt(x, y) == 44);
            }
        }
    }

    SECTION("Search count") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        chunk.replaceArea(QRect{ QPoint{ 3, 3 }, QSize{ 7, 7 } }, 37);
        auto found = chunk.findAll(37);
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == 7 * 7);

        found = chunk.findAll(0);
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == chunk.tiles.size() - 7 * 7);

        found = chunk.findAll(QRect{ QPoint{ 1, 1 }, QSize{ 4, 6 } }, 37);
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == 2 * 4);
    }

    SECTION("Search result") {
        QPoint pos = { 7, 37 };
        core::Chunk<qint32> chunk{ pos };

        chunk.tileAt(1, 1) = 69;
        chunk.tileAt(3, 7) = 37;
        chunk.tileAt(6, 9) = 69;

        auto found = chunk.findAll(37);
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 3, 7 });

        found = chunk.findAll(69);
        REQUIRE(found.size() == 2);
        CHECK(found[0] == QPoint{ 1, 1 });
        CHECK(found[1] == QPoint{ 6, 9 });

        found = chunk.findAll(QRect{ QPoint{ 1, 1 }, QSize{ 3, 7 } }, 37);
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 3, 7 });

        found = chunk.findAll(QRect{ QPoint{ 1, 1 }, QSize{ 3, 7 } }, 69);
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 1, 1 });
    }
}
