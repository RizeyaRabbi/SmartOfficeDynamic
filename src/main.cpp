#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>
#define ON 1
#define OFF 0
#define arraySize(i) (sizeof(i) / sizeof(i[0]))

uint8_t server[] = {203, 130, 133, 166};
uint8_t mac[] = {0xAa, 0xBb, 0xCc, 0xDd, 0xEe, 0xFf};
uint8_t ip[] = {192, 168, 0, 118};
EthernetClient client;

const uint8_t input[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37};
const uint8_t output[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

uint8_t inputStatus[arraySize(input)];
uint8_t outputStatus[arraySize(output)];

void InputConfig(const uint8_t *_input)
{
  for (uint8_t i = 0; i < arraySize(input); i++)
  {
    pinMode(_input[i], INPUT_PULLUP);
  }
}
void OutputConfig(const uint8_t *_output)
{
  for (uint8_t i = 0; i < arraySize(output); i++)
  {
    pinMode(_output[i], OUTPUT);
  }
}
void UpdateInputPinStatus(const uint8_t *_input, uint8_t *_inputStatus)
{
  for (uint8_t i = 0; i < arraySize(input); i++)
  {
    _inputStatus[i] = digitalRead(_input[i]);
  }
}
void UpdateOutputPinStatus(const uint8_t *_output, uint8_t *_outputStatus)
{
  for (uint8_t i = 0; i < sizeof(output); i++)
  {
    _outputStatus[i] = digitalRead(_output[i]);
  }
}
void PrintInputPinStatus(const uint8_t *_input, uint8_t *_inputStatus)
{
  for (uint8_t i = 0; i < arraySize(input); i++)
  {
    Serial.print("\n");
    Serial.print("Status of input pin ");
    Serial.print(_input[i]);
    Serial.print(" is: ");
    Serial.print(_inputStatus[i]);
  }
  Serial.print("\n");
}
void PrintOutputPinStatus(const uint8_t *_output, uint8_t *_outputStatus)
{
  for (uint8_t i = 0; i < arraySize(output); i++)
  {
    Serial.print("\n");
    Serial.print("Status of output pin ");
    Serial.print(_output[i]);
    Serial.print(" is: ");
    Serial.print(_outputStatus[i]);
  }
  Serial.print("\n");
}
void EthernetConfig(uint8_t *_mac, uint8_t *_ip)
{
  Ethernet.begin(_mac, _ip);
  delay(2000);
  if (Ethernet.hardwareStatus() == EthernetW5100)
  {
    Serial.print("\n");
    Serial.print("W5100 Ethernet controller detected.");
    Serial.print("\n");
    Serial.print("IP address:");
    Serial.print(" ");
    Serial.print(Ethernet.localIP());
  }
  // else if (Ethernet.hardwareStatus() == EthernetW5500)
  // {
  //   Serial.print("\n");
  //   Serial.print("W5500 Ethernet controller detected.");
  //   Serial.print("\n");
  //   Serial.print("IP address:");
  //   Serial.print(" ");
  //   Serial.print(Ethernet.localIP());
  // }
  // else if (Ethernet.hardwareStatus() == EthernetW5200)
  // {
  //   Serial.print("\n");
  //   Serial.print("W5200 Ethernet controller detected.");
  //   Serial.print("\n");
  //   Serial.print("IP address:");
  //   Serial.print(" ");
  //   Serial.print(Ethernet.localIP());
  // }
  else if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.");
  }
  // if (Ethernet.linkStatus() == Unknown)
  // {
  //   Serial.print("\n");
  //   Serial.print("Link status unknown. Link status detection is only available with W5200 and W5500.");
  // }
  // else if (Ethernet.linkStatus() == LinkON)
  // {
  //   Serial.print("\n");
  //   Serial.print("Link status: On");
  // }
  // else if (Ethernet.linkStatus() == LinkOFF)
  // {
  //   Serial.print("\n");
  //   Serial.print("Link status: Off");
  // }
  Serial.print("\n");
  Serial.print("The gateway IP address is: ");
  Serial.print(Ethernet.gatewayIP());

  Serial.print("\n");
  Serial.print("The subnet mask is: ");
  Serial.print(Ethernet.subnetMask());

  // Serial.print("\n");
  // uint8_t macBuffer[6];
  // Ethernet.MACAddress(macBuffer);
  // Serial.print("The MAC address is: ");
  // for (uint8_t octet = 0; octet < 6; octet++)
  // {
  //   Serial.print(macBuffer[octet], HEX);
  //   if (octet < 5)
  //   {
  //     Serial.print(':');
  //   }
  // }
  Serial.print("\n");
}

