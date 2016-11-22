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
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

public:
    QTFilterProxyModel(QObject *parent = 0);

    void setFilterRegExp(const boost::optional<std::regex> &filter);

    std::chrono::seconds get_total_time() const { return std::chrono::seconds(total_time); }
    QStandardItemModel* get_source_model() const { return static_cast<QStandardItemModel*>(sourceModel()); }
};

#endif // QTFILTERPROXYMODEL_H
