#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>

class ModelItem;

class Model : public QAbstractItemModel
{
public:
    Model();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    bool isDirectory(const QModelIndex &index);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QFileInfo fileInfo(const QModelIndex &index);
    bool isDir(const QModelIndex &index);
    QString filePath(const QModelIndex &index);
    QModelIndex index(const QString& path) const;
    bool setRootPath(const QString& path);
private:
    ModelItem* m_rootItem;
    QString m_currentRootPath;
};

#endif // ITEMMODEL_H
