/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "selectinstrumentscenario.h"

using namespace mu::instrumentsscene;
using namespace mu::notation;

mu::RetVal<PartInstrumentListScoreOrder> SelectInstrumentsScenario::selectInstruments() const
{
    QStringList params {
        "canSelectMultipleInstruments=true"
    };

    return selectInstruments(params);
}

mu::RetVal<Instrument> SelectInstrumentsScenario::selectInstrument(const InstrumentKey& currentInstrumentKey) const
{
    RetVal<Instrument> result;

    QStringList params {
        "canSelectMultipleInstruments=false",
        "currentInstrumentId=" + currentInstrumentKey.instrumentId
    };

    RetVal<PartInstrumentListScoreOrder> selectedInstruments = selectInstruments(params);
    if (!selectedInstruments.ret) {
        result.ret = selectedInstruments.ret;
        return result;
    }

    result.ret = make_ret(Ret::Code::Ok);

    if (selectedInstruments.val.instruments.empty()) {
        return result;
    }

    const InstrumentTemplate& templ = selectedInstruments.val.instruments.first().instrumentTemplate;
    result.val = Instrument::fromTemplate(&templ);

    return result;
}

mu::RetVal<PartInstrumentListScoreOrder> SelectInstrumentsScenario::selectInstruments(const QStringList& params) const
{
    RetVal<PartInstrumentListScoreOrder> result;

    QString uri = QString("musescore://instruments/select?%1").arg(params.join('&'));
    RetVal<Val> retVal = interactive()->open(uri.toStdString());
    if (!retVal.ret) {
        result.ret = retVal.ret;
        return result;
    }

    result.ret = make_ret(Ret::Code::Ok);

    ValMap content = retVal.val.toMap();

    ValMap order = content["scoreOrder"].toMap();
    result.val.scoreOrder = instrumentsRepository()->order(order["id"].toString());
    result.val.scoreOrder.customized = order["customized"].toBool();

    for (const Val& obj: content["instruments"].toList()) {
        ValMap map = obj.toMap();
        PartInstrument pi;

        pi.partId = ID(map["partId"].toString());
        pi.isExistingPart = map["isExistingPart"].toBool();
        pi.isSoloist = map["isSoloist"].toBool();

        std::string instrumentId = map["instrumentId"].toString();
        pi.instrumentTemplate = instrumentsRepository()->instrumentTemplate(instrumentId);

        result.val.instruments << pi;
    }

    return result;
}
