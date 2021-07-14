#pragma once
#include "InOut.h"
void goToXy(SHORT posX, SHORT posY);
void SetColor(int backgound_color, int text_color);
void SetWindowSize(SHORT width, SHORT height);
void SetScreenBufferSize(SHORT width, SHORT height);
void DisableResizeWindow();
void DisableCtrButton(bool Close, bool Min, bool Max);
BOOL DirectoryExists(const char* dirName);
void DisableSelection();
void Console();