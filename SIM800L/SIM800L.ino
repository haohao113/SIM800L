#include <Arduino.h>
#include <SoftwareSerial.h>

const int RX_PIN = 2;
const int TX_PIN = 3;
const int RELAY_PIN = 7;          // Chân điều khiển relay
const String SDT = "+1234567890"; // Số điện thoại được phép điều khiển

SoftwareSerial sim800l(RX_PIN, TX_PIN); // SIM800L Tx & Rx được kết nối với Arduino #3 & #2

void setup()
{
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Tắt relay ban đầu

  Serial.begin(9600);
  sim800l.begin(9600);

  sim800l.println("AT"); // Sau khi kiểm tra kết nối thành công, nó sẽ trả về OK
  delay(1000);
  if (sim800l.available())
  {
    Serial.println(sim800l.readString());
  }
  sim800l.println("AT+CMGF=1"); // Cấu hình chế độ TEXT
  delay(1000);
  if (sim800l.available())
  {
    Serial.println(sim800l.readString());
  }
  sim800l.println("AT+CNMI=1,2,0,0,0"); // Thiết lập để nhận tin nhắn SMS tự động
  delay(1000);
  if (sim800l.available())
  {
    Serial.println(sim800l.readString());
  }
}
void loop()
{
  if (sim800l.available())
  {
    String message = sim800l.readString();
    Serial.println(message);

    if (message.indexOf(SDT) != -1) // Kiểm tra số điện thoại gửi tin nhắn
    {

      if (message.indexOf("ON") != -1) // Kiểm tra nội dung tin nhắn
      {
        digitalWrite(RELAY_PIN, HIGH); // Bật relay
        Serial.println("Relay ON");
      }
      else if (message.indexOf("OFF") != -1)
      {
        digitalWrite(RELAY_PIN, LOW); // Tắt relay
        Serial.println("Relay OFF");
      }
    }
    else
    {
      Serial.println("Số điện thoại không được phép");
    }
  }
}
