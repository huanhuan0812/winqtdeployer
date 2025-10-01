#ifndef FOLDABLECHECKBOXWIDGET_H
#define FOLDABLECHECKBOXWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QList>
#include <QVariant>

class CheckBoxTree : public QTreeWidget
{
    Q_OBJECT

public:
    explicit CheckBoxTree(QWidget *parent = nullptr);
    
    // 添加项的方法
    QTreeWidgetItem* addTopLevelItem(const QString &text, const QVariant &data = QVariant());
    QTreeWidgetItem* addChildItem(QTreeWidgetItem *parent, const QString &text, const QVariant &data = QVariant());
    void addChildItem(QTreeWidgetItem *parent,QTreeWidgetItem *child);
    QTreeWidgetItem* insertChildItem(QTreeWidgetItem *parent, int index, const QString &text, const QVariant &data = QVariant());
    QTreeWidgetItem* createTreeItem(const QString &text, const QVariant &data);
    
    // 获取项的方法
    QList<QTreeWidgetItem*> getCheckedItems() const;
    QList<QTreeWidgetItem*> getPartiallyCheckedItems() const;
    QList<QVariant> getCheckedItemsData() const;
    
    // 项管理
    void removeItem(QTreeWidgetItem *item);
    void clearAllItems();

    
    // 状态设置
    void setItemChecked(QTreeWidgetItem *item, bool checked);
    bool isItemChecked(QTreeWidgetItem *item) const;
    
    // 查找项
    QTreeWidgetItem* findItemByText(const QString &text, Qt::MatchFlags flags = Qt::MatchExactly | Qt::MatchRecursive) const;
    QTreeWidgetItem* findItemByData(const QVariant &data, Qt::MatchFlags flags = Qt::MatchExactly | Qt::MatchRecursive) const;

signals:
    void itemCheckedStateChanged(QTreeWidgetItem *item, bool checked);
    void selectionChanged(const QList<QTreeWidgetItem*> &checkedItems);

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    void init();
    
    void updateChildItems(QTreeWidgetItem *parent, Qt::CheckState state);
    void updateParentItems(QTreeWidgetItem *item);
    void collectCheckedItemsRecursive(QTreeWidgetItem *parent, QList<QTreeWidgetItem*> &checkedItems, QList<QTreeWidgetItem*> &partialItems) const;
};

#endif // CheckBoxTree_H