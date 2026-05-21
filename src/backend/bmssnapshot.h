#ifndef BMSSNAPSHOT_H
#define BMSSNAPSHOT_H

#include <QDateTime>
#include <QStringList>
#include <QVector>

/// @brief BmsSnapshot - Data structure containing BMS status snapshot
struct BmsSnapshot {
    QDateTime timestamp;
    double packVoltage = 0.0;
    double current = 0.0;
    int soc = 0;
    double temperatureC = 0.0;
    QStringList alarms;
    QVector<double> cells;
    double remainingCapacityAh = 0.0;
    double designCapacityAh = 0.0;
    int cycleLife = 0;
};

#endif // BMSSNAPSHOT_H
