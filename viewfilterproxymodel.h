#ifndef VIEWFILTERPROXYMODEL_H
#define VIEWFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class ViewFilterProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // VIEWFILTERPROXYMODEL_H
