#include "./../include/design.h"

// Forward declarations
void normalizeSegment(unsigned char number, Digit segmentDigits[2]);
void drawClockWindow(WINDOW *targetWindow, ClockPixel (*shapeToBeDrawn)[3], ColorID digitColorID);
void fillClockColons(struct DatetimeScreenManagerDesignerModules userArguments);

// Public functions

void drawAllClockWindows(struct tm *timeStruct, struct DatetimeScreenManagerDesignerModules userArguments){
    WINDOW *segmentToFill[2];

    fillClockSegment(getClockSegment(HOURS_SEGMENT, segmentToFill), timeStruct->tm_hour);
    fillClockSegment(getClockSegment(MINUTES_SEGMENT, segmentToFill), timeStruct->tm_min);

    if(userArguments.hideTheSeconds == false && checkIfTheSecondsIsVisible() == true)
        fillClockSegment(getClockSegment(SECONDS_SEGMENT, segmentToFill), timeStruct->tm_sec);

    fillClockColons(userArguments);

    refreshWindows();
}

void fillClockSegment(WINDOW *clockWindows[], unsigned char numberToDraw){
    Digit segmentDigits[2];
    ClockPixel (*theDigit)[3];
    ColorID digitColorID;

    normalizeSegment(numberToDraw, segmentDigits);

    for(short i = 0; i < 2; i++){
        theDigit = getDigitShape(segmentDigits[i]);
        digitColorID = getDigitColor(0);

        drawClockWindow(clockWindows[i], theDigit, digitColorID);
    }
}

// Get the date window and draw the date stored in the program,
// given by the user or by the OS
void drawDate(struct tm *theTime, struct DatetimeModule datetimeArguments, struct ColorsModule colorArguments){
    char dateBuffer[MAX_CLOCK_DATE_BUFFER_LEN + 1];
    WINDOW *dateWindow;
    size_t dateStringLen;

    dateWindow = getDateWindow();

    generateDateString(*theTime, datetimeArguments, dateBuffer);
    dateStringLen = strlen(dateBuffer) + 2;

    setDateStringLength(dateStringLen);
    moveDateWindowToPlaceholder();
    
    wattron(dateWindow, COLOR_PAIR(getDateColor()));
    mvwprintw(dateWindow, 1, 1, dateBuffer);
    wattroff(dateWindow, COLOR_PAIR(getDateColor()));
    wrefresh(dateWindow);
    refresh();
}

void writeErrorMessageOnErrorWindow(char *msg, size_t windowWidth, WINDOW *errorWindow){
    size_t msgLen = strlen(msg);
    size_t requiredLines;
    size_t remainder;
    short i;
    

    wclear(errorWindow);
    wrefresh(errorWindow);

    requiredLines = msgLen / (float) windowWidth;
    remainder = msgLen % windowWidth;

    wattron(errorWindow, COLOR_PAIR(ERROR_MESSAGE_RED_ID));

    // Write chunks of the error message with the     
    // max length that each line support
    for(i = 1; i <= requiredLines; i++){
        mvwaddnstr(errorWindow, i, 0, msg, windowWidth);

        msg += windowWidth;
    }

    // Write the rest of the message that doesn't
    // fill the whole line aligned to the center
    mvwaddnstr(errorWindow, i, windowWidth / 2 - remainder / 2, msg, remainder);

    wattroff(errorWindow, COLOR_PAIR(ERROR_MESSAGE_RED_ID));

    wrefresh(errorWindow);
    refresh();
}

// A callback that draws the program error on screen.
// this procedure is given to the screen-manager module 
// to be possible draw the date without a include, that 
// would generate a circular dependency
void drawProgramErrorCallback(void *arguments){
    struct UpdateErrorFramesCallbackArguments *typecastedArguments = (struct UpdateErrorFramesCallbackArguments*) arguments;

    clear();
    refresh();
    
    if(false){
        wclear(typecastedArguments->windows.exitMessageWindow);
        wrefresh(typecastedArguments->windows.exitMessageWindow);
    }

    writeErrorMessageOnErrorWindow(typecastedArguments->errorMsg, typecastedArguments->windows.measures.errorWindowWidth, typecastedArguments->windows.errorWindow);

    refresh();
}

// Private functions

// Procedure that split a number into two digits, if the number is smaller than 10
// a zero will placed before the number. For instance: 9 -> 09, 4 -> 04
void normalizeSegment(unsigned char number, Digit segmentDigits[2]){
    if(number < 10){
        segmentDigits[0] = 0;
        segmentDigits[1] = number;
    }else{
        segmentDigits[0] = ((int)(number / 10));
        segmentDigits[1] = (number % 10);
    }
}

// Draws the given digit shape to the given window
void drawClockWindow(WINDOW *targetWindow, ClockPixel (*shapeToBeDrawn)[3], ColorID digitColorID){
    wmove(targetWindow, 0, 0);

    for(short j = 0; j < 5; j++){
        for(short k = 0; k < 3; k++){
            if(shapeToBeDrawn[j][k] == COLOR){
                wattron(targetWindow, COLOR_PAIR(digitColorID));
                wprintw(targetWindow, "  ");
                wattroff(targetWindow, COLOR_PAIR(digitColorID));
                wrefresh(targetWindow);
            }else{
                wprintw(targetWindow, "  ");
                wrefresh(targetWindow);
            }
        }
    }
}

// Draws the colons of the clock
void fillClockColons(struct DatetimeScreenManagerDesignerModules userArguments){
    ClockPixel (*colonShape)[3] = getColonShape();
    ColorID colonColor = getColonColor();
    WINDOW *segmentToFill[2];

    drawClockWindow(getClockSegment(FIRST_CLOCK_COLON, segmentToFill)[0], colonShape, colonColor);

    // The last colon is drawn only if the seconds is visible
    if(userArguments.hideTheSeconds == false && checkIfTheSecondsIsVisible() == true)
        drawClockWindow(getClockSegment(SECOND_CLOCK_COLON, segmentToFill)[0], colonShape, colonColor);
}