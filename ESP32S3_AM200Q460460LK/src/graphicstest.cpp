/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

#include <U8g2lib.h>
#include "FreeMono8pt7b.h"
#include "FreeSansBold10pt7b.h"
#include "FreeSerifBoldItalic12pt7b.h"
// #include <Wire.h>

#include <Adafruit_VS1053.h>

/* Uncomment a dev device in Arduino_GFX_dev_device.h */
#define GFX_DEV_DEVICE AM200Q460460LK

#include "Arduino_GFX_dev_device.h"

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#ifdef ESP32
#undef F
#define F(s) (s)
#endif

int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

void setup_screen();
void loop(void);
int32_t testFillScreen();
int32_t testText();
int32_t testPixels();
int32_t testLines();
int32_t testFastLines();
int32_t testFilledRects();
int32_t testRects();
int32_t testFilledCircles(uint8_t radius);
int32_t testCircles(uint8_t radius);
int32_t testFillArcs();
int32_t testArcs();
int32_t testFilledTriangles();
int32_t testTriangles();
int32_t testFilledRoundRects();
int32_t testRoundRects();

void setup_screen()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  // Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX PDQgraphicstest example!");

  // Init Display
  if (!gfx->begin(20 * 1000 * 1000))
  // if (!gfx->begin(80000000)) /* specify data bus speed */
  {
    Serial.println("gfx->begin() failed!");
  }

  gfx->setBrightness(60);

  // gfx->setUTF8Print(true); // enable UTF8 support for the Arduino print() function
  // gfx->setFont(u8g2_font_unifont_t_chinese);

  // gfx->setFont(&FreeMono8pt7b);

  // Serial.println("gfx->begin() started!");
  // delay(5000);
  // Serial.println("gfx->begin() done!");

  w = gfx->width();
  h = gfx->height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;
  tsa = ((w <= 176) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 4); // text size A
  tsb = ((w <= 272) || (h <= 220)) ? 1 : 2;                                    // text size B
  tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
  ds = (w <= 160) ? 9 : ((w <= 280) ? 10 : 12);                                // digit size

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

#ifdef ESP32
void printnice(const char *item, long int v)
#else
void printnice(const __FlashStringHelper *item, long int v)
#endif
{
  gfx->setTextSize(tsb);
  gfx->setTextColor(RGB565_CYAN);
  gfx->print(item);

  gfx->setTextSize(tsc);
  gfx->setTextColor(RGB565_YELLOW);
  if (v < 0)
  {
    gfx->println(F("      N / A"));
  }
  else
  {
    char str[32] = {0};
#ifdef RTL8722DM
    sprintf(str, "%d", (int)v);
#else
    sprintf(str, "%ld", v);
#endif
    for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
    {
      memmove(p + 1, p, strlen(p) + 1);
      *p = ',';
    }
    while (strlen(str) < ds)
    {
      memmove(str + 1, str, strlen(str) + 1);
      *str = ' ';
    }
    gfx->println(str);
  }
}

#ifdef ESP32
void serialOut(const char *item, int32_t v, uint32_t d, bool clear)
#else
void serialOut(const __FlashStringHelper *item, int32_t v, uint32_t d, bool clear)
#endif
{
#ifdef CANVAS
  gfx->flush();
#endif
  Serial.print(item);
  if (v < 0)
  {
    Serial.println(F("N/A"));
  }
  else
  {
    Serial.println(v);
  }
  delay(d);
  if (clear)
  {
    gfx->fillScreen(RGB565_BLACK);
  }
}

