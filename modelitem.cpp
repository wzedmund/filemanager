#include "modelitem.h"

ModelItem::ModelItem(const QFileInfo& fileInfo, ModelItem* parent)
{
    m_parent = parent;
    m_fileInfo = fileInfo;

    if(parent)
    {
          parent->addChild(this);
          m_absoluteFilePath = fileInfo.filePath();
    }
    else
    {
        m_absoluteFilePath = "";
    }
}

ModelItem::~ModelItem()
{
    qDeleteAll(m_children);
}


ModelItem *ModelItem::childAt(int index)
{
    return m_children.at(index);
}

ModelItem *ModelItem::parent()
{
    return m_parent;
}

ModelItem *ModelItem::matchPath(const QStringList &path, int startIndex)
{
    QStringList temp = path;
    temp.replace(0,"/");
    temp.removeAll("");

    foreach(ModelItem* child, m_children)
    {
        QString match = temp.at(startIndex);

        if(child->fileName() == match)
        {
            if(startIndex + 1 == temp.count())
                return child;
            else
                return child->matchPath(path, startIndex + 1);
        }
    }
    return NULL;
}

QList<ModelItem *> ModelItem::children()
{
    return m_children;
}

int ModelItem::childCount() const
{
    return m_children.size();
}

int ModelItem::childNumber() const
{
    if (m_parent != NULL)
        return m_parent->children().indexOf(const_cast<ModelItem*>(this));
    return 0;
}

bool ModelItem::hasChild(QString fileName)
{
    foreach(ModelItem * item, m_children)
        if(item->fileName() == fileName)
            return true;
    return false;
}

QString ModelItem::absoluteFilePath() const
{
    return m_absoluteFilePath;
}

QString ModelItem::fileName() const
{
    if(m_absoluteFilePath == "/")
        return "/";
    else
        return m_fileInfo.fileName();
}

QFileInfo ModelItem::fileInfo() const
{
    return m_fileInfo;
}

void ModelItem::refreshFileInfo()
{
    m_fileInfo.refresh();
    m_permissions.clear();
}

void ModelItem::addChild(ModelItem *child)
{
    if(!m_children.contains(child))
        m_children.append(child);
}

void ModelItem::removeChild(ModelItem *child)
{
    m_children.removeOne(child);
}

void ModelItem::changeName(QString name)
{
    m_absoluteFilePath = m_parent->absoluteFilePath() + "/" + name;
    m_fileInfo.setFile(m_absoluteFilePath);
    clearAll();
}

void ModelItem::clearAll()
{
    foreach(ModelItem *child, m_children)
        delete child;
    m_children.clear();
}
