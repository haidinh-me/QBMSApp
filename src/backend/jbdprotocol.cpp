#include "jbdprotocol.h"
#include <QtGlobal>

QByteArray JbdProtocol::buildBasicInfoRequest() {
    return QByteArray::fromHex("DDA50300FFFD77");
}

QByteArray JbdProtocol::buildCellVoltagesRequest() {
    return QByteArray::fromHex("DDA50400FFFC77");
}

JbdProtocol::ParseResult JbdProtocol::parseFrame(const QByteArray &frame) {
    ParseResult result;

    if (frame.size() < 7 || static_cast<quint8>(frame[0]) != 0xDD
        || static_cast<quint8>(frame[frame.size() - 1]) != 0x77) {
        result.errorCode = ParseErrorCode::InvalidFormat;
        result.errorMessage = QStringLiteral("Invalid frame markers.");
        return result;
    }

    if (static_cast<quint8>(frame[2]) != 0x00) {
        result.errorCode = ParseErrorCode::BmsStatusError;
        result.errorMessage = QStringLiteral("BMS returned non-zero status.");
        return result;
    }

    const quint8 dataLength = static_cast<quint8>(frame[3]);
    const int expectedFrameLength = 7 + static_cast<int>(dataLength);
    if (frame.size() != expectedFrameLength) {
        result.errorCode = ParseErrorCode::LengthMismatch;
        result.errorMessage = QStringLiteral("Frame length mismatch.");
        return result;
    }

    if (!verifyChecksum(frame)) {
        result.errorCode = ParseErrorCode::ChecksumMismatch;
        result.errorMessage = QStringLiteral("Checksum mismatch.");
        return result;
    }

    const quint8 command = static_cast<quint8>(frame[1]);
    if (command == 0x03) return parseBasicInfoPayload(frame);
    if (command == 0x04) return parseCellVoltagesPayload(frame);

    result.errorCode = ParseErrorCode::UnsupportedCommand;
    result.errorMessage = QStringLiteral("Unsupported JBD command.");
    return result;
}

JbdProtocol::ParseResult JbdProtocol::parseBasicInfoPayload(const QByteArray &frame) {
    ParseResult result;

    const quint8 dataLength = static_cast<quint8>(frame[3]);
    if (dataLength < 23) {
        result.errorCode = ParseErrorCode::PayloadTooShort;
        result.errorMessage = QStringLiteral("Basic info payload too short.");
        return result;
    }

    const quint8 *payload = reinterpret_cast<const quint8 *>(frame.constData()) + 4;

    // for(int i = 0; i<frame.size(); ++i) {
    //     qDebug("Frame Basic Infor %d: 0x%02X", i, static_cast<quint8>(frame[i]));
    // }

    const quint16 rawVoltage = (static_cast<quint16>(payload[0]) << 8) | payload[1];
    const qint16 rawCurrent = static_cast<qint16>((static_cast<quint16>(payload[2]) << 8) | payload[3]);
    const quint16 rawRemainCapacity = (static_cast<quint16>(payload[4]) << 8) | payload[5];
    const quint16 rawDesignCapacity = (static_cast<quint16>(payload[6]) << 8) | payload[7];
    const quint16 cycleLife = (static_cast<quint16>(payload[8]) << 8) | payload[9];
    const quint16 protectionFlags = (static_cast<quint16>(payload[16]) << 8) | payload[17];
    const quint8 soc = payload[19];
    const quint8 cellCount = payload[21];
    const quint8 ntcCount = payload[22];

    result.snapshot.packVoltage = rawVoltage * 0.01;        // Đơn vị 10mV
    result.snapshot.current = rawCurrent * 0.01;            // Đơn vị 10mA
    result.snapshot.remainingCapacityAh = rawRemainCapacity * 0.01; // Đơn vị 10mAh
    result.snapshot.designCapacityAh = rawDesignCapacity * 0.01;    // Đơn vị 10mAh
    result.snapshot.cycleLife = cycleLife;
    result.snapshot.soc = soc;

    if (protectionFlags != 0) {
        result.snapshot.alarms.push_back(QStringLiteral("Protection active"));
    }

    if (ntcCount > 0 && dataLength >= 25) {
        const quint16 rawTemp = (static_cast<quint16>(payload[23]) << 8) | payload[24];
        result.snapshot.temperatureC = (rawTemp - 2731) * 0.1; // Đơn vị 0.1K tuyệt đối
    }

    if (cellCount > 0) {
        result.snapshot.cells.reserve(cellCount);
    }

    result.ok = true;
    result.frameType = FrameType::BasicInfo;
    result.errorCode = ParseErrorCode::None;
    return result;
}

JbdProtocol::ParseResult JbdProtocol::parseCellVoltagesPayload(const QByteArray &frame) {
    ParseResult result;

    const quint8 dataLength = static_cast<quint8>(frame[3]);
    if (dataLength < 2 || (dataLength % 2) != 0) {
        result.errorCode = ParseErrorCode::PayloadTooShort;
        result.errorMessage = QStringLiteral("Cell voltage payload length is invalid.");
        return result;
    }

    const quint8 *payload = reinterpret_cast<const quint8 *>(frame.constData()) + 4;
    const int cellCount = dataLength / 2;

    // for(int i = 0; i<frame.size(); ++i) {
    //     qDebug("Frame Cell Infor %d: 0x%02X", i, static_cast<quint8>(frame[i]));
    // }
    
    result.snapshot.cells.reserve(cellCount);
    for (int i = 0; i < cellCount; ++i) {
        const quint16 rawMv = (static_cast<quint16>(payload[i * 2]) << 8) | payload[i * 2 + 1];
        result.snapshot.cells.push_back(rawMv * 0.001); // Đơn vị mV
    }

    result.ok = true;
    result.frameType = FrameType::CellVoltages;
    result.errorCode = ParseErrorCode::None;
    return result;
}

bool JbdProtocol::verifyChecksum(const QByteArray &frame) {
    const int length = frame.size();
    const quint8 dataLength = static_cast<quint8>(frame[3]);

    quint16 sum = 0;
    for (int i = 2; i < 4 + dataLength; ++i) {
        sum += static_cast<quint8>(frame[i]);
    }

    const quint16 calculatedChecksum = 0x10000 - sum;
    const quint8 chkHigh = static_cast<quint8>(frame[length - 3]);
    const quint8 chkLow = static_cast<quint8>(frame[length - 2]);
    const quint16 receivedChecksum = (static_cast<quint16>(chkHigh) << 8) | chkLow;

    return calculatedChecksum == receivedChecksum;
}
