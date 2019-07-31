#ifndef _UTILS_H
#define _UTILS_H

//#include <Arduino.h>




// class utils
// {
//     private:
//     void toggleOutput(uint8_t pin)
//     {
//         if(digitalRead(pin)==HIGH)
//         {
//             digitalWrite(pin,LOW);
//         }
//         else
//         {
//             digitalWrite(pin,HIGH);
//         }

//     }

//     public:
//     void setupPeripheral()
//     {
//           pinMode(LED_BUILTIN,OUTPUT);
//     }

//     void heartbeat()
//     {

//           toggleOutput(LED_BUILTIN);
        

//     }

// }
   
    void toggleOutput(uint8_t pin)
    {
        if(digitalRead(pin)==HIGH)
        {
            digitalWrite(pin,LOW);
        }
        else
        {
            digitalWrite(pin,HIGH);
        }

    }
    


    void heartbeat(void* pvParameters)
    {
        while(1){
          toggleOutput(LED_BUILTIN);
          vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

// void help()
// {
//     usbSerial.println("Usage: help, calibrate, position, list");
// }

// void MoveAbsolute()
// {
    
// }

// void PrintPosition(){
//     usbSerial.print("X: "); usbSerial.println(xAxis.currentPosition());
//     usbSerial.print("Y: "); usbSerial.println(yAxis.currentPosition());
// }
// struct commandFunction {
//     Commands com;
//     void (*func) ();
// };

// commandFunction commandsFunctions[]
// {
//     {HELP, help},
//     {G0, MoveAbsolute},
    
//     {POSITION, PrintPosition},
//     {UNKNOWN, NULL}
// };

// void handleCommand(Commands CommandToExecute)
// {
//     for (int i = 0; commandsFunctions[i].func!=NULL ; ++i)
//     {
//         if(commandsFunctions[i].com == CommandToExecute)
//             commandsFunctions[i].func (CommandToExecute);
//     }
    
// }


#endif // !_UTILS_H