class Controller
{
private:
  unsigned long waitTime, startInterval, startWaitTime, logStart, logStop, logValue, startTime, stopTime,
      debounceDelay, secondsInOneMinute, milliSecondsInOneSecond;
  bool deviceOn, keepDeviceOn, logFlag;

public:
  Controller();
  void Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t sS3, uint8_t sS4, uint8_t device, const char *dN, unsigned long wT);
  void Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t sS3, uint8_t device, const char *dN, unsigned long wT);
  void Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t device, const char *dN, unsigned long wT);
  void Automation(uint8_t sS0, uint8_t sS1, uint8_t device, const char *dN, unsigned long wT);
  void Automation(uint8_t sS0, uint8_t device, const char *dN, unsigned long wT);
  void interval(unsigned long intervalTime, void (&function)());
  void writeLog(const char *_dN, unsigned int _timeOn);
  void debugVariables();
};
Controller::Controller()
{
  debounceDelay = 250UL;
  secondsInOneMinute = 6UL;
  milliSecondsInOneSecond = 1000UL;
  deviceOn = false;
  keepDeviceOn = false;
  logFlag = false;
}
void Controller::Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t sS3, uint8_t sS4, uint8_t device, const char *dN, unsigned long wT)
{
  waitTime = wT * secondsInOneMinute * milliSecondsInOneSecond;
  if (!deviceOn)
  {
    if (sS0 == HIGH || sS1 == HIGH || sS2 == HIGH || sS3 == HIGH || sS4 == HIGH)
    {
      digitalWrite(device, ON);
      delay(debounceDelay);
      if (!logFlag)
      {
        logStart = millis();
        logFlag = true;
        Serial.print(dN);
        Serial.print(" ");
        Serial.print("Turned On");
        Serial.print("\n");
      }
      deviceOn = true;
      keepDeviceOn = true;
    }
  }
  if (deviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW && sS3 == LOW && sS4 == LOW)
    {
      deviceOn = false;
      startWaitTime = millis();
    }
  }
  if (keepDeviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW && sS3 == LOW && sS4 == LOW && (millis() - startWaitTime >= waitTime))
    {
      digitalWrite(device, OFF);
      delay(debounceDelay);
      if (logFlag)
      {
        logStop = millis();
        logValue = (logStop - logStart) / 1000;
        writeLog(dN, logValue);
        Serial.print(dN);
        Serial.print(" ");
        Serial.print(" Turned Off and was On for:");
        Serial.print(" ");
        Serial.print(logValue);
        Serial.print(" ");
        Serial.print("Seconds");
        Serial.print("\n");
        logFlag = false;
      }
      keepDeviceOn = false;
    }
  }
}
void Controller::Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t sS3, uint8_t device, const char *dN, unsigned long wT)
{
  waitTime = wT * secondsInOneMinute * milliSecondsInOneSecond;
  if (!deviceOn)
  {
    if (sS0 == HIGH || sS1 == HIGH || sS2 == HIGH || sS3 == HIGH)
    {
      digitalWrite(device, ON);
      delay(debounceDelay);
      if (!logFlag)
      {
        logStart = millis();
        logFlag = true;
        Serial.print(dN);
        Serial.print(" ");
        Serial.print("Turned On");
        Serial.print("\n");
      }
      deviceOn = true;
      keepDeviceOn = true;
    }
  }
  if (deviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW && sS3 == LOW)
    {
      deviceOn = false;
      startWaitTime = millis();
    }
  }
  if (keepDeviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW && sS3 == LOW && (millis() - startWaitTime >= waitTime))
    {
      digitalWrite(device, OFF);
      delay(debounceDelay);
      if (logFlag)
      {
        logStop = millis();
        logValue = (logStop - logStart) / 1000;
        writeLog(dN, logValue);
        Serial.print(dN);
        Serial.print(" ");
        Serial.print(" Turned Off and was On for:");
        Serial.print(" ");
        Serial.print(logValue);
        Serial.print(" ");
        Serial.print("Seconds");
        Serial.print("\n");
        logFlag = false;
      }
      keepDeviceOn = false;
    }
  }
}
void Controller::Automation(uint8_t sS0, uint8_t sS1, uint8_t sS2, uint8_t device, const char *dN, unsigned long wT)
{
  waitTime = wT * secondsInOneMinute * milliSecondsInOneSecond;
  if (!deviceOn)
  {
    if (sS0 == HIGH || sS1 == HIGH || sS2 == HIGH)
    {
      digitalWrite(device, ON);
      delay(debounceDelay);
      if (!logFlag)
      {
        logStart = millis();
        logFlag = true;
        Serial.print(dN);
        Serial.print(" ");
        Serial.print("Turned On");
        Serial.print("\n");
      }
      deviceOn = true;
      keepDeviceOn = true;
    }
  }
  if (deviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW)
    {
      deviceOn = false;
      startWaitTime = millis();
    }
  }
  if (keepDeviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && sS2 == LOW && (millis() - startWaitTime >= waitTime))
    {
      digitalWrite(device, OFF);
      delay(debounceDelay);
      if (logFlag)
      {
        logStop = millis();
        logValue = (logStop - logStart) / 1000;
        writeLog(dN, logValue);
        Serial.print(dN);
        Serial.print(" ");
        Serial.print(" Turned Off and was On for:");
        Serial.print(" ");
        Serial.print(logValue);
        Serial.print(" ");
        Serial.print("Seconds");
        Serial.print("\n");
        logFlag = false;
      }
      keepDeviceOn = false;
    }
  }
}
void Controller::Automation(uint8_t sS0, uint8_t sS1, uint8_t device, const char *dN, unsigned long wT)
{
  waitTime = wT * secondsInOneMinute * milliSecondsInOneSecond;
  if (!deviceOn)
  {
    if (sS0 == HIGH || sS1 == HIGH)
    {
      digitalWrite(device, ON);
      delay(debounceDelay);
      if (!logFlag)
      {
        logStart = millis();
        logFlag = true;
        Serial.print(dN);
        Serial.print(" ");
        Serial.print("Turned On");
        Serial.print("\n");
      }
      deviceOn = true;
      keepDeviceOn = true;
    }
  }
  if (deviceOn)
  {
    if (sS0 == LOW && sS1 == LOW)
    {
      deviceOn = false;
      startWaitTime = millis();
    }
  }
  if (keepDeviceOn)
  {
    if (sS0 == LOW && sS1 == LOW && (millis() - startWaitTime >= waitTime))
    {
      digitalWrite(device, OFF);
      delay(debounceDelay);
      if (logFlag)
      {
        logStop = millis();
        logValue = (logStop - logStart) / 1000;
        writeLog(dN, logValue);
        Serial.print(dN);
        Serial.print(" ");
        Serial.print(" Turned Off and was On for:");
        Serial.print(" ");
        Serial.print(logValue);
        Serial.print(" ");
        Serial.print("Seconds");
        Serial.print("\n");
        logFlag = false;
      }
      keepDeviceOn = false;
    }
  }
}
void Controller::Automation(uint8_t sS0, uint8_t device, const char *dN, unsigned long wT)
{
  waitTime = wT * secondsInOneMinute * milliSecondsInOneSecond;
  if (!deviceOn)
  {
    if (sS0 == HIGH)
    {
      digitalWrite(device, ON);
      delay(debounceDelay);
      if (!logFlag)
      {
        logStart = millis();
        logFlag = true;
        Serial.print(dN);
        Serial.print(" ");
        Serial.print("Turned On");
        Serial.print("\n");
      }
      deviceOn = true;
      keepDeviceOn = true;
    }
  }
  if (deviceOn)
  {
    if (sS0 == LOW)
    {
      deviceOn = false;
      startWaitTime = millis();
    }
  }
  if (keepDeviceOn)
  {
    if (sS0 == LOW && (millis() - startWaitTime >= waitTime))
    {
      digitalWrite(device, OFF);
      delay(debounceDelay);
      if (logFlag)
      {
        logStop = millis();
        logValue = (logStop - logStart) / 1000;
        writeLog(dN, logValue);
        Serial.print(dN);
        Serial.print(" ");
        Serial.print(" Turned Off and was On for:");
        Serial.print(" ");
        Serial.print(logValue);
        Serial.print(" ");
        Serial.print("Seconds");
        Serial.print("\n");
        logFlag = false;
      }
      keepDeviceOn = false;
    }
  }
}
void Controller::interval(unsigned long intervalTime, void (&function)())
{
  if (millis() - startInterval >= intervalTime)
  {
    function();
    startInterval = millis();
  }
}
void Controller::writeLog(const char *_dN, unsigned int _timeOn)
{
  StaticJsonDocument<48> doc;
  doc["IOT_NAME"] = _dN;
  doc["TIME_ON"] = _timeOn;
  WriteBufferingStream bufferedClient(client, 64);
  if (client.connect(server, 80))
  {
    client.println("POST /ATI-ERP2/api/create-iot-log HTTP/1.1");
    client.println("Host: 203.130.133.166");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(measureJson(doc));
    client.println("Content-Type: application/json");
    client.println();
    serializeJson(doc, bufferedClient);
    Serial.println("Log created successfully.");
  }
  else
  {
    Serial.println("Could not connect to the server. Retrying... ");
    if (client.connect(server, 80))
    {
      client.println("POST /ATI-ERP2/api/create-iot-log HTTP/1.1");
      client.println("Host: 203.130.133.166");
      client.println("User-Agent: Arduino/1.0");
      client.println("Connection: close");
      client.print("Content-Length: ");
      client.println(measureJson(doc));
      client.println("Content-Type: application/json");
      client.println();
      serializeJson(doc, bufferedClient);
      Serial.println("Log created successfully.");
    }
    else
    {
      Serial.println("Retry Failed.");
    }
  }
}
void Controller::debugVariables()
{
  Serial.print("\n");
  Serial.print("Value of deviceOn: ");
  Serial.print(deviceOn);

  Serial.print("\n");
  Serial.print("Value of keepDeviceOn: ");
  Serial.print(keepDeviceOn);

  Serial.print("\n");
  Serial.print("Value of logFlag: ");
  Serial.print(logFlag);

  Serial.print("\n");
  Serial.print("Value of debounceDelay: ");
  Serial.print(debounceDelay);

  Serial.print("\n");
  Serial.print("Value of secondsInOneMinute: ");
  Serial.print(secondsInOneMinute);

  Serial.print("\n");
  Serial.print("Value of milliSecondsInOneSecond: ");
  Serial.print(milliSecondsInOneSecond);
  Serial.print("\n");
}

