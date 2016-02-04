#include "modelcompleter.h"

ModelCompleter::ModelCompleter()
{

}

QStringList ModelCompleter::splitPath(const QString &path) const
{
    QStringList list = path.split("/");
    list[0] = "/";
    return list;
}

QString ModelCompleter::pathFromIndex(const QModelIndex &index) const
{
    if(!index.isValid())
        return "";

    QModelIndex i = index;
    QStringList list;
    do
    {
        QString t = model()->data(i, Qt::EditRole).toString();
        list.prepend(t);
        QModelIndex parent = i.parent();
        i = parent.sibling(parent.row(), index.column());
    }
    while (i.isValid());
    list[0].clear() ;
    return list.join("/");
}

