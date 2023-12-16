#include "./../../include/public/screen-manager.h"

void _getTerminalSize(unsigned int *width, unsigned int *height){
    getmaxyx(stdscr, *height, *width);
}


struct ErrorWindowsMeasures _calculateErrorWindowsMeasures(float errorWindowWidthFraction, struct WindowSize winSize){
    struct ErrorWindowsMeasures measures;

    measures.errorWindowWidth = winSize.width * (errorWindowWidthFraction <= 0 ? 0.5 : errorWindowWidthFraction);
    measures.errorWindowTop = winSize.height / 2 - 2;
    measures.errorWindowLeft = winSize.width * (1 - errorWindowWidthFraction) / 2;
    measures.exitMessageWindowWidth = winSize.width;
    measures.exitMessageWindowTop = winSize.height / 2 + 3;

    return measures;
}