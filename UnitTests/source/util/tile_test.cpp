#include <catch2.pch>

#include <ddnet/util/tile.hpp>

using namespace ddnet;

TEST_CASE("util::toTilesetCoords") {
    constexpr util::Point<qint32> coords11 = util::toTilesetCoords(17);
    constexpr util::Point<qint32> coords69 = util::toTilesetCoords(105);

    CHECK(coords11 == util::Point<qint32>{ 1, 1 });
    CHECK(coords69 == util::Point<qint32>{ 6, 9 });
}

TEST_CASE("util::rotateTile") {
    SECTION("Rotate clockwise") {
        map::Tile tile;

        // non-mirrored
        tile.rotation = Rotation::N;

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::R);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::VH);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::VHR);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::N);

        // mirrored
        tile.rotation = Rotation::V;

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::VR);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::H);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::HR);

        util::rotateTile(tile, Rotate::Clockwise);
        CHECK(tile.rotation == Rotation::V);
    }

    SECTION("Rotate counter-clockwise") {
        map::Tile tile;

        // non-mirrored
        tile.rotation = Rotation::N;

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::VHR);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::VH);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::R);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::N);

        // mirrored
        tile.rotation = Rotation::V;

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::HR);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::H);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::VR);

        util::rotateTile(tile, Rotate::CounterClockwise);
        CHECK(tile.rotation == Rotation::V);
    }
}

TEST_CASE("util::mirrorTile") {
    SECTION("In relation to vertical axis") {
        map::Tile tile;

        // N <-> V
        tile.rotation = Rotation::N;

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::V);

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::N);

        // R <-> HR
        tile.rotation = Rotation::R;

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::HR);

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::R);

        // VH <-> H
        tile.rotation = Rotation::VH;

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::H);

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::VH);

        // VHR <-> VR
        tile.rotation = Rotation::VHR;

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::VR);

        util::mirrorTile(tile, Mirror::VerticalAxis);
        CHECK(tile.rotation == Rotation::VHR);
    }

    SECTION("In relation to horizontal axis") {
        map::Tile tile;

        // N <-> H
        tile.rotation = Rotation::N;

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::H);

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::N);

        // R <-> VR
        tile.rotation = Rotation::R;

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::VR);

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::R);

        // VH <-> V
        tile.rotation = Rotation::VH;

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::V);

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::VH);

        // VHR <-> HR
        tile.rotation = Rotation::VHR;

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::HR);

        util::mirrorTile(tile, Mirror::HorizontalAxis);
        CHECK(tile.rotation == Rotation::VHR);
    }
}
