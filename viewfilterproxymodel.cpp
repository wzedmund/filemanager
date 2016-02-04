#include "viewfilterproxymodel.h"
#include "model.h"


bool ViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    Model* fileModel = dynamic_cast<Model*>(sourceModel());

    if(fileModel->fileInfo(index0).isHidden())
        return false;
    else
        return true;
}

