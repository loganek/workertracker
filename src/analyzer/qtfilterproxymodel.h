/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef QTFILTERPROXYMODEL_H
#define QTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include <boost/optional.hpp>

#include <chrono>
#include <regex>

class QTFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    boost::optional<std::regex> filter_pattern;
    qlonglong total_time;

    void update_parents_values();
    void setSourceModel(QAbstractItemModel *sourceModel) Q_DECL_OVERRIDE { QSortFilterProxyModel::setSourceModel(sourceModel); }

protected:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

public:
    virtual ~QTFilterProxyModel();
    QTFilterProxyModel(QObject *parent = 0);

    void setFilterRegExp(const boost::optional<std::regex> &filter);

    std::chrono::seconds get_total_time() const { return std::chrono::seconds(total_time); }
    QStandardItemModel* get_source_model() const { return static_cast<QStandardItemModel*>(sourceModel()); }
};

#endif // QTFILTERPROXYMODEL_H
