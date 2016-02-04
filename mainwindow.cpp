#include "mainwindow.h"
#include "model.h"
#include "treefilterproxymodel.h"
#include "viewfilterproxymodel.h"
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QTabBar>
#include <QtConcurrent/QtConcurrent>
#include <QDirModel>
#include "searchbar.h"
#include "addresstoolbar.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(800, 550);
    //set the current path as start address
    m_startPath = QDir::currentPath();
    m_model = new Model;
    m_dockTree = new QDockWidget("", this, Qt::SubWindow);
    m_treeView = new QTreeView(m_dockTree);
    m_dockTree->setWidget(m_treeView);
    addDockWidget(Qt::LeftDockWidgetArea, m_dockTree);

    QWidget * mainWidget = new QWidget;
    m_vBoxLayout = new QVBoxLayout(mainWidget);
    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(0,0,0,0);

    m_stackedWidget = new QStackedWidget();
    QWidget * pageWidget = new QWidget();
    QHBoxLayout * localHBoxLayout = new QHBoxLayout(pageWidget);
    localHBoxLayout->setSpacing(0);
    localHBoxLayout->setContentsMargins(0,0,0,0);
    m_listView = new QListView(pageWidget);
    localHBoxLayout->addWidget(m_listView);
    m_stackedWidget->addWidget(pageWidget);

    QWidget *pageWidget1 = new QWidget();
    QHBoxLayout * localHBoxLayout1 = new QHBoxLayout(pageWidget1);
    localHBoxLayout1->setSpacing(0);
    localHBoxLayout1->setContentsMargins(0,0,0,0);
    m_detailTreeView = new QTreeView(pageWidget1);
    localHBoxLayout1->addWidget(m_detailTreeView);
    m_stackedWidget->addWidget(pageWidget1);
    m_vBoxLayout->addWidget(m_stackedWidget);
    setCentralWidget(mainWidget);

    //create the proxy model for the tree view
    m_treeModel = new TreeFilterProxyModel;
    m_treeModel->setSourceModel(m_model);
    m_treeModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    //create the proxy model for the main view
    m_viewModel = new ViewFilterProxyModel;
    m_viewModel->setSourceModel(m_model);
    m_viewModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    //setup model for tree view
    m_treeView->setModel(m_treeModel);
    m_treeView->setHeaderHidden(true);
    m_treeView->setUniformRowHeights(true);
    m_treeView->hideColumn(1);
    m_treeView->hideColumn(2);
    m_treeView->hideColumn(3);
    m_treeView->hideColumn(4);


    //setup model for list view
    m_listView->setWrapping(true);
    m_listView->setModel(m_viewModel);
    m_listViewSelectionModel = m_listView->selectionModel();

    //setup model for detailed tree view(stacked in the main view)
    m_detailTreeView->setRootIsDecorated(false);
    m_detailTreeView->setItemsExpandable(false);
    m_detailTreeView->setUniformRowHeights(true);
    m_detailTreeView->setModel(m_viewModel);
    m_detailTreeView->setSelectionModel(m_listViewSelectionModel);
    m_treeViewSelectionModel = m_treeView->selectionModel();


    connect(m_treeViewSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(treeViewSelectionChanged(QModelIndex,QModelIndex)));
    m_treeView->setCurrentIndex(m_treeModel->mapFromSource(m_model->index(m_startPath)));
    m_treeView->scrollTo(m_treeView->currentIndex());

    //create tool bar for editing address
    m_pathEdit = new AddressToolBar;
    m_addressBar = addToolBar("Address");
    m_addressBar->addWidget(m_pathEdit);

    connect(m_pathEdit, SIGNAL(activated(QString)), this, SLOT(pathEditChanged(QString)));

    connect(m_listViewSelectionModel,
            SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
            this, SLOT(listViewSelectionChanged(const QItemSelection,
                                            const QItemSelection)));

    connect(m_listView, SIGNAL(activated(QModelIndex)),
            this, SLOT(listViewDoubleClicked(QModelIndex)));
    show();
    m_treeView->setDragDropMode(QAbstractItemView::DragDrop);
    m_treeView->setDefaultDropAction(Qt::MoveAction);
    m_treeView->setDropIndicatorShown(true);
    m_treeView->setEditTriggers(QAbstractItemView::EditKeyPressed |
                          QAbstractItemView::SelectedClicked);
}

MainWindow::~MainWindow()
{

}

