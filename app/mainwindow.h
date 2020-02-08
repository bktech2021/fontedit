#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "./ui_mainwindow.h"
#include "mainwindowmodel.h"
#include "facewidget.h"
#include "glyphwidget.h"

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupActions();

    void displayFace(const Font::Face &face);
    void displayGlyph(const Font::Glyph &glyph);
    void updateActions(MainWindowModel::ActionsState actionsState);

    std::unique_ptr<Ui::MainWindow> ui_ { std::make_unique<Ui::MainWindow>() };

    std::unique_ptr<GlyphWidget> glyphWidget_ {};
    FaceWidget *faceWidget_ { nullptr };
    std::unique_ptr<MainWindowModel> viewModel_ { std::make_unique<MainWindowModel>() };
    std::unique_ptr<QGraphicsScene> faceScene_ { std::make_unique<QGraphicsScene>() };
};

#endif // MAINWINDOW_H
