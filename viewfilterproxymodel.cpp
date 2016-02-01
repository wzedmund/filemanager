#include "viewfilterproxymodel.h"
#include "model.h"


bool ViewFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(this->filterRegExp().isEmpty())
        return true;

    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    Model* fileModel = (Model*)(sourceModel());

    if(fileModel->fileInfo(index0).isHidden())
        return false;
    else
        return true;
}

