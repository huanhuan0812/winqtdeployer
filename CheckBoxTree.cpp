#include "CheckBoxTree.h"
#include <QDebug>
#include <QHeaderView>

CheckBoxTree::CheckBoxTree(QWidget *parent)
    : QTreeWidget(parent)
{
    init();
}

void CheckBoxTree::init()
{
    // 设置树形控件属性
    setHeaderLabel("CheckBoxTree");
    setColumnCount(1);
    setIndentation(15);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    
    // 连接信号
    connect(this, &QTreeWidget::itemChanged, this, &CheckBoxTree::onItemChanged);
}

QTreeWidgetItem* CheckBoxTree::addTopLevelItem(const QString &text, const QVariant &data)
{
    QTreeWidgetItem *item = createTreeItem(text, data);
    QTreeWidget::addTopLevelItem(item);
    return item;
}

QTreeWidgetItem* CheckBoxTree::addChildItem(QTreeWidgetItem *parent, const QString &text, const QVariant &data)
{
    if (!parent) return nullptr;
    
    QTreeWidgetItem *item = createTreeItem(text, data);
    parent->addChild(item);
    return item;
}

void CheckBoxTree::addChildItem(QTreeWidgetItem* parent,QTreeWidgetItem* child){
    if(!parent)return;
    parent->addChild(child);
}

QTreeWidgetItem* CheckBoxTree::insertChildItem(QTreeWidgetItem *parent, int index, const QString &text, const QVariant &data)
{
    if (!parent) return nullptr;
    
    QTreeWidgetItem *item = createTreeItem(text, data);
    parent->insertChild(index, item);
    return item;
}

QTreeWidgetItem* CheckBoxTree::createTreeItem(const QString &text, const QVariant &data)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, text);
    
    if (data.isValid()) {
        item->setData(0, Qt::UserRole, data);
    }
    
    // 启用复选框
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);
    
    return item;
}

QList<QTreeWidgetItem*> CheckBoxTree::getCheckedItems() const
{
    QList<QTreeWidgetItem*> checkedItems;
    QList<QTreeWidgetItem*> partialItems; // 不需要，但函数参数需要
    collectCheckedItemsRecursive(invisibleRootItem(), checkedItems, partialItems);
    return checkedItems;
}

QList<QTreeWidgetItem*> CheckBoxTree::getPartiallyCheckedItems() const
{
    QList<QTreeWidgetItem*> checkedItems; // 不需要，但函数参数需要
    QList<QTreeWidgetItem*> partialItems;
    collectCheckedItemsRecursive(invisibleRootItem(), checkedItems, partialItems);
    return partialItems;
}

QList<QVariant> CheckBoxTree::getCheckedItemsData() const
{
    QList<QVariant> dataList;
    QList<QTreeWidgetItem*> checkedItems = getCheckedItems();
    
    for (QTreeWidgetItem *item : checkedItems) {
        QVariant data = item->data(0, Qt::UserRole);
        if (data.isValid()) {
            dataList.append(data);
        }
    }
    
    return dataList;
}

void CheckBoxTree::collectCheckedItemsRecursive(QTreeWidgetItem *parent, 
                                                         QList<QTreeWidgetItem*> &checkedItems,
                                                         QList<QTreeWidgetItem*> &partialItems) const
{
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *item = parent->child(i);
        Qt::CheckState state = item->checkState(0);
        
        if (state == Qt::Checked&& item->childCount()==0) {
            checkedItems.append(item);
        } else if (state == Qt::PartiallyChecked) {
            partialItems.append(item);
        }
        
        // 递归处理子项
        if (item->childCount() > 0) {
            collectCheckedItemsRecursive(item, checkedItems, partialItems);
        }
    }
}

void CheckBoxTree::removeItem(QTreeWidgetItem *item)
{
    if (!item) return;
    
    // 如果是顶级项
    if (item->parent() == nullptr) {
        int index = indexOfTopLevelItem(item);
        if (index != -1) {
            takeTopLevelItem(index);
            delete item;
        }
    } else {
        // 如果是子项
        QTreeWidgetItem *parent = item->parent();
        int index = parent->indexOfChild(item);
        if (index != -1) {
            parent->takeChild(index);
            delete item;
            // 更新父项状态
            updateParentItems(parent);
        }
    }
}

void CheckBoxTree::clearAllItems()
{
    clear();
}

void CheckBoxTree::setItemChecked(QTreeWidgetItem *item, bool checked)
{
    if (!item) return;
    
    blockSignals(true);
    item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
    updateChildItems(item, checked ? Qt::Checked : Qt::Unchecked);
    updateParentItems(item);
    blockSignals(false);
    
    emit selectionChanged(getCheckedItems());
}

bool CheckBoxTree::isItemChecked(QTreeWidgetItem *item) const
{
    if (!item) return false;
    return item->checkState(0) == Qt::Checked;
}

QTreeWidgetItem* CheckBoxTree::findItemByText(const QString &text, Qt::MatchFlags flags) const
{
    QList<QTreeWidgetItem*> items = findItems(text, flags);
    return items.isEmpty() ? nullptr : items.first();
}

QTreeWidgetItem* CheckBoxTree::findItemByData(const QVariant &data, Qt::MatchFlags flags) const
{
    QList<QTreeWidgetItem*> items = findItems("*", flags); // 匹配所有项
    for (QTreeWidgetItem *item : items) {
        if (item->data(0, Qt::UserRole) == data) {
            return item;
        }
    }
    return nullptr;
}

void CheckBoxTree::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return;
    
    // 阻止信号循环
    blockSignals(true);
    
    // 获取复选框状态
    Qt::CheckState state = item->checkState(0);
    
    // 更新子项状态
    updateChildItems(item, state);
    
    // 更新父项状态
    updateParentItems(item);
    
    // 恢复信号
    blockSignals(false);
    
    // 发射信号
    bool checked = (state == Qt::Checked);
    emit itemCheckedStateChanged(item, checked);
    emit selectionChanged(getCheckedItems());
}

void CheckBoxTree::updateChildItems(QTreeWidgetItem *parent, Qt::CheckState state)
{
    // 递归更新所有子项的状态
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *child = parent->child(i);
        child->setCheckState(0, state);
        updateChildItems(child, state);
    }
}

void CheckBoxTree::updateParentItems(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;
    
    int checkedCount = 0;
    int uncheckedCount = 0;
    int partialCount = 0;
    
    // 统计子项的选中状态
    for (int i = 0; i < parent->childCount(); ++i) {
        Qt::CheckState state = parent->child(i)->checkState(0);
        if (state == Qt::Checked)
            checkedCount++;
        else if (state == Qt::Unchecked)
            uncheckedCount++;
        else if (state == Qt::PartiallyChecked)
            partialCount++;
    }
    
    // 根据子项状态设置父项状态
    Qt::CheckState newState;
    if (checkedCount == parent->childCount()) {
        newState = Qt::Checked;
    } else if (uncheckedCount == parent->childCount()) {
        newState = Qt::Unchecked;
    } else {
        newState = Qt::PartiallyChecked;
    }
    
    // 只有当状态确实改变时才更新，避免无限循环
    if (parent->checkState(0) != newState) {
        parent->setCheckState(0, newState);
    }
    
    // 递归更新父项
    updateParentItems(parent);
}