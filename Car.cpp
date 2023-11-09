

#include <SoftwareSerial.h>

// Global Variáveis
bool carRunning = false;
boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
bool switchCar = false;
float anguloY;

SoftwareSerial serial1(11, 10);  // RX, TX

// Iremos fazer uma classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.
class DCMotor {  
  int spd = 255, pin1, pin2;
  
  public:  
    void Pinout(int in1, int in2){ // Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
    }   
    void Speed(int in1){ // Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
      spd = in1;
    }     
    void Forward(){ // Forward é o método para fazer o motor ir para frente
      analogWrite(pin1, spd);
      analogWrite(pin2, spd);
    }   
    void Spin(){
      if(anguloY < 1)
      {
        analogWrite(pin1, spd);
        digitalWrite(pin2, LOW);
      }
      else if(anguloY > 2)
      {
        digitalWrite(pin1, LOW);
        analogWrite(pin2, spd);
      }
      else
      {
        analogWrite(pin1, spd);
        analogWrite(pin2, spd);
      }
    }
    void Stop(){ // Stop é o metodo para fazer o motor ficar parado.
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
    }
};

DCMotor Motor1, Motor2; // Criação de dois objetos motores, já que usaremos dois motores, e eles já estão prontos para receber os comandos já configurados acima. 

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    char receivedBool[6]; // Armazena "true" ou "false"
    bool receivedBoolean;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        } else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;

            // Verificar se os dados são "true" ou "false" e armazenar em uma variável booleana
            if (strcmp(receivedChars, "true") == 0) {
                receivedBoolean  = true;
                receivedBoolean  = switchCar;
            } else if (strcmp(receivedChars, "false") == 0) {
                receivedBoolean  = false;
                receivedBoolean  = switchCar;
            } else {
                Serial.println("Valor booleano inválido!"); // Caso não seja "true" ou "false"
            }
        }
    }
}

void showNewData()
{
  float receivedFloatValue = 0.0;
  
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);

    // Converter a string em um valor float
    receivedFloatValue = atof(receivedChars);

    // Agora você pode usar receivedFloatValue como um float
    Serial.print("Valor float recebido: ");
    Serial.println(receivedFloatValue);

    receivedFloatValue = anguloY;
    newData = false;
  }
}
void carRun()
{
  if (switchCar && !carRunning)
  {
    carRunning = true;
    Motor1.Speed(200);
    Motor2.Speed(200);
    Motor1.Forward();
    Motor2.Forward();
    Motor1.Spin();
    Motor2.Spin();
    delay(1000);
  }
  else if (switchCar && carRunning)
  {
    carRunning = false;
    Motor1.Stop();
    Motor2.Stop();
  }

  if (!carRunning)
  {
    Motor1.Stop();
    Motor2.Stop();
  }
}

void setup() {
  serial1.begin(9600);
  serial1.print("$");
  serial1.print("$");
  serial1.print("$");

  Motor1.Pinout(5, 6);
  Motor2.Pinout(3, 9); 
}

void loop() {
  recvWithEndMarker();
  showNewData();
  carRun();