void loop_screen(void)
{
  Serial.println(F("Benchmark\tmicro-secs"));

  int32_t usecFillScreen = testFillScreen();

  serialOut(F("Screen fill\t"), usecFillScreen, 100, true);

  // Serial.println(F("before delay 0"));
  // delay(10000);
  // Serial.println(F("after delay 0"));

  int32_t usecText = testText();
  serialOut(F("Text\t"), usecText, 3000, true);

  // Serial.println(F("before delay 2"));
  // delay(10000);
  // Serial.println(F("after delay 2"));

  // int32_t usecPixels = testPixels();
  // serialOut(F("Pixels\t"), usecPixels, 100, true);

  // Serial.println(F("before delay 3"));
  // delay(5000);
  // Serial.println(F("after delay 3"));

  int32_t usecLines = testLines();
  serialOut(F("Lines\t"), usecLines, 100, true);

  int32_t usecFastLines = testFastLines();
  serialOut(F("Horiz/Vert Lines\t"), usecFastLines, 100, true);

  int32_t usecFilledRects = testFilledRects();
  serialOut(F("Rectangles (filled)\t"), usecFilledRects, 100, false);

  int32_t usecRects = testRects();
  serialOut(F("Rectangles (outline)\t"), usecRects, 100, true);

  int32_t usecFilledTrangles = testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 100, false);

  int32_t usecTriangles = testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 100, true);

  int32_t usecFilledCircles = testFilledCircles(10);
  serialOut(F("Circles (filled)\t"), usecFilledCircles, 100, false);

  int32_t usecCircles = testCircles(10);
  serialOut(F("Circles (outline)\t"), usecCircles, 100, true);

  int32_t usecFilledArcs = testFillArcs();
  serialOut(F("Arcs (filled)\t"), usecFilledArcs, 100, false);

  int32_t usecArcs = testArcs();
  serialOut(F("Arcs (outline)\t"), usecArcs, 100, true);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

#ifdef CANVAS
  uint32_t start = micros_start();
  gfx->flush();
  int32_t usecFlush = micros() - start;
  serialOut(F("flush (Canvas only)\t"), usecFlush, 0, false);
#endif

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    gfx->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
    {
      d = -d;
    }
  }

  gfx->setCursor(0, 0);

  gfx->setTextSize(tsa);
  gfx->setTextColor(RGB565_MAGENTA);
  gfx->println(F("Arduino GFX PDQ"));

  if (h > w)
  {
    gfx->setTextSize(tsb);
    gfx->setTextColor(RGB565_GREEN);
    gfx->print(F("\nBenchmark "));
    gfx->setTextSize(tsc);
    if (ds == 12)
    {
      gfx->print(F("   "));
    }
    gfx->println(F("micro-secs"));
  }

  printnice(F("Screen fill "), usecFillScreen);
  printnice(F("Text        "), usecText);
  // printnice(F("Pixels      "), usecPixels);
  printnice(F("Lines       "), usecLines);
  printnice(F("H/V Lines   "), usecFastLines);
  printnice(F("Rectangles F"), usecFilledRects);
  printnice(F("Rectangles  "), usecRects);
  printnice(F("Triangles F "), usecFilledTrangles);
  printnice(F("Triangles   "), usecTriangles);
  printnice(F("Circles F   "), usecFilledCircles);
  printnice(F("Circles     "), usecCircles);
  printnice(F("Arcs F      "), usecFilledArcs);
  printnice(F("Arcs        "), usecArcs);
  printnice(F("RoundRects F"), usecFilledRoundRects);
  printnice(F("RoundRects  "), usecRoundRects);

  if ((h > w) || (h > 240))
  {
    gfx->setTextSize(tsc);
    gfx->setTextColor(RGB565_GREEN);
    gfx->print(F("\nBenchmark Complete!"));
  }

#ifdef CANVAS
  gfx->flush();
#endif

  delay(10 * 1000L);
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  gfx->fillScreen(RGB565_WHITE);
  gfx->fillScreen(RGB565_RED);
  gfx->fillScreen(RGB565_GREEN);
  gfx->fillScreen(RGB565_BLUE);
  gfx->fillScreen(RGB565_BLACK);
  // 这里竟然刷出屏幕来，这后面delay能看到

#ifdef CANVAS
  gfx->flush(); // 确保Canvas内容刷新到屏幕
#endif

  return micros() - start;
}

