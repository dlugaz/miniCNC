//#include <Arduino.h>
#include <MapleFreeRTOS821.h>
#include <usb_serial.h>
#include <AccelStepper.h>

#include <utils.h>
#include <Gcode_parser.h>


//IO settings
USBSerial usbSerial;
//Stepper xAxisMotor(StepsPerRevolution, 0,1,2,4);
//Stepper yAxisMotor(StepsPerRevolution, 5,6,7,8);


// Slider xAxis(StepsPerRevolution, 0,1,2,3);
// Slider yAxis(StepsPerRevolution, 4,5,6,7);
AccelStepper xAxis(AccelStepper::FULL4WIRE, PA0, PA1, PA2, PA3);
AccelStepper yAxis(AccelStepper::FULL4WIRE, PA4, PA5, PA6, PA7);
AccelStepper zAxis(AccelStepper::FULL4WIRE, PA8, PA9,PA10,PA11);

#define ENDSWITCH_X PA12
#define ENDSWITCH_Y PA13
#define ENDSWITCH_Z PA14

QueueHandle_t xQueue = NULL;



void Communication(void *pvParameters) 
{
    String receivedData;
    GCodeParser gcodeParser ("help");

    for (;;) {
 
        vTaskDelay(pdMS_TO_TICKS(2000));
        if(usbSerial.availableForWrite()>0)
        {
            usbSerial.println("Hello! This is STM32");
        }
        
        if(usbSerial.available() > 0)
        {
            receivedData = usbSerial.readStringUntil('\n');

            bool parsingSuccess = gcodeParser.parseCommand(receivedData);
            if(!parsingSuccess)
            {
                usbSerial.println("Sorry! I didn't understand you!");
            }
            else
            {
                
                switch(gcodeParser.command)
                {
                    case HELP:
                    usbSerial.println("Usage: help, G0 X Y, calibrate, position");
                    break;
                    case G0: 
                        usbSerial.println("Moving to absolute position");
                        usbSerial.print("X");usbSerial.println(gcodeParser.xAxis);
                        usbSerial.print("Y");usbSerial.println(gcodeParser.yAxis);
                        usbSerial.print("Z");usbSerial.println(gcodeParser.zAxis);
                        
                        xAxis.moveTo((long)gcodeParser.xAxis);
                        yAxis.moveTo((long)gcodeParser.yAxis);
                        zAxis.moveTo((long)gcodeParser.zAxis);
                        break;
                        
                    case G1:
                        usbSerial.println("Moving to relative position");
                        usbSerial.print("X");usbSerial.println(gcodeParser.xAxis);
                        usbSerial.print("Y");usbSerial.println(gcodeParser.yAxis);
                        usbSerial.print("Z");usbSerial.println(gcodeParser.zAxis);
                        xAxis.move((long)gcodeParser.xAxis);
                        yAxis.move((long)gcodeParser.yAxis);
                        zAxis.move((long)gcodeParser.zAxis);
          
                        // if(xQueue !=NULL)
                        // {
                        //     if(pdTRUE != xQueueSend(xQueue,&gcodeParser,pdMS_TO_TICKS(1000)))
                        //     {
                        //         usbSerial.println("Command Queue is full");
                        //     }
                        // }
                                          
                        break;
                    case G28:
                        xAxis.moveTo(-1000);
                        yAxis.moveTo(-1000);
                        zAxis.moveTo(-1000);
                    

                    case POSITION:
                        usbSerial.print("X: "); usbSerial.println(xAxis.currentPosition());
                        usbSerial.print("Y: "); usbSerial.println(yAxis.currentPosition());
                        usbSerial.print("Z: "); usbSerial.println(zAxis.currentPosition());
                    break;
                    
                    default:
                    break;

                }


            }
        }
        
        }

}



void MotorControl(void *pvParameters) 
{
    // GCodeParser recCommand;

    while(1)
    {
        //Calibrate positions
        // if(digitalRead(ENDSWITCH_X)==LOW
        //     &&xAxis.currentPosition()!=0) 
        //     {
        //         xAxis.setCurrentPosition(0); 
        //         if(xAxis.targetPosition()<0)
        //             xAxis.moveTo(0);
        //     }

        // if(digitalRead(ENDSWITCH_Y)==LOW
        //     &&yAxis.currentPosition()!=0)
        //     {
        //         yAxis.setCurrentPosition(0); 
        //         if(yAxis.targetPosition()<0)
        //             yAxis.moveTo(0);
        //     }
               
        // if(digitalRead(ENDSWITCH_Z)==LOW
        //     &&zAxis.currentPosition()!=0) 
        //     {
        //         zAxis.setCurrentPosition(0); 
        //         if(zAxis.targetPosition()<0)
        //             zAxis.moveTo(0);
        //     }
        if(xAxis.distanceToGo()==0)
            xAxis.disableOutputs();
            else
            xAxis.enableOutputs();
        if(yAxis.distanceToGo()==0)
            yAxis.disableOutputs();
            else
            yAxis.enableOutputs();
        if(zAxis.distanceToGo()==0)
            zAxis.disableOutputs();
            else
            zAxis.enableOutputs(); 
        //Step motors
        xAxis.run();
        yAxis.run();
        zAxis.run();

        // if(!xAxis.isRunning()&&!yAxis.isRunning()&&!zAxis.isRunning())
        // {
        //     usbSerial.println("ok");
        // }

         vTaskDelay(pdMS_TO_TICKS(10));
    }
}


void setup() {
    usbSerial.begin();

    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(PB0,INPUT_PULLUP);
    pinMode(PB0,INPUT_PULLUP);
    pinMode(ENDSWITCH_X, INPUT_PULLUP);
    pinMode(ENDSWITCH_Y, INPUT_PULLUP);
    pinMode(ENDSWITCH_Z, INPUT_PULLUP);

    // Configure each stepper
    xAxis.setSpeed(20);
    yAxis.setSpeed(20);
    zAxis.setSpeed(20);
    
    xAxis.setMaxSpeed(50);
    yAxis.setMaxSpeed(50);
    zAxis.setMaxSpeed(50);
    
    
    // xQueue = xQueueCreate(5,sizeof(Command));
   
    xTaskCreate(Communication,
                "Communication Task",
                3*configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(MotorControl,
                "Control Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 3,
                NULL);

    xTaskCreate(heartbeat,
                "heartbeat",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 3,
                NULL);           
    

    vTaskStartScheduler();

}

void loop() {


}