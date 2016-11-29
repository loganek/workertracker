/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "drilldownslice.h"

#include "wtcommon/datetimeutils.h"

QT_CHARTS_USE_NAMESPACE

DrilldownSlice::DrilldownSlice(qreal value, QString prefix, int row)
    : m_prefix(prefix),
      row(row)
{
    setValue(value);
    updateLabel();
    setLabelFont(QFont("Arial", 8));
    connect(this, SIGNAL(percentageChanged()), this, SLOT(updateLabel()));
    connect(this, SIGNAL(hovered(bool)), this, SLOT(showHighlight(bool)));
}

void DrilldownSlice::updateLabel()
{
    QString label = QString::fromStdString(WT::time_to_display(std::chrono::seconds(qlonglong(this->value()))));
    label += " ";
    label += QString::number(this->percentage() * 100, 'f', 1);
    label += "%";
    label += ", ";
    label += m_prefix;
    setLabel(label);
}

void DrilldownSlice::showHighlight(bool show)
{
    setLabelVisible(show);
    setExploded(show);
}

#include "moc_drilldownslice.cpp"
