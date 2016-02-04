#include "model.h"
#include "modelitem.h"
#include <QDir>
#include <QDateTime>

Model::Model()
{
    m_rootItem = new ModelItem(QFileInfo("/"), new ModelItem(QFileInfo(), 0));
    QDir root("/");
    QFileInfoList drives = root.entryInfoList( QDir::AllEntries | QDir::Files
                                              | QDir::Hidden | QDir::System
                                              | QDir::NoDotAndDotDot);

    foreach (QFileInfo drive, drives)
    {
        new ModelItem(drive, m_rootItem);
    }
    m_rootItem->setSettle(true);
    m_rootItem = m_rootItem->parent();
    m_fileIconProvider = new QFileIconProvider;
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ModelItem * parentItem = static_cast<ModelItem*>(parent.internalPointer());
    if(!parentItem)
        parentItem = m_rootItem;

    ModelItem *childItem = parentItem->childAt(row);
    if(childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    ModelItem *childItem = static_cast<ModelItem*>(index.internalPointer());

    if(!childItem)
        return QModelIndex();

    ModelItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int Model::rowCount(const QModelIndex &parent) const
{
    ModelItem *item = static_cast<ModelItem*>(parent.internalPointer());
    if(item)
        return item->childCount();
    return m_rootItem->childCount();
}

int Model::columnCount(const QModelIndex &parent) const
{
    return (parent.column() > 0) ? 0 : 5;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());

    //Color
    if (role == Qt::ForegroundRole)
    {
        QFileInfo type(item->fileInfo());
        if(type.isHidden())
            return QColor(Qt::darkGray);
        else if(type.isSymLink())
            return QColor(Qt::darkYellow);
        else if(type.isDir())
            return QColor(Qt::black);
        else if(type.isExecutable())
            return QColor(Qt::darkBlue);
    }
    //Alignment
    else if (role == Qt::TextAlignmentRole)
    {
        if (index.column() == 1)
        {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
    }
    //file info
    else if (role == Qt::DisplayRole) {
      QVariant data;
      switch (index.column()) {
        case 0 :
          data = item->fileName();
          break;
        case 3 :
          data = item->fileInfo().lastModified().toString(Qt::LocalDate);
          break;
        case 4 : {
          if (item->m_permissions.isNull())
          {
            QString str;
            QFile::Permissions perms = item->fileInfo().permissions();
            str.append(perms.testFlag(QFileDevice::ReadOwner) ? "r" : "-" );
            str.append(perms.testFlag(QFileDevice::WriteOwner) ? "w" : "-" );
            str.append(perms.testFlag(QFileDevice::ExeOwner) ? "x" : "-" );
            str.append(perms.testFlag(QFileDevice::ReadGroup) ? "r" : "-" );
            str.append(perms.testFlag(QFileDevice::WriteGroup) ? "w" : "-" );
            str.append(perms.testFlag(QFileDevice::ExeGroup) ? "x" : "-" );
            str.append(perms.testFlag(QFileDevice::ReadOther) ? "r" : "-" );
            str.append(perms.testFlag(QFileDevice::WriteOther) ? "w" : "-" );
            str.append(perms.testFlag(QFileDevice::ExeOther) ? "x" : "-" );
            str.append(" " + item->fileInfo().owner() + " " +
                       item->fileInfo().group());
            item->m_permissions = str;
          }
          return item->m_permissions;
        }
        default :
          data = "";
          break;
      }
      return data;
    }
    //file name
    else if(role == Qt::EditRole)
    {
        return item->fileName();
    }
    //file icon
    else if (role == Qt::DecorationRole)
    {
        if (index.column() != 0)
            return QVariant();
        return getIcon(item);
    }
    return QVariant();
}

bool Model::isDirectory(const QModelIndex &index)
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());
    if(item && item != m_rootItem)
        return item->fileInfo().isDir();
    return false;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
        switch(section)
        {
            case 0: return tr("Name");
            case 1: return tr("Size");
            case 2: return tr("Type");
            case 4: return tr("Owner");
            case 3: return tr("Date Modified");
            default: return QVariant();
        }
    return QVariant();
}

QFileInfo Model::fileInfo(const QModelIndex &index)
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());

    if(item)
        return item->fileInfo();

    return QFileInfo();
}

bool Model::isDir(const QModelIndex &index)
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());

    if(item && item != m_rootItem)
        return item->fileInfo().isDir();

    return false;
}

QString Model::filePath(const QModelIndex &index)
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());

    if(item)
        return item->absoluteFilePath();

    return "";
}

QModelIndex Model::index(const QString &path) const
{
    ModelItem *item = m_rootItem->matchPath(path.split("/"),0);

    if(item)
        return createIndex(item->childNumber(),0,item);

    return QModelIndex();
}

bool Model::setRootPath(const QString &path)
{
    m_currentRootPath = path;
    ModelItem *item = m_rootItem->matchPath(path.split("/"));
    if(item && !item->isSettled())
        settleItem(item);
    return true;
}

int Model::size(const QModelIndex &index)
{
    ModelItem *item = static_cast<ModelItem*>(index.internalPointer());
    if(item)
        return item->fileInfo().size();
    return 0;
}

void Model::settleItem(ModelItem *item)
{
    item->setSettle(true);
    QDir dir(item->absoluteFilePath());
    QFileInfoList all = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    foreach(QFileInfo one, all)
        new ModelItem(one,item);
}

QVariant Model::getIcon(ModelItem *item) const
{
    QFileInfo type(item->fileInfo());
    return m_fileIconProvider->icon(type);
    //TODO: get more icons if these files have their own icons
    return QVariant();
}

bool Model::canFetchMore(const QModelIndex &parent) const
{
    ModelItem *item = static_cast<ModelItem*>(parent.internalPointer());

    if(item)
        if(item->isSettled())
            return false;

    return true;
}

