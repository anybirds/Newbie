#include "projectwidget.h"
#include "scenewidget.h"
#include "objectitem.h"
#include "viewportwidget.h"

#include <QDebug>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>

#include <Common/Project.hpp>
#include <Common/Resource.hpp>
#include <Common/Scene.hpp>

using namespace Engine;

ProjectWidget::ProjectWidget(QWidget *parent) : QWidget(parent)
{

}

void ProjectWidget::OnProjectOpen() {
    QListWidget *listWidget = findChild<QListWidget *>();
    for (Scene *scene : Project::sceneset) {
        ListItem *item = new ListItem(tr(scene->GetName().c_str()), listWidget);
        item->SetObject(scene);
    }
    for (Resource *res : Project::resset) {
        ListItem *item = new ListItem(tr(res->GetName().c_str()), listWidget);
        item->SetObject(res);
    }
}

void ProjectWidget::OnItemDoubleClicked(QListWidgetItem *item) {
    ListItem *objItem = dynamic_cast<ListItem *>(item);
    if (objItem) {
// wingdi.h
#ifdef GetObject
#undef GetObject
        Scene *scene = dynamic_cast<Scene *>(objItem->GetObject());
#define GetObject GetObjectA
#else
        Scene *scene = dynamic_cast<Scene *>(objItem->GetObject());
#endif
        // SceneWidget is unique and should exist with a ViewportWidget
        SceneWidget *sceneWidget = window()->findChild<SceneWidget *>();
        ViewportWidget *viewportWidget = sceneWidget->findChild<ViewportWidget *>();
        viewportWidget->makeCurrent();
        if (!Scene::Load(scene->GetName())) {
            viewportWidget->doneCurrent();
            QMessageBox::warning(this, "Warning", "Failed to load scene json file " + tr(scene->GetPath().c_str()));
            return;
        }
        viewportWidget->doneCurrent();
        emit SceneOpen();
    }
}
