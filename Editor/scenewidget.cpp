#include "scenewidget.h"

#include "viewportwidget.h"

SceneWidget::SceneWidget(QWidget *parent) : QWidget(parent)
{

}

void SceneWidget::OnSceneOpen() {
    ViewportWidget *viewportWidget = findChild<ViewportWidget *>();
    viewportWidget->update();
}
