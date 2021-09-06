#pragma once

#include <ddnet/map/node.hpp>
#include <ddnet/map/layer_properties.hpp>

#include <QPoint>
#include <QSize>

#include <memory>
#include <vector>

namespace ddnet::map {

class BaseTileLayer {
    using node_iterator = std::vector<std::shared_ptr<Node>>::iterator;

public:
    virtual ~BaseTileLayer() = default;

    node_iterator nodeAt(size_t index);

    bool hasNode(qint32 id);
    bool hasNode(QPoint coords);

    void addNode(std::shared_ptr<Node>& node);

    inline QSize getSize() noexcept { return size; }
    void updateSize();

protected:
    std::vector<std::shared_ptr<Node>> nodes;
    QSize size = { 0, 0 };
};

class TileLayer : public BaseTileLayer {
public:
    TileLayerProperties properties;
};

class GameLayer : public BaseTileLayer {};
class FrontLayer : public BaseTileLayer {};
class TeleLayer : public BaseTileLayer {};
class TuneLayer : public BaseTileLayer {};
class SwitchLayer : public BaseTileLayer {};
class SpeedupLayer : public BaseTileLayer {};

class QuadLayer {
    // TODO
};

class SoundLayer {
    // TODO
};

} // ddnet::map::
