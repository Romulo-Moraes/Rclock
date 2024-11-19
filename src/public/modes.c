#include <public/pomodoro.h>
#include <public/arguments.h>
#include <public/design.h>
#include <helpers/design.h>

void pomodoroMode(struct tm *timeStruct, struct tm *timeStructOldValue, void (*timeoutHandler)(int)){
    if(timeStruct->tm_sec != timeStructOldValue->tm_sec){
        updateClock(*timeStruct, *timeStructOldValue, POMODORO_MODE);
        
        *timeStructOldValue = *timeStruct;

        if (timeStruct->tm_min == 0 && timeStruct->tm_sec == 0) {
            signal(SIGALRM, timeoutHandler);
            setTimeout(POMODORO_TIMEOUT);
            drawOptions(OPTIONS_BACKGROUND_TRANSPARENT_ID);
            return;
        }
    }
}

void clockMode(struct DatetimeModuleArguments datetime, struct ColorsModuleArguments colors, struct tm timeStruct, struct tm timeStructOldValue) {

    if(timeStruct.tm_sec != timeStructOldValue.tm_sec){

        updateClock(timeStruct, timeStructOldValue, CLOCK_MODE);

        if(timeStruct.tm_hour < timeStructOldValue.tm_hour){
            mktime(&timeStruct);
            drawDate(&timeStruct, datetime, colors);
        }
                
        // Making both have the same value for the next alarm
        timeStructOldValue = timeStruct;
    }

}