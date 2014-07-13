#include "../frameBuffer/frameBuffer.h"

extern void setForeColour(short colour);
extern void setGraphicsAddress(struct FrameBufferDescription* address);
extern void drawPixel(int x, int y);
extern void drawLine(int x0, int y0, int x1, int y1);
extern void drawCharacter(char character, int x, int y);
extern void drawString(char* string, int length, int x, int y);
