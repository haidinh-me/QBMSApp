#include <SoftwareSerial.h>

const int bmsRxPin = 10; 
const int bmsTxPin = 11; 
SoftwareSerial bmsSerial(bmsRxPin, bmsTxPin);

// --- CÁC LỆNH YÊU CẦU DỮ LIỆU (Theo chuẩn JBD BMS) ---
// Lệnh đọc Basic Info (Thanh ghi 0x03)
byte cmdBasicInfo[] = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77}; 
// Lệnh đọc Cell Info (Thanh ghi 0x04)
byte cmdCellInfo[] =  {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77};

// Mảng đệm (Buffer) để lưu gói tin BMS trả về
byte rxBuffer[100]; 
int rxLength = 0;

void setup() {
  Serial.begin(9600);
  bmsSerial.begin(9600); // Tốc độ baud mặc định của JBD BMS thường là 9600
  Serial.println("Khởi động hệ thống đọc BMS...");
  delay(1000);
}

void loop() {
  // 1. Gửi lệnh yêu cầu Basic Info và giải mã
  Serial.println("--- ĐANG ĐỌC BASIC INFO ---");
  sendRequest(cmdBasicInfo, sizeof(cmdBasicInfo));
  if (receiveResponse()) {
    parseBasicInfo();
  }
  
  delay(1000); // Nghỉ 1 giây trước khi hỏi thông tin tiếp theo

  // 2. Gửi lệnh yêu cầu Cell Info và giải mã
  Serial.println("--- ĐANG ĐỌC CELL INFO ---");
  sendRequest(cmdCellInfo, sizeof(cmdCellInfo));
  if (receiveResponse()) {
    parseCellInfo();
  }

  delay(3000); // Lặp lại sau 3 giây
}

// Hàm gửi mảng byte lệnh đi
void sendRequest(byte* command, int length) {
  // Xóa sạch buffer nhận trước khi gửi lệnh mới để tránh nhiễu
  while (bmsSerial.available()) { bmsSerial.read(); } 
  
  for (int i = 0; i < length; i++) {
    bmsSerial.write(command[i]);
  }
}

// Hàm nhận dữ liệu và lưu vào rxBuffer
bool receiveResponse() {
  rxLength = 0;
  unsigned long startTime = millis();
  
  // Đợi phản hồi trong tối đa 500ms (Timeout)
  while (millis() - startTime < 500) {
    if (bmsSerial.available()) {
      delay(20); // Đợi một chút để toàn bộ gói tin truyền tới buffer của Arduino
      
      while (bmsSerial.available() && rxLength < 100) {
        rxBuffer[rxLength] = bmsSerial.read();
        Serial.print(rxBuffer[rxLength]);
        Serial.print(" ");
        rxLength++;
        delay(2);
      }
      return true; // Đã nhận xong một gói tin
    }
  }

  Serial.print("\n");
  
  Serial.println("Lỗi: Không nhận được phản hồi (Timeout)!");
  return false;
}

// Hàm giải mã Basic Info (Theo cấu trúc phản hồi thanh ghi 0x03 của JBD)
void parseBasicInfo() {
  // Gói tin phản hồi hợp lệ của JBD thường bắt đầu bằng 0xDD và byte thứ 2 là thanh ghi
  if (rxBuffer[0] == 0xDD && rxBuffer[1] == 0x03) {
    
    // Điện áp tổng: byte 4 và 5 (Đơn vị: 10mV)
    // Dịch bit: Lấy byte cao dịch trái 8 bit, rồi OR (|) với byte thấp
    uint16_t rawVoltage = (rxBuffer[4] << 8) | rxBuffer[5];
    float totalVoltage = rawVoltage / 100.0; // Chuyển sang Volt
    
    // Dòng điện: byte 6 và 7 (Đơn vị: 10mA). Trừ đi 30000 theo chuẩn JBD đối với dòng âm/dương
    // Để đơn giản, ta xem như nó là số nguyên có dấu 16-bit
    int16_t rawCurrent = (rxBuffer[6] << 8) | rxBuffer[7];
    float current = rawCurrent / 100.0; // Chuyển sang Ampere
    
    // Dung lượng còn lại (Remaining Capacity): byte 8 và 9 (Đơn vị: 10mAh)
    uint16_t rawCapacity = (rxBuffer[8] << 8) | rxBuffer[9];
    float capacity = rawCapacity / 100.0; // Chuyển sang Ah
    
    // Phần trăm Pin (SOC): byte 23
    byte soc = rxBuffer[23];

    Serial.print("Điện áp tổng: "); Serial.print(totalVoltage); Serial.println(" V");
    Serial.print("Dòng điện: "); Serial.print(current); Serial.println(" A");
    Serial.print("Dung lượng còn: "); Serial.print(capacity); Serial.println(" Ah");
    Serial.print("Phần trăm Pin (SOC): "); Serial.print(soc); Serial.println(" %");
    Serial.println();
    
  } else {
    Serial.println("Gói tin Basic Info không hợp lệ hoặc sai Header.");
  }
}

// Hàm giải mã Cell Info (Theo cấu trúc phản hồi thanh ghi 0x04 của JBD)
void parseCellInfo() {
  if (rxBuffer[0] == 0xDD && rxBuffer[1] == 0x04) {
    
    // Byte thứ 3 chứa độ dài (số byte) của phần dữ liệu cell
    // Mỗi cell chiếm 2 byte, nên số lượng cell = Data Length / 2
    int dataLength = rxBuffer[3];
    int numCells = dataLength / 2;
    
    Serial.print("Số lượng Cell nhận diện: "); Serial.println(numCells);
    
    // Dữ liệu điện áp từng cell bắt đầu từ byte số 4
    int byteIndex = 4;
    for (int i = 0; i < numCells; i++) {
      // Ghép 2 byte thành điện áp cell (Đơn vị: mV)
      uint16_t cellVoltage_mV = (rxBuffer[byteIndex] << 8) | rxBuffer[byteIndex + 1];
      float cellVoltage_V = cellVoltage_mV / 1000.0;
      
      Serial.print(" - Cell "); Serial.print(i + 1); Serial.print(": ");
      Serial.print(cellVoltage_V, 3); // In ra 3 số lẻ (Ví dụ: 3.325 V)
      Serial.println(" V");
      
      byteIndex += 2; // Nhảy sang 2 byte của cell tiếp theo
    }
    Serial.println();
    
  } else {
    Serial.println("Gói tin Cell Info không hợp lệ.");
  }
}