int32_t testText()
{
  uint32_t start = micros_start();
  gfx->setCursor(0, 0);

  gfx->setTextSize(1);
  gfx->setTextColor(RGB565_WHITE, RGB565_BLACK);
  gfx->println(F("Hello World!"));

  gfx->setTextSize(2);
  gfx->setTextColor(gfx->color565(0xff, 0x00, 0x00));
  gfx->print(F("RED "));
  gfx->setTextColor(gfx->color565(0x00, 0xff, 0x00));
  gfx->print(F("GREEN "));
  gfx->setTextColor(gfx->color565(0x00, 0x00, 0xff));
  gfx->println(F("BLUE"));

  gfx->setTextSize(tsa);
  gfx->setTextColor(RGB565_YELLOW);
  gfx->println(1234.56);

  gfx->setTextColor(RGB565_WHITE);
  gfx->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

  gfx->setTextColor(RGB565_CYAN, RGB565_WHITE);
  gfx->println(F("Groop,"));

  gfx->setTextSize(tsc);
  gfx->setTextColor(RGB565_MAGENTA, RGB565_WHITE);
  gfx->println(F("I implore thee,"));

  gfx->setTextSize(1);
  gfx->setTextColor(RGB565_NAVY, RGB565_WHITE);
  gfx->println(F("my foonting turlingdromes."));

  gfx->setTextColor(RGB565_DARKGREEN, RGB565_WHITE);
  gfx->println(F("And hooptiously drangle me"));

  gfx->setTextColor(RGB565_DARKCYAN, RGB565_WHITE);
  gfx->println(F("with crinkly bindlewurdles,"));

  gfx->setTextColor(RGB565_MAROON, RGB565_WHITE);
  gfx->println(F("Or I will rend thee"));

  gfx->setTextColor(RGB565_PURPLE, RGB565_WHITE);
  gfx->println(F("in the gobberwartsb"));

  gfx->setTextColor(RGB565_OLIVE, RGB565_WHITE);
  gfx->println(F("with my blurglecruncheon,"));

  gfx->setTextColor(RGB565_DARKGREY, RGB565_WHITE);
  gfx->println(F("see if I don't!"));

  gfx->setTextSize(2);
  gfx->setTextColor(RGB565_RED);
  gfx->println(F("Size 2"));

  gfx->setTextSize(3);
  gfx->setTextColor(RGB565_ORANGE);
  gfx->println(F("Size 3"));

  gfx->setTextSize(4);
  gfx->setTextColor(RGB565_YELLOW);
  gfx->println(F("Size 4"));

  gfx->setTextSize(5);
  gfx->setTextColor(RGB565_GREENYELLOW);
  gfx->println(F("Size 5"));

  gfx->setTextSize(6);
  gfx->setTextColor(RGB565_GREEN);
  gfx->println(F("Size 6"));

  gfx->setTextSize(7);
  gfx->setTextColor(RGB565_BLUE);
  gfx->println(F("Size 7"));

  gfx->setTextSize(8);
  gfx->setTextColor(RGB565_PURPLE);
  gfx->println(F("Size 8"));

  gfx->setTextSize(9);
  gfx->setTextColor(RGB565_PALERED);
  gfx->println(F("Size 9"));

#ifdef CANVAS
  gfx->flush(); // 确保Canvas内容刷新到屏幕
#endif

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (int16_t y = 0; y < h; y++)
  {
    for (int16_t x = 0; x < w; x++)
    {
      gfx->drawPixel(x, y, gfx->color565(x << 3, y << 3, x * y));
    }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  start = micros_start();

  x1 = y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = 0;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, RGB565_BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
  {
    gfx->drawFastHLine(0, y, w, RGB565_RED);
  }
  for (x = 0; x < w; x += 5)
  {
    gfx->drawFastVLine(x, 0, h, RGB565_BLUE);
  }

  return micros() - start;
}

int32_t testFilledRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    gfx->fillRect(cx - i2, cy - i2, i, i, gfx->color565(i, i, 0));
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    gfx->drawRect(cx - i2, cy - i2, i, i, RGB565_GREEN);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      gfx->fillCircle(x, y, radius, RGB565_MAGENTA);
    }
  }

  return micros() - start;
}

int32_t testCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      gfx->drawCircle(x, y, radius, RGB565_WHITE);
    }
  }

  return micros() - start;
}

int32_t testFillArcs()
{
  int16_t i, r = (360 > cn) ? (360 / cn) : 1;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->fillArc(cx1, cy1, i, i - 3, 0, i * r, RGB565_RED);
  }

  return micros() - start;
}

int32_t testArcs()
{
  int16_t i, r = (360 > cn) ? (360 / cn) : 1;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->drawArc(cx1, cy1, i, i - 3, 0, i * r, RGB565_WHITE);
  }

  return micros() - start;
}

int32_t testFilledTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    gfx->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      gfx->color565(0, i, i));
  }

  return micros() - start;
}

int32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    gfx->drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        gfx->color565(0, 0, i));
  }

  return micros() - start;
}

int32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    gfx->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(0, i, 0));
  }

  return micros() - start;
}

int32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 20; i < n1; i += 6)
  {
    i2 = i / 2;
    gfx->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(i, 0, 0));
  }

  return micros() - start;
}

/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/