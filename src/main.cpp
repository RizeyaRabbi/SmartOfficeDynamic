#include <Arduino.h>
#define arraySize(i) (sizeof(i) / sizeof(i[0]))

const uint8_t input[] = {22, 23, 24, 25, 26};
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

void setup()
{
  Serial.begin(9600);
  InputConfig(input);
  OutputConfig(output);
  UpdateInputPinStatus(input, inputStatus);
  UpdateOutputPinStatus(output, outputStatus);
  PrintInputPinStatus(input, inputStatus);
  PrintOutputPinStatus(output, outputStatus);
}

void loop()
{
}