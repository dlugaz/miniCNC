#ifndef GCODE_PARSER_H
#define GCODE_PARSER_H

#include "Arduino.h"
#include <stdio.h>



enum Commands
    {
        UNKNOWN,
        HELP,
        G0,
        G1,
        G28,
        G92,
        POSITION
    };

    struct GCodeParser {
        // private:
        String rawCommand;
        Commands command = UNKNOWN;
        float xAxis = 0;
        float yAxis = 0;   
        float zAxis = 0;
        int numberOfParsedElements = 0; 
        // public:
        GCodeParser (){       
        }
        GCodeParser (String inputString)
        {
            parseCommand(inputString);
        }
        ~GCodeParser(){     
        }
        bool parseCommand(String inputString)
        {   
            bool returnValue = false;
            char *temporaryBuffer = NULL;

            temporaryBuffer = new char[inputString.length()];
            
            inputString.toUpperCase();
            
            numberOfParsedElements = sscanf(inputString.c_str(),"%s %*c%f %*c%f %*c%f", temporaryBuffer,&xAxis,&yAxis,&zAxis);
            if(numberOfParsedElements >= 1)
            {
                rawCommand = temporaryBuffer;
                returnValue = true;

                if(rawCommand.equals("HELP")) 
                    command = HELP;
                else if(rawCommand.equals("G0"))
                        command = G0;
                else if(rawCommand.equals("G1"))
                        command = G1;
                else if(rawCommand.equals("G28")||rawCommand.equals("CALIBRATE"))
                        command = G28;
                else if(rawCommand.equals("G92"))
                        command = G92;
                else if(rawCommand.equals("POSITION"))
                    command = POSITION;
                else 
                    returnValue = false;
                //TODO add more commands
            }

            delete temporaryBuffer;

            return returnValue;

        }
    };


#endif // !GCODE_PARSER_H
