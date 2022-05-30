#include <catch2.pch>

#include <vt/util/tile.hpp>

using namespace vt;

TEST_CASE("util::toTilesetCoords") {
    constexpr QPoint coords11 = util::toTilesetCoords(17);
    constexpr QPoint coords69 = util::toTilesetCoords(105);

    CHECK(coords11 == QPoint{ 1, 1 });
    CHECK(coords69 == QPoint{ 6, 9 });
}

TEST_CASE("util::rotateTile") {
    SECTION("enums::Rotate clockwise") {
        Tile tile;

        // non-mirrored
        tile.rotation = enums::Rotation::N;

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::R);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VH);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VHR);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::N);

        // mirrored
        tile.rotation = enums::Rotation::V;

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VR);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::H);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::HR);

        util::rotateTile(tile, enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::V);
    }

    SECTION("enums::Rotate counter-clockwise") {
        Tile tile;

        // non-mirrored
        tile.rotation = enums::Rotation::N;

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VHR);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VH);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::R);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::N);

        // mirrored
        tile.rotation = enums::Rotation::V;

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::HR);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::H);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VR);

        util::rotateTile(tile, enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::V);
    }
}

TEST_CASE("util::mirrorTile") {
    SECTION("In relation to vertical axis") {
        Tile tile;

        // N <-> V
        tile.rotation = enums::Rotation::N;

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::V);

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::N);

        // R <-> HR
        tile.rotation = enums::Rotation::R;

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::HR);

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::R);

        // VH <-> H
        tile.rotation = enums::Rotation::VH;

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::H);

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VH);

        // VHR <-> VR
        tile.rotation = enums::Rotation::VHR;

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VR);

        util::mirrorTile(tile, enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VHR);
    }

    SECTION("In relation to horizontal axis") {
        Tile tile;

        // N <-> H
        tile.rotation = enums::Rotation::N;

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::H);

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::N);

        // R <-> VR
        tile.rotation = enums::Rotation::R;

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VR);

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::R);

        // VH <-> V
        tile.rotation = enums::Rotation::VH;

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::V);

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VH);

        // VHR <-> HR
        tile.rotation = enums::Rotation::VHR;

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::HR);

        util::mirrorTile(tile, enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VHR);
    }
}
