#include <string.h>
#include <math.h>
#include "node.h"
#include "constants.h"

#include "deps/memd.h"
#include "deps/minigfx.h"

// Forward declarations
bool isMouseOver(Node *self, MgSurface *surface);
void adjustHeight(Node *self);
bool isWithinBounds(Node *self, int width, int height);
void adjustNodeToBounds(Node *self, int width, int height);
bool isMouseOverInputConnector(Node *self, int connectorId, MgSurface *surface);
bool isMouseOverOutputConnector(Node *self, int connectorId, MgSurface *surface);

static bool isOneDragged = false;
static const int minWidth = 40;
static const int minHeight = 40;
static int connectorSpacing = 18;
static int namePadding = 34;
static int circleR = 8;

Node *nodeNew(int x, int y, char *name, MgPixel color)
{
    Node *node = malloc(sizeof(Node));
    node->x = x;
    node->y = y;

    int textW = mgTextWidth(mgfont, name);
    int wCalc = textW + namePadding * 2;
    node->width = minWidth > wCalc ? minWidth : wCalc;
    node->height = minHeight;

    node->name = name;
    node->color = color;

    node->numInputs = 0;
    node->numOutputs = 0;
    node->inputs = NULL;
    node->outputs = NULL;

    node->isDragging = false;
    node->dragOffsetX = 0;
    node->dragOffsetY = 0;
    node->prevButtons = 0;

    return node;
}

void nodeUpdate(Node *self, MgSurface *surface)
{
    int mx, my, buttons = 0;
    mgMouse(surface, &mx, &my, &buttons);

    bool mouseOver = isMouseOver(self, surface);
    bool mousePressed = (buttons & 1) && !(self->prevButtons & 1);  // Mouse button was just pressed
    bool mouseReleased = !(buttons & 1) && (self->prevButtons & 1); // Mouse button was just released

    if (mouseOver && mousePressed && !self->isDragging && !isOneDragged)
    {
        // Start dragging
        isOneDragged = true;
        self->isDragging = true;
        self->dragOffsetX = self->x - mx;
        self->dragOffsetY = self->y - my;
    }
    else if (self->isDragging && mouseReleased)
    {
        // Stop dragging
        isOneDragged = false;
        self->isDragging = false;
    }

    if (self->isDragging)
    {
        // Continue dragging
        self->x = mx + self->dragOffsetX;
        self->y = my + self->dragOffsetY;
    }

    self->prevButtons = buttons;

    adjustNodeToBounds(self, SCREEN_W, SCREEN_H - 80);
}

void nodeDraw(Node *self, MgSurface *surface)
{
    mgFillRect(surface, self->x, self->y, self->width, self->height, self->color);

    // Highlight when dragging
    if (isMouseOver(self, surface))
    {
        mgRect(surface, self->x, self->y, self->width, self->height, mgRGB(255, 255, 200));
    }

    int textHeight = mgTextHeight(mgfont, self->name);
    int textY = self->y + self->height / 2 - textHeight / 2;
    int textX = self->x + namePadding;

    mgPrint(surface, mgfont, textX, textY, mgRGB(160, 160, 160), self->name);

    // Draw inputs
    for (int i = 0; i < self->numInputs; ++i)
    {
        self->inputs[i]->x = self->x;
        self->inputs[i]->y = self->y + (i + 1) * connectorSpacing;

        if (isMouseOverInputConnector(self, i, surface))
        {
            mgFillCircle(surface, self->inputs[i]->x, self->inputs[i]->y, circleR, mgRGB(160, 160, 160));
        }
        else
        {
            mgFillCircle(surface, self->inputs[i]->x, self->inputs[i]->y, circleR, mgRGB(54, 54, 54));
        }

        int connNameH = mgTextHeight(mgfont, self->inputs[i]->name);
        mgPrint(surface, mgfont, self->x + circleR + 3, self->inputs[i]->y - connNameH / 3, mgRGB(200, 200, 200), self->inputs[i]->name);
    }

    // Draw outputs
    for (int i = 0; i < self->numOutputs; ++i)
    {
        self->outputs[i]->x = self->x + self->width;
        self->outputs[i]->y = self->y + (i + 1) * connectorSpacing;

        if (isMouseOverOutputConnector(self, i, surface))
        {
            mgFillCircle(surface, self->outputs[i]->x, self->outputs[i]->y, circleR, mgRGB(160, 160, 160));
        }
        else
        {
            mgFillCircle(surface, self->outputs[i]->x, self->outputs[i]->y, circleR, mgRGB(54, 54, 54));
        }

        int connNameW = mgTextWidth(mgfont, self->outputs[i]->name);
        int connNameH = mgTextHeight(mgfont, self->outputs[i]->name);
        mgPrint(surface, mgfont, (self->x + self->width) - connNameW - circleR - 3, self->outputs[i]->y - connNameH / 3, mgRGB(200, 200, 200), self->outputs[i]->name);
    }
}

