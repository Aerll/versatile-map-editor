#include <catch2.pch>

#include <vt/core/tile.hpp>

using namespace vt;

TEST_CASE("core::Tile") {
    SECTION("Tileset coords from index") {
        core::Tile coords11 = { .index = 17 };
        core::Tile coords69 = { .index = 105 };

        CHECK(coords11.getTilesetCoords() == QPoint{ 1, 1 });
        CHECK(coords69.getTilesetCoords() == QPoint{ 6, 9 });
    }

    SECTION("Selecting") {
        core::Tile tile;

        CHECK_FALSE(tile.is_selected);
        tile.select();
        CHECK(tile.is_selected);
        tile.unselect();
        CHECK_FALSE(tile.is_selected);
    }

    SECTION("Rotate clockwise") {
        core::Tile tile;

        // non-mirrored
        tile.rotation = enums::Rotation::N;

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::R);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VH);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VHR);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::N);

        // mirrored
        tile.rotation = enums::Rotation::V;

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::VR);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::H);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::HR);

        tile.rotate(enums::Rotate::Clockwise);
        CHECK(tile.rotation == enums::Rotation::V);
    }

    SECTION("Rotate counter-clockwise") {
        core::Tile tile;

        // non-mirrored
        tile.rotation = enums::Rotation::N;

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VHR);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VH);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::R);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::N);

        // mirrored
        tile.rotation = enums::Rotation::V;

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::HR);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::H);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::VR);

        tile.rotate(enums::Rotate::CounterClockwise);
        CHECK(tile.rotation == enums::Rotation::V);
    }

    SECTION("Mirror in relation to vertical axis") {
        core::Tile tile;

        // N <-> V
        tile.rotation = enums::Rotation::N;

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::V);

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::N);

        // R <-> HR
        tile.rotation = enums::Rotation::R;

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::HR);

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::R);

        // VH <-> H
        tile.rotation = enums::Rotation::VH;

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::H);

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VH);

        // VHR <-> VR
        tile.rotation = enums::Rotation::VHR;

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VR);

        tile.mirror(enums::Mirror::VerticalAxis);
        CHECK(tile.rotation == enums::Rotation::VHR);
    }

    SECTION("Mirror in relation to horizontal axis") {
        core::Tile tile;

        // N <-> H
        tile.rotation = enums::Rotation::N;

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::H);

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::N);

        // R <-> VR
        tile.rotation = enums::Rotation::R;

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VR);

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::R);

        // VH <-> V
        tile.rotation = enums::Rotation::VH;

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::V);

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VH);

        // VHR <-> HR
        tile.rotation = enums::Rotation::VHR;

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::HR);

        tile.mirror(enums::Mirror::HorizontalAxis);
        CHECK(tile.rotation == enums::Rotation::VHR);
    }
}
