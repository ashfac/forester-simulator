#ifndef SUBARUFORESTER_H
#define SUBARUFORESTER_H

#include <QTextStream>
#include <functional>
#include <map>

#include "ecu.h"
#include "dlc.h"
#include "ecm.h"
#include "tcm.h"

#include "obd2.h"

class SubaruForester
{
public:
    SubaruForester();

    const std::shared_ptr<CanBus> get_can_bus() const;

    const std::shared_ptr<DLC> get_dlc() const;
    const std::shared_ptr<ECM> get_ecm() const;
    const std::shared_ptr<TCM> get_tcm() const;

private:
    const std::shared_ptr<CanBus> m_can_bus = nullptr;

    const std::map<const can::can_id_t, const std::shared_ptr<ECU>> m_ecus;
};

#endif // SUBARUFORESTER_H
