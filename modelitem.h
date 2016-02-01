#ifndef MODELITEM_H
#define MODELITEM_H

#include <QFileInfo>

class ModelItem
{
public:
    ModelItem(const QFileInfo& fileInfo, ModelItem* parent);
    ~ModelItem();

    ModelItem* childAt(int index);
    ModelItem* parent();
    ModelItem* matchPath(const QStringList& path, int startIndex = 0);

    QList<ModelItem*> children();

    int childCount() const;
    int childNumber() const;
    bool hasChild(QString fileName);

    QString absoluteFilePath() const;
    QString fileName() const;

    QFileInfo fileInfo() const;

    void refreshFileInfo();
    void addChild(ModelItem *child);
    void removeChild(ModelItem *child);
    void changeName(QString name);
    void clearAll();

    QString m_mimeType;
    QString m_permissions;

private:

    QList<ModelItem*> m_children;
    ModelItem* m_parent;
    QFileInfo m_fileInfo;
    QString m_absoluteFilePath;
};

#endif // MODELITEM_H
