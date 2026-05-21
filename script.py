import struct
import serial

PORT = "COM10"
BAUD = 9600

REQ_BASIC = bytes.fromhex("DDA50300FFFD77")
REQ_CELLS = bytes.fromhex("DDA50400FFFC77")

def build_frame(cmd, payload: bytes) -> bytes:
    body = bytes([cmd, 0x00, len(payload)]) + payload
    csum = (0x10000 - (sum(body) & 0xFFFF)) & 0xFFFF
    return bytes([0xDD]) + body + csum.to_bytes(2, "big") + bytes([0x77])

def basic_payload():
    p = bytearray(25)

    pack_v = int(25.6 / 0.01)        # 2560
    current = int(23.5 / 0.01)      # -4020 (signed)
    remain = int(10.0 / 0.01)        # 1000
    design = int(12.0 / 0.01)        # 1200
    cycles = 100
    soc = 23
    temp_raw = int(34.5 / 0.1 + 2731)  # parser: (raw - 2731) * 0.1

    p[0:2] = pack_v.to_bytes(2, "big")
    p[2:4] = struct.pack(">h", current)
    p[4:6] = remain.to_bytes(2, "big")
    p[6:8] = design.to_bytes(2, "big")
    p[8:10] = cycles.to_bytes(2, "big")
    p[19] = soc
    p[20:22] = (0).to_bytes(2, "big")   # protection flags
    p[23:25] = temp_raw.to_bytes(2, "big")
    return bytes(p)

def cells_payload():
    cells_mv = [4200, 2912, 3820, 3199, 1230, 4020, 3520, 3197]
    p = bytearray()
    for mv in cells_mv:
        p += mv.to_bytes(2, "big")
    return bytes(p)

def extract_frames(buf: bytearray):
    frames = []
    while True:
        s = buf.find(b"\xDD")
        if s < 0:
            buf.clear()
            break
        if s > 0:
            del buf[:s]
        if len(buf) < 4:
            break
        ln = buf[3]
        need = 7 + ln
        if len(buf) < need:
            break
        f = bytes(buf[:need])
        del buf[:need]
        if f[-1] == 0x77:
            frames.append(f)
    return frames

with serial.Serial(PORT, BAUD, timeout=0.05) as ser:
    print("UART simulator started on", PORT, BAUD)
    rx = bytearray()
    while True:
        chunk = ser.read(ser.in_waiting or 1)
        if not chunk:
            continue
        rx += chunk
        for req in extract_frames(rx):
            if req == REQ_BASIC:
                resp = build_frame(0x03, basic_payload())
                ser.write(resp)
                print("-> BASIC", resp.hex(" ").upper())
            elif req == REQ_CELLS:
                resp = build_frame(0x04, cells_payload())
                ser.write(resp)
                print("-> CELLS", resp.hex(" ").upper())
            else:
                print("<- UNKNOWN", req.hex(" ").upper())