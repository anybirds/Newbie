#include "hierarchywidget.h"
#include "objectitem.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <Common/GameObject.hpp>
#include <Common/Scene.hpp>
#include <Common/Transform.hpp>

using namespace Engine;

HierarchyWidget::HierarchyWidget(QWidget *parent) : QWidget(parent)
{

}

void HierarchyWidget::OnSceneOpen() {
    QTreeWidget *treeWidget = findChild<QTreeWidget *>();
    QTreeWidgetItem *header = new QTreeWidgetItem((QTreeWidget *)nullptr, QStringList(Scene::curr->GetName().c_str()));
    treeWidget->setHeaderItem(header);
    for (GameObject *go : Scene::curr->goset) {
        TreeItem *item = new TreeItem(treeWidget, QStringList(go->GetName().c_str()));
    }
    /*
    for (GameObject *go : Scene::curr->goset) {
        Transform *parentTransform = go->GetTransform()->GetParent();
        if (parentTransform) {
            QString parentName(parentTransform->GetGameObject()->GetName().c_str());
            QList<QTreeWidgetItem *> list = treeWidget->findItems(parentName, Qt::MatchExactly);
            QTreeWidgetItem *p = list.constFirst();

        } else {

        }
    }
    */
}