Controller device[16];

void setup()
{
  Serial.begin(9600);
  InputConfig(input);
  OutputConfig(output);
  EthernetConfig(mac, ip);
  UpdateInputPinStatus(input, inputStatus);
  UpdateOutputPinStatus(output, outputStatus);
  PrintInputPinStatus(input, inputStatus);
  PrintOutputPinStatus(output, outputStatus);
}

void loop()
{
  UpdateInputPinStatus(input, inputStatus);
  UpdateOutputPinStatus(output, outputStatus);
  device[0].Automation(inputStatus[0], output[0], "TestDevice0", 1);
  device[1].Automation(inputStatus[1], output[1], "TestDevice1", 1);
  device[2].Automation(inputStatus[2], output[2], "TestDevice2", 1);
  device[3].Automation(inputStatus[3], output[3], "TestDevice3", 1);
  device[4].Automation(inputStatus[4], output[4], "TestDevice4", 1);
  device[5].Automation(inputStatus[5], output[5], "TestDevice5", 1);
  device[6].Automation(inputStatus[6], output[6], "TestDevice6", 1);
  device[7].Automation(inputStatus[7], output[7], "TestDevice7", 1);
  device[8].Automation(inputStatus[8], output[8], "TestDevice8", 1);
  device[9].Automation(inputStatus[9], output[9], "TestDevice9", 1);
  device[10].Automation(inputStatus[10], output[10], "TestDevice10", 1);
  device[11].Automation(inputStatus[11], output[11], "TestDevice11", 1);
  device[12].Automation(inputStatus[12], output[12], "TestDevice12", 1);
  device[13].Automation(inputStatus[13], output[13], "TestDevice13", 1);
  device[14].Automation(inputStatus[14], output[14], "TestDevice14", 1);
  device[15].Automation(inputStatus[15], output[15], "TestDevice15", 1);
}