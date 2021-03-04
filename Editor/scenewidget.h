#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>

class SceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = nullptr);

private slots:
    void OnSceneOpen();
};

#endif // SCENEWIDGET_H
