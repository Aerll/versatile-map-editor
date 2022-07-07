#pragma once

#include <vt/core/chunk.hpp>
#include <vt/core/quad.hpp>
#include <vt/core/sound_source.hpp>
#include <vt/core/tile.hpp>

#include <QFileInfo>
#include <QColor>
#include <QPoint>
#include <QSize>

#include <memory>
#include <vector>

namespace vt::core {

class Asset;

class BaseLayer {
public:
    bool is_visible = false;
    bool is_detail = false;

protected:
    Asset* asset = nullptr;
};



class BaseTileLayer : public BaseLayer {
    using node_iterator = std::vector<std::shared_ptr<core::Chunk<core::Tile>>>::iterator;

public:
    virtual ~BaseTileLayer() = default;

    node_iterator nodeAt(size_t index);

    bool hasNode(qint32 id);
    bool hasNode(QPoint coords);

    void addNode(std::shared_ptr<core::Chunk<core::Tile>>& node);

    inline QSize getSize() noexcept { return size; }
    void updateSize();

protected:
    std::vector<std::shared_ptr<core::Chunk<core::Tile>>> nodes;
    QSize size = { 0, 0 };
};



class TileLayer : public BaseTileLayer {
public:
    QColor color{ Qt::white };
    quint16 color_envelope_id = 0;
    quint32 color_envelope_offset = 0;
};



class GameLayer : public BaseTileLayer {};
class FrontLayer : public BaseTileLayer {};
class TeleLayer : public BaseTileLayer {};
class TuneLayer : public BaseTileLayer {};
class SwitchLayer : public BaseTileLayer {};
class SpeedupLayer : public BaseTileLayer {};



class QuadLayer : public BaseLayer {
public:
    std::vector<Quad> quads;
};



class SoundLayer : public BaseLayer {
public:
    std::vector<SoundSourceCircle> circle_sound_sources;
    std::vector<SoundSourceRectangle> rectangle_sound_sources;
};

}
