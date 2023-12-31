#include "Display.h"
#include "Constants.h"
#include "TFTMC050_3.h"

void Display::drawPixel(int x, int y, int colour) {

  int unoPixeloIgriko = LCD_YSIZE_TFT / gridYLimit; // change name later
  int unoPixeloIkselo = LCD_XSIZE_TFT / gridXLimit; // DEFINITELY change name later

  ER5517.DrawSquare_Fill(x, y, unoPixeloIkselo+x, y+unoPixeloIgriko, colour);
}
