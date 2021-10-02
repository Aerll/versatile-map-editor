#pragma once

#include <QImage>
#include <QOpenGLWidget>

namespace ddnet::editor {

class OpenGLWidget final : public QOpenGLWidget {
    Q_OBJECT;

public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget() = default;

protected:
    void paintEvent(QPaintEvent* event) final;
};

}
