#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto window = new MainWindow();
    window->show();
    return QApplication::exec();
}
