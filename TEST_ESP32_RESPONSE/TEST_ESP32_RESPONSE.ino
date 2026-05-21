#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void fillJBDChecksum(uint8_t *data, int len) {
  uint16_t sum = 0;
  for (int i = 2; i <= len - 4; i++) {
    sum += data[i];
  }
  uint16_t checksum = 0x10000 - sum;
  data[len - 3] = highByte(checksum);
  data[len - 2] = lowByte(checksum);
}

void sendJBDBasicInfo() {
  // Gói tin 34 bytes (Data Length = 27)
  uint8_t res[34] = {0}; 
  res[0] = 0xDD;
  res[1] = 0x03;
  res[2] = 0x00;
  res[3] = 27;   // Length = 23 (cơ bản) + (2 NTC * 2 byte) = 27

  // Randomize dữ liệu cơ bản
  uint16_t volt = random(2000, 3000);  // Tổng áp: 60.00V - 70.00V
  uint16_t curr = random(-1000, 2000);   // Dòng điện: 1.00A - 30.00A
  uint16_t rcap = random(1000, 4000);  // Dung lượng còn lại: 10.00Ah - 40.00Ah
  uint16_t dcap = 4000;                // Dung lượng thiết kế thường cố định (40.00Ah)
  uint16_t cyc  = random(10, 150);     // Số chu kỳ sạc/xả: 10 - 150
  uint16_t date = 0x2491;              // Ngày SX thường cố định
  
  res[4] = highByte(volt); res[5] = lowByte(volt); 
  res[6] = highByte(curr); res[7] = lowByte(curr); 
  res[8] = highByte(rcap); res[9] = lowByte(rcap); 
  res[10] = highByte(dcap); res[11] = lowByte(dcap); 
  res[12] = highByte(cyc); res[13] = lowByte(cyc); 
  res[14] = highByte(date); res[15] = lowByte(date); 
  
  // Balance Status (4 bytes: Low & High)
  res[16] = 0; res[17] = 0; res[18] = 0; res[19] = 0; 
  
  // Protection Status (0 = Không lỗi)
  res[20] = 0; res[21] = 0; 
  
  res[22] = 0x12; // Version 
  res[23] = random(10, 101); // RSOC (% Pin) = 10% - 100%
  res[24] = 0x03; // FET Status (Bật cả sạc/xả) 
  res[25] = 7;    // Số lượng Cell = 7 
  res[26] = 2;    // Số lượng NTC = 2 
  
  // Nhiệt độ NTC 1 & NTC 2 (Random từ 25.0°C đến 45.0°C -> 2731 + 250...450 = 2981...3181)
  uint16_t t1 = random(2981, 3182); 
  res[27] = highByte(t1); res[28] = lowByte(t1); 
  
  uint16_t t2 = random(2981, 3182); 
  res[29] = highByte(t2); res[30] = lowByte(t2);

  res[33] = 0x77;
  fillJBDChecksum(res, 34);
  Serial2.write(res, 34);
  Serial.println(">>> Sent JBD Basic Info (0x03) - Random Data");
}

// Giả lập lệnh 0x04 (Cell Info)
void sendJBDCellInfo() {
  uint8_t cells = 7; // 7 cell 
  uint8_t dataLen = cells * 2;
  int frameLen = 7 + dataLen; 
  
  uint8_t res[100] = {0};
  res[0] = 0xDD;
  res[1] = 0x04;
  res[2] = 0x00; // Status
  res[3] = dataLen;
  
  // Tạo điện áp random cho từng cell
  for(int i = 0; i < cells ; i++) {
    // Điện áp cell random từ 3.200V đến 4.200V (3200 - 4200 mV)
    uint16_t cellV = random(100, 4201); 
    res[4 + (i * 2)] = highByte(cellV);
    res[5 + (i * 2)] = lowByte(cellV);
  }

  res[frameLen - 1] = 0x77;
  fillJBDChecksum(res, frameLen);
  Serial2.write(res, frameLen);
  Serial.println(">>> Sent JBD Cell Info (0x04) - Random Data");
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  // Khởi tạo seed cho hàm random từ chân analog chưa được kết nối
  // Giúp các lần reset board sẽ ra chuỗi số random khác nhau
  randomSeed(analogRead(0)); 
  
  Serial.println("JBD BMS Simulator V4 (Randomized) Ready...");
}

void loop() {
  if (Serial2.available() >= 7) {
    uint8_t buf[7];
    if (Serial2.peek() == 0xDD) {
      Serial2.readBytes(buf, 7);
      if (buf[1] == 0xA5) {
        uint8_t cmd = buf[2];
        if (cmd == 0x03) sendJBDBasicInfo();
        else if (cmd == 0x04) sendJBDCellInfo();
      }
    } else {
      Serial2.read();
    }
  }
}