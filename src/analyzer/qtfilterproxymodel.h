#ifndef QTFILTERPROXYMODEL_H
#define QTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <boost/optional.hpp>

#include <regex>

class QTFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    boost::optional<std::regex> filter_pattern;
    qlonglong total_time;

    void update_parents_values();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

public:
    QTFilterProxyModel(QObject *parent = 0);
    void setFilterRegExp(const boost::optional<std::regex> &filter);
    qlonglong get_total_time() const { return total_time; }
};

#endif // QTFILTERPROXYMODEL_H