void nodeFree(Node *self)
{
    if (self != NULL)
    {
        // Free all InputConnectors
        for (size_t i = 0; i < self->numInputs; ++i)
        {
            free(self->inputs[i]->name);
            free(self->inputs[i]);
        }
        free(self->inputs);

        // Free all OutputConnectors
        for (size_t i = 0; i < self->numOutputs; ++i)
        {
            free(self->outputs[i]->name);
            free(self->outputs[i]);
        }
        free(self->outputs);

        free(self);
    }
}

void nodeAddInput(Node *self, char *name)
{
    InputConnector *input = inputConnectorNew(circleR, name);
    self->numInputs++;
    self->inputs = realloc(self->inputs, self->numInputs * sizeof(InputConnector));
    self->inputs[self->numInputs - 1] = input;
    adjustHeight(self);
}

void nodeAddOutput(Node *self, char *name)
{
    OutputConnector *output = outputConnectorNew(circleR, name);
    self->numOutputs++;
    self->outputs = realloc(self->outputs, self->numOutputs * sizeof(OutputConnector));
    self->outputs[self->numOutputs - 1] = output;
    adjustHeight(self);
}

// helpers
bool isMouseOver(Node *self, MgSurface *surface)
{
    int mx, my, buttons = 0;
    mgMouse(surface, &mx, &my, &buttons);

    // Make the bounding box smaller in the width because we need to have the connectors beeing free
    int margin = circleR;

    return (mx - margin) > self->x && (mx + margin) < self->x + self->width && my > self->y && my < self->y + self->height;
}

bool isMouseOverInputConnector(Node *self, int connectorId, MgSurface *surface)
{
    int mx, my, buttons = 0;
    mgMouse(surface, &mx, &my, &buttons);

    if (connectorId < 0 || connectorId >= self->numInputs)
    {
        return false;
    }

    InputConnector *con = self->inputs[connectorId];
    if (con == NULL)
    {
        return false;
    }

    // Calculate the squared distance between the mouse and the connector's center
    int dx = mx - self->inputs[connectorId]->x;
    int dy = my - self->inputs[connectorId]->y;
    int distanceSquared = dx * dx + dy * dy;
    int radiusSquared = con->r * con->r;

    // Check if the distance is less than or equal to the radius
    return distanceSquared <= radiusSquared;
}

bool isMouseOverOutputConnector(Node *self, int connectorId, MgSurface *surface)
{
    int mx, my, buttons = 0;
    mgMouse(surface, &mx, &my, &buttons);

    if (connectorId < 0 || connectorId >= self->numOutputs)
    {
        return false;
    }

    OutputConnector *con = self->outputs[connectorId];
    if (con == NULL)
    {
        return false;
    }

    // Calculate the squared distance between the mouse and the connector's center
    int dx = mx - self->outputs[connectorId]->x;
    int dy = my - self->outputs[connectorId]->y;
    int distanceSquared = dx * dx + dy * dy;
    int radiusSquared = con->r * con->r;

    // Check if the distance is less than or equal to the radius
    return distanceSquared <= radiusSquared;
}

bool isWithinBounds(Node *self, int width, int height)
{
    return self->x >= 0 && self->y >= 0 &&
           (self->x + self->width) <= width &&
           (self->y + self->height) <= height;
}

void adjustNodeToBounds(Node *self, int width, int height)
{
    // Check and adjust for the left edge
    if (self->x < 0)
    {
        self->x = 0;
    }

    // Check and adjust for the right edge
    if ((self->x + self->width) > width)
    {
        self->x = width - self->width;
    }

    // Check and adjust for the top edge
    if (self->y < 0)
    {
        self->y = 0;
    }

    // Check and adjust for the bottom edge
    if ((self->y + self->height) > height)
    {
        self->y = height - self->height;
    }
}

void adjustHeight(Node *self)
{
    int maxTerminals = self->numInputs > self->numOutputs ? self->numInputs : self->numOutputs;
    int requiredHeight = maxTerminals * connectorSpacing + connectorSpacing;
    self->height = requiredHeight > minHeight ? requiredHeight : minHeight;
}