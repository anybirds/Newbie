#ifndef HIERARCHYWIDGET_H
#define HIERARCHYWIDGET_H

#include <QWidget>

class HierarchyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HierarchyWidget(QWidget *parent = nullptr);

private slots:
    void OnSceneOpen();
};

#endif // HIERARCHYWIDGET_H
