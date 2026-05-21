#ifndef JBDPROTOCOL_H
#define JBDPROTOCOL_H

#include <QByteArray>
#include <QString>

#include "bmssnapshot.h"

/// @brief JbdProtocol - JBD BMS protocol encoder/decoder
class JbdProtocol
{
public:
    enum class FrameType {
        Unknown = 0,        ///< Unknown frame type
        BasicInfo,          ///< Basic battery information
        CellVoltages        ///< Individual cell voltages
    };

    enum class ParseErrorCode {
        None = 0,                  ///< No error
        InvalidFormat,             ///< Invalid frame format
        BmsStatusError,            ///< BMS device status error
        LengthMismatch,            ///< Frame length mismatch
        ChecksumMismatch,          ///< Checksum verification failed
        UnsupportedCommand,        ///< Unsupported command
        PayloadTooShort            ///< Payload data too short
    };

    struct ParseResult {
        bool ok = false;                                ///< Parse successful
        FrameType frameType = FrameType::Unknown;       ///< Frame type
        ParseErrorCode errorCode = ParseErrorCode::None; ///< Error code if failed
        QString errorMessage;                           ///< Detailed error message
        BmsSnapshot snapshot;                           ///< Parsed BMS data
    };

    /// @brief Build basic info request command
    static QByteArray buildBasicInfoRequest();
    
    /// @brief Build cell voltages request command
    static QByteArray buildCellVoltagesRequest();
    
    /// @brief Parse received frame
    static ParseResult parseFrame(const QByteArray &frame);

private:
    static bool verifyChecksum(const QByteArray &frame);
    static ParseResult parseBasicInfoPayload(const QByteArray &frame);
    static ParseResult parseCellVoltagesPayload(const QByteArray &frame);
};

#endif // JBDPROTOCOL_H
