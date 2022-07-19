#include <catch2.pch>

#include <vt/core/chunk.hpp>

using namespace vt;

TEST_CASE("core::Chunk") {
    SECTION("Initialize from position") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        CHECK(chunk.getID() == static_cast<quint32>(pos.y() * constants::_grid_size + pos.x()));

        REQUIRE(chunk.tiles.size() == constants::_chunk_size * constants::_chunk_size);
        
        chunk.tiles[3 * constants::_chunk_size + 7].index = 37;
        CHECK(chunk.tileAt(7, 3).index == 37);
    }

    SECTION("Accessing individual tiles") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        CHECK(chunk.tiles[9 * constants::_chunk_size + 6].index == 0);
        chunk.tileAt({ 6, 9 }).index = 69;
        CHECK(chunk.tiles[9 * constants::_chunk_size + 6].index == 69);
        CHECK(chunk.tileAt({ 6, 9 }).index == 69);
    }

    SECTION("Modifying tiles") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        core::Tile default_tile;
        core::Tile check_tile = { .index = 69, .rotation = enums::Rotation::VHR, .is_selected = true };

        // modifyAt
        chunk.modifyAt({ 6, 9 }, [&](core::Tile& tile) { 
            tile = check_tile;
        });
        CHECK(chunk.tileAt(6, 9) == check_tile);

        // modifyAtIf
        chunk.modifyAtIf({ 6, 9 }, [&](core::Tile& tile) {
            tile = default_tile;
        }, [&](const core::Tile& tile) {
            return tile != check_tile;
        });
        CHECK(chunk.tileAt(6, 9) == check_tile);
        chunk.modifyAtIf({ 6, 9 }, [&](core::Tile& tile) {
            tile = default_tile;
        }, [&](const core::Tile& tile) {
            return tile == check_tile;
        });
        CHECK(chunk.tileAt(6, 9) == default_tile);

        // modifyArea
        chunk.modifyArea(QRect{ 1, 1, 3, 7 }, [&](core::Tile& tile) { 
            tile = check_tile;
        });
        CHECK(chunk.tileAt(0, 0) == default_tile);
        CHECK(chunk.tileAt(0, 0) != chunk.tileAt(1, 1));
        CHECK(std::ranges::count(chunk.tiles, check_tile) == 3 * 7);

        // modifyAreaIf
        chunk.modifyAreaIf(QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }, [&](core::Tile& tile) { 
            tile = check_tile;
        }, [&](const core::Tile& tile) {
            return tile != check_tile;
        });
        CHECK(std::ranges::count(chunk.tiles, check_tile) == qsizetype(chunk.tiles.size()));
        chunk.modifyAreaIf(QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }, [&](core::Tile& tile) { 
            tile = default_tile;
        }, [&](const core::Tile& tile) {
            return tile == check_tile;
        });
        CHECK(std::ranges::count(chunk.tiles, default_tile) == qsizetype(chunk.tiles.size()));
    }

    SECTION("Basic modifiers") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        // _Clear
        chunk.tileAt(6, 9) = { .index = 69, .rotation = enums::Rotation::VHR, .is_selected = true };
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_Clear);
        CHECK(chunk.tileAt(6, 9) == core::Tile{});

        // _Select
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_Select);
        CHECK(chunk.tileAt(6, 9).is_selected);

        // _Unselect
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_Unselect);
        CHECK_FALSE(chunk.tileAt(6, 9).is_selected);

        // _RotateClockwise
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_RotateClockwise);
        CHECK(chunk.tileAt(6, 9).rotation != enums::Rotation::N);

        // _RotateCounterClockwise
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_RotateCounterClockwise);
        CHECK(chunk.tileAt(6, 9).rotation == enums::Rotation::N);

        // _MirrorVerticalAxis
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_MirrorVerticalAxis);
        CHECK(chunk.tileAt(6, 9).rotation != enums::Rotation::N);
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_MirrorVerticalAxis);
        CHECK(chunk.tileAt(6, 9).rotation == enums::Rotation::N);

        // _MirrorHorizontalAxis
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_MirrorHorizontalAxis);
        CHECK(chunk.tileAt(6, 9).rotation != enums::Rotation::N);
        chunk.modifyAt(6, 9, core::Chunk<core::Tile>::_MirrorHorizontalAxis);
        CHECK(chunk.tileAt(6, 9).rotation == enums::Rotation::N);
    }

    SECTION("Search count") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        chunk.modifyArea(QRect{ 3, 3, 7, 7 }, [](core::Tile& tile) {
            tile.index = 37;
        });
        
        auto found = chunk.findAll({ .index = 37 });
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == 7 * 7);

        found = chunk.findAll({ .index = 0 });
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == chunk.tiles.size() - 7 * 7);

        found = chunk.findAll(QRect{ 1, 1, 4, 6 }, { .index = 37 });
        REQUIRE(found.size() == found.capacity());
        REQUIRE(found.size() == 2 * 4);
    }

    SECTION("Search result") {
        QPoint pos = { 7, 37 };
        core::Chunk<core::Tile> chunk{ pos };

        chunk.tileAt(1, 1) = { .index = 69 };
        chunk.tileAt(3, 7) = { .index = 37 };
        chunk.tileAt(6, 9) = { .index = 69 };

        auto found = chunk.findAll({ .index = 37 });
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 3, 7 });

        found = chunk.findAll({ .index = 69 });
        REQUIRE(found.size() == 2);
        CHECK(found[0] == QPoint{ 1, 1 });
        CHECK(found[1] == QPoint{ 6, 9 });

        found = chunk.findAll(QRect{ 1, 1, 3, 7 }, { .index = 37 });
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 3, 7 });

        found = chunk.findAll(QRect{ 1, 1, 3, 7 }, { .index = 69 });
        REQUIRE(found.size() == 1);
        CHECK(found[0] == QPoint{ 1, 1 });
    }
}
