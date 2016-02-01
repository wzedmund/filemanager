#include "mainwindow.h"
#include "model.h"
#include "treefilterproxymodel.h"
#include "viewfilterproxymodel.h"
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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

    m_treeModel = new TreeFilterProxyModel;
    m_treeModel->setSourceModel(m_model);
    m_treeModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    m_viewModel = new ViewFilterProxyModel;
    m_viewModel->setSourceModel(m_model);
    m_viewModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    m_treeView->setModel(m_treeModel);
    m_treeView->setHeaderHidden(true);
    m_treeView->setUniformRowHeights(true);

    m_listView->setWrapping(true);
    m_listView->setModel(m_viewModel);

    m_listViewSelectionModel = m_listView->selectionModel();

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
    show();
}

MainWindow::~MainWindow()
{

}

void MainWindow::treeViewSelectionChanged(QModelIndex current, QModelIndex previous)
{
    QFileInfo name = m_model->fileInfo(m_treeModel->mapToSource(current));
    if(!name.exists()) return;

    m_currentIndex = name;
    setWindowTitle(m_currentIndex.fileName());

    if(m_treeView->hasFocus() && QApplication::mouseButtons() == Qt::MidButton)
    {
        listViewPressed(m_viewModel->mapFromSource(m_model->index(name.filePath())));
        m_listView->setFocus(Qt::TabFocusReason);
    }


    if(m_model->setRootPath(name.filePath()))
        m_viewModel->invalidate();

    QModelIndex baseIndex = m_viewModel->mapFromSource(m_model->index(name.filePath()));
    m_listView->setRootIndex(baseIndex);

    m_listViewSelectionModel->blockSignals(0);
    QTimer::singleShot(30,this,SLOT(dirLoaded()));
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
    QModelIndex srcIndex = m_viewModel->mapToSource(index);

    if (run)
    {
        QProcess *process = new QProcess(this);
        process->startDetached(m_model->filePath(srcIndex));
    }
}

void MainWindow::directoryLoaded()
{

}

