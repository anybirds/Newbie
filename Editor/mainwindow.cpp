#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "projectwidget.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QLibrary>

#include <Common/Project.hpp>

using namespace Engine;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    ProjectWidget *projectWidget = findChild<ProjectWidget *>(tr("Project"));
    connect(this, SIGNAL(ProjectOpen()), projectWidget, SLOT(OnProjectOpen()));
    QListWidget *listWidget = findChild<QListWidget *>();
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), projectWidget, SLOT(OnItemDoubleClicked(QListWidgetItem *)));

    SceneWidget *sceneWidget = findChild<SceneWidget *>("Scene");
    HierarchyWidget *hierarchyWidget = findChild<HierarchyWidget *>("Hierarchy");
    connect(projectWidget, SIGNAL(SceneOpen()), sceneWidget, SLOT(OnSceneOpen()));
    connect(projectWidget, SIGNAL(SceneOpen()), hierarchyWidget, SLOT(OnSceneOpen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Project_triggered() {

}

void MainWindow::on_actionOpen_Project_triggered() {
    QString libpath = QFileDialog::getOpenFileName(this, "Open project shared library file");

    QLibrary lib(libpath);
    if (!lib.load()) {
        QMessageBox::warning(this, "Warning", "Failed to load project shared library file " + libpath);
        return;
    }

    typedef void (*Func)();
    Func TypeInit = reinterpret_cast<Func>(lib.resolve("TypeInit"));
    if (!TypeInit) {
        QMessageBox::warning(this, "Warning", "Failed to resolve TypeInit function");
        return;
    }
    TypeInit();

    QString jsonpath = QFileDialog::getOpenFileName(this, "Open project json file");

    if (!jsonpath.isEmpty()) {
        if (!Project::Load(jsonpath.toStdString())) {
            QMessageBox::warning(this, "Warning", "Failed reading project json file " + jsonpath);
            return;
        }
        emit ProjectOpen();
    }
}

void MainWindow::on_actionSave_Project_triggered() {

}
