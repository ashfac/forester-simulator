#ifndef SUBARUFORESTER_H
#define SUBARUFORESTER_H

#include <QTextStream>
#include <functional>
#include <vector>

#include "subaru.h"
#include "ecu.h"
#include "dlc.h"
#include "ecm.h"
#include "tcm.h"

class SubaruForester
{
public:
    SubaruForester();

    const std::shared_ptr<CanBus> get_can_bus() const { return m_can_bus; };

    const std::shared_ptr<DLC> get_dlc() const { return m_dlc; };
    const std::shared_ptr<ECM> get_ecm() const { return m_ecm; };
    const std::shared_ptr<TCM> get_tcm() const { return m_tcm; };

private:
    const std::shared_ptr<CanBus> m_can_bus = nullptr;

    const std::shared_ptr<DLC> m_dlc = nullptr;
    const std::shared_ptr<ECM> m_ecm = nullptr;
    const std::shared_ptr<TCM> m_tcm = nullptr;
};

#endif // SUBARUFORESTER_H
