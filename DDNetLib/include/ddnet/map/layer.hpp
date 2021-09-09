#pragma once

#include <ddnet/map/node.hpp>

#include <QFileInfo>
#include <QColor>
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
    quint16 group_id = 0;
    quint16 id = 0;
    QFileInfo asset_path;
    bool is_detail = false;
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



class QuadLayer {
public:
    quint16 group_id = 0;
    quint16 id = 0;
    QFileInfo asset_path;
    bool is_detail = false;
};



class SoundLayer {
public:
    quint16 group_id = 0;
    quint16 id = 0;
    QFileInfo asset_path;
    bool is_detail = false;
};

} // ddnet::map::
