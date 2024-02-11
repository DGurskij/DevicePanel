#pragma once
#include "../Panel.h"

void registratorWorker(Panel* panel);

void pauseEmitter(Panel* panel, const char* params);
void resumeEmitter(Panel* panel, const char* params);

void fasterEmitter(Panel* panel, const char* params);
void slowerEmitter(Panel* panel, const char* params);
void intervalEmitter(Panel* panel, const char* params);