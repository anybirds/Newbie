#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;

class ProjectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectWidget(QWidget *parent = nullptr);

signals:
    void SceneOpen();

private slots:
    void OnProjectOpen();
    void OnItemDoubleClicked(QListWidgetItem *item);
};

#endif // PROJECTWIDGET_H
