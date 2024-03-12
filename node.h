#ifndef __NODE_H__
#define __NODE_H__

#include <stdbool.h>
#include "deps/minigfx.h"
#include "connector.h"

typedef struct
{
    // Drawing stuff
    int x;
    int y;
    int width;
    int height;
    char *name;
    MgPixel color;

    // Logic stuff
    InputConnector **inputs;
    int numInputs;
    OutputConnector **outputs;
    int numOutputs;

    // Internal usage
    bool isDragging;
    int dragOffsetX;
    int dragOffsetY;
    int prevButtons;

} Node;

Node *nodeNew(int x, int y, char *name, MgPixel color);
void nodeUpdate(Node *self, MgSurface *surface);
void nodeDraw(Node *self, MgSurface *surface);
void nodeFree(Node *self);
void nodeAddInput(Node *self, char *name);
void nodeAddOutput(Node *self, char *name);

#endif // __NODE_H__