void MainWindow::treeViewSelectionChanged(QModelIndex current, QModelIndex previous)
{
    QFileInfo name = m_model->fileInfo(m_treeModel->mapToSource(current));
    if(!name.exists()) return;

    m_currentFileInfo = name;
    setWindowTitle(m_currentFileInfo.fileName());

    if(m_treeView->hasFocus() && QApplication::mouseButtons() == Qt::MidButton)
    {
        listViewPressed(m_viewModel->mapFromSource(m_model->index(name.filePath())));
        m_listView->setFocus(Qt::TabFocusReason);
    }

    if(m_currentFileInfo.filePath() != m_pathEdit->itemText(0))
    {
        m_pathEdit->insertItem(0, m_currentFileInfo.filePath());
        m_pathEdit->setCurrentIndex(0);
        m_pathEdit->setTabBarText(m_currentFileInfo.filePath());
    }

    if(m_model->setRootPath(name.filePath()))
        m_viewModel->invalidate();

    QModelIndex baseIndex = m_viewModel->mapFromSource(m_model->index(name.filePath()));
    m_listView->setRootIndex(baseIndex);

    if(m_backIndex.isValid())
    {
        m_listViewSelectionModel->setCurrentIndex(m_treeModel->mapFromSource(m_backIndex),
                                                  QItemSelectionModel::ClearAndSelect);
        m_listView->scrollTo(m_treeModel->mapFromSource(m_backIndex));
    }
    else
    {
        m_listViewSelectionModel->blockSignals(1);
        m_listViewSelectionModel->clear();
    }

    m_listViewSelectionModel->blockSignals(0);
    QTimer::singleShot(30,this,SLOT(directoryChanged()));
}

void MainWindow::listViewPressed(QModelIndex current)
{
    if(QApplication::mouseButtons() == Qt::MidButton)
        if(m_model->isDir(m_viewModel->mapToSource(current)))
        {
            if(QApplication::keyboardModifiers() == Qt::ControlModifier)
                openFile();
        }
        else
            openFile();
}

void MainWindow::openFile()
{
    QModelIndexList items;
    if (m_listViewSelectionModel->selectedRows(0).count())
    {
        items = m_listViewSelectionModel->selectedRows(0);
    }
    else
    {
        items = m_listViewSelectionModel->selectedIndexes();
    }

    foreach (QModelIndex index, items)
    {
        executeFile(index, 0);
    }
}

void MainWindow::executeFile(QModelIndex index, bool run)
{
    qDebug() << "Execute File";
    QModelIndex srcIndex = m_viewModel->mapToSource(index);

    if (run)
    {
        qDebug() << "Execute Filepath = " << m_model->filePath(srcIndex);
        QProcess *process = new QProcess(this);
        process->startDetached(m_model->filePath(srcIndex));
    }
}

void MainWindow::directoryChanged()
{
    qDebug() << "Directory Changed";
    if(m_backIndex.isValid())
    {
        m_backIndex = QModelIndex();
        return;
    }

    qint64 bytes = 0;
    QModelIndexList items;
    bool includeHidden = true;

    for(int x = 0; x < m_model->rowCount(m_model->index(m_pathEdit->currentText())); ++x)
        items.append(m_model->index(x,0,m_model->index(m_pathEdit->currentText())));


    foreach(QModelIndex item,items)
    {
        if(includeHidden || !m_model->fileInfo(item).isHidden())
            bytes = bytes + m_model->size(item);
        else
            items.removeOne(item);
    }
}

void MainWindow::pathEditChanged(QString path)
{
    qDebug() << "Path Eidt Changed" << path;
    QString info = path;
    if (!QFileInfo(path).exists())
        return;
    info.replace("~",QDir::homePath());
    qDebug() << "info" << info;
    m_treeView->setCurrentIndex(m_treeModel->mapFromSource(m_model->index(info)));
}

void MainWindow::listViewSelectionChanged(const QItemSelection selected, const QItemSelection deselected)
{
    qDebug() << "list View Selection Changed";
    QModelIndexList items;

    if(m_listViewSelectionModel->selectedRows(0).count())
        items = m_listViewSelectionModel->selectedRows(0);
    else
        items = m_listViewSelectionModel->selectedIndexes();

    if(items.count() == 0)
    {
        m_currentFileInfo = m_pathEdit->itemText(0);
        return;
    }

    m_currentFileInfo = m_model->fileInfo(m_viewModel->mapToSource(m_listViewSelectionModel->currentIndex()));

    qint64 bytes = 0;
    int folders = 0;
    int files = 0;

    foreach(QModelIndex item, items)
    {
        if(m_model->isDir(m_viewModel->mapToSource(item)))
            folders++;
        else
            files++;
        bytes = bytes + m_model->size(m_viewModel->mapToSource(item));
    }

    m_model->filePath(m_viewModel->mapToSource(items.at(0)));
}

void MainWindow::listViewDoubleClicked(QModelIndex current)
{
    Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
    if (mods == Qt::ControlModifier || mods == Qt::ShiftModifier)
    {
        return;
    }
    if (m_model->isDir(m_viewModel->mapToSource(current)))
    {
        QModelIndex i = m_viewModel->mapToSource(current);
        m_treeView->setCurrentIndex(m_treeModel->mapFromSource(i));
    }
    else
    {
        //TODO: directly run it or open it with selected application
        executeFile(current, true);
    }
}
