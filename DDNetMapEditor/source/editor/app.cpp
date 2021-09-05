#include <editor/app.hpp>

#include <editor/mainwindow.hpp>

#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#define NOMINMAX
#include <windows.h>
#include <singleapplication.h>

namespace ddnet::editor {

qint32 App::run(qint32 argc, char* argv[]) {
    SingleApplication::addLibraryPath("./");
    SingleApplication::setStyle(QStyleFactory::create("Fusion"));

    SingleApplication app{ argc, argv, true, SingleApplication::Mode::User | SingleApplication::Mode::SecondaryNotification };

    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        app.sendMessage(app.arguments().join(", ").toUtf8());
        return 0;
    }

    // initialize resources
    if (debug::failed(App::initResources()))
        return -1; // TODO prompt
    if (debug::failed(App::initFonts()))
        return -1; // TODO prompt
    if (debug::failed(App::initTheme()))
        return -1; // TODO prompt
    if (debug::failed(App::initThemePalette()))
        return -1; // TODO prompt

    app.setFont(QFont{ App::resources.setting(SettingIndex::FontFamily), App::resources.setting(SettingIndex::FontSize).toInt() });

    editor::MainWindow window;
    window.HandleMessage_(app.applicationPid(), app.arguments().join(", ").toUtf8());
    window.show();

    QObject::connect(&app, &SingleApplication::receivedMessage, &window, &MainWindow::HandleMessage_);
    QObject::connect(&app, &SingleApplication::instanceStarted, [&window]() {
        window.setWindowState((window.windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        window.raise();
        window.activateWindow();
    });
    
    return app.exec();
}



debug::ErrorCode App::initResources() {
    return App::resources.initResources();
}



debug::ErrorCode App::initFonts() {
    debug::ErrorCode error_code;
    auto font_files = App::resources.fonts(error_code);
    if (debug::failed(error_code))
        return error_code;

    for (const auto& font_file : font_files)
        if (QFontDatabase::addApplicationFont(font_file) == -1)
            return debug::ErrorCode::Resource_Font_FailedToReadFont;
    return error_code;
}



debug::ErrorCode App::initTheme() {
    debug::ErrorCode error_code;
    App::instance()->setStyleSheet(App::resources.theme(error_code));
    return error_code;
}



debug::ErrorCode App::initThemePalette() {
    debug::ErrorCode error_code;
    App::instance()->setPalette(App::resources.themePalette(error_code));
    return error_code;
}



QApplication* App::instance() {
    return static_cast<QApplication*>(SingleApplication::instance());
}

} // ddnet::editor::
