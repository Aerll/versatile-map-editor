#pragma once

#include <editor/resourcemanager.hpp>

class QApplication;

namespace ddnet::editor {

class App {
public:
    static qint32 run(qint32 argc, char* argv[]);

private:
    static debug::ErrorCode initResources();
    static debug::ErrorCode initFonts();
    static debug::ErrorCode initTheme();
    static debug::ErrorCode initThemePalette();

    static QApplication* instance();

public:
    inline static ResourceManager resources = {};
};

} // ddnet::editor::
