#include "objectitem.h"

ListItem::ListItem(const QString &text, QListWidget *parent, int type) : QListWidgetItem(text, parent, type) {

}

TreeItem::TreeItem(QTreeWidget *parent, const QStringList &strings, int type) : QTreeWidgetItem(parent, strings, type) {

}

TreeItem::TreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type) : QTreeWidgetItem(parent, strings, type) {

}
