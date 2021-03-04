#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QListWidgetItem>
#include <QTreeWidgetItem>

namespace Engine {
    class Object;
}

class ListItem : public QListWidgetItem
{
private:
    Engine::Object *obj;

public:
    ListItem(const QString &text, QListWidget *parent, int type = Type);

    Engine::Object *GetObject() const { return obj; }
    void SetObject(Engine::Object *obj) { this->obj = obj; }
};

class TreeItem : public QTreeWidgetItem
{
private:
    Engine::Object *obj;
public:
    TreeItem(QTreeWidget *parent, const QStringList &strings, int type = Type);
    TreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type = Type);

    Engine::Object *GetObject() const { return obj; }
    void SetObject(Engine::Object *obj) { this->obj = obj; }
};

#endif // OBJECTITEM_H
