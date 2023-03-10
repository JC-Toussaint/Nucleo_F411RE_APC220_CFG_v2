/* USER CODE BEGIN */
#include "hal_lcd.h"
#include "fonts.h"
#include "bluetooth_logo.h"
#include <stdio.h>
#include <string.h>

static char tempLcdBuffer[32];

void LCD_Init(void)
{
  SSD1306_Init();
}

void LCD_PrintTest(uint16_t x,uint16_t y, char * msg, SSD1306_COLOR_t color, LCD_CharSize_t charSize)
{
  FontDef_t Font;
  
  /* Clear screen first */
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  
  SSD1306_GotoXY(x,y);
  
  if(charSize == LCD_CHAR_SMALL){
    Font = Font_7x10;
    SSD1306_Puts("12345678901234567890", &Font, color);
    SSD1306_UpdateScreen();

    SSD1306_GotoXY(x,y+10);
    SSD1306_Puts("ABCDEFGHIJKLMNOPQRST", &Font, color);
    SSD1306_UpdateScreen();

    SSD1306_GotoXY(x,y+20);
    SSD1306_Puts("abcdefghijklmnopqrst", &Font, color);
    SSD1306_UpdateScreen();
  }else if(charSize == LCD_CHAR_MEDIUM)
  {
    Font = Font_11x18;
    SSD1306_Puts("12345678901", &Font, color);
    SSD1306_UpdateScreen();

    SSD1306_GotoXY(x,y+14);
    SSD1306_Puts("ABCDEFGHIJK", &Font, color);
    SSD1306_UpdateScreen();
  }else if(charSize == LCD_CHAR_BIG)
  {
    Font = Font_16x26;
    SSD1306_Puts("12345678", &Font, color);
    SSD1306_UpdateScreen();
  }else
  {
    /* put Medium size by default */
    Font = Font_11x18;
    SSD1306_Puts("12345678901", &Font, color);
    SSD1306_UpdateScreen();

    SSD1306_GotoXY(x,y+14);
    SSD1306_Puts("ABCDEFGHIJK", &Font, color);
    SSD1306_UpdateScreen();
  }
  

}

/* Only 11 characters per line with font 11x18 can be displayed */
void LCD_Print1stLine(char * msg)
{
  FontDef_t Font;
  
  /* Clear screen first */
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  
  SSD1306_GotoXY(0,0);
  /* put Medium size by default */
  Font = Font_11x18;
  
  SSD1306_Puts(msg, &Font, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
}

void LCD_Print2ndLine(char * msg)
{
  FontDef_t Font;
  
  /* Clear screen first */
  //SSD1306_Fill(SSD1306_COLOR_BLACK);
  
  SSD1306_GotoXY(0,14);
  /* put Medium size by default */
  Font = Font_11x18;
  
  SSD1306_Puts(msg, &Font, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
}

/* Only 11 characters per line with Font_11x18 can be displayed */
/* Only 20 characters per line with Font_7x10  can be displayed */
void LCD_Print(char* line1, char* line2){
  FontDef_t Font;
  
  /* Clear screen first */
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  
  SSD1306_GotoXY(0,0);
  /* put Medium size by default */
  Font = Font_11x18;
  SSD1306_Puts(line1, &Font, SSD1306_COLOR_WHITE);
  
  if(line2 != NULL)
  {
    Font = Font_7x10;
    SSD1306_GotoXY(0,22);
    SSD1306_Puts("-> ", &Font, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(20,22);
    SSD1306_Puts(line2, &Font, SSD1306_COLOR_WHITE);
  }
  
  SSD1306_UpdateScreen();
}

void LCD_PrintLabel(char * label)
{
  SSD1306_DrawFilledRectangle(0,0,80,19,SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(0,0);
  SSD1306_Puts(label, &Font_11x18, SSD1306_COLOR_WHITE); 
  SSD1306_UpdateScreen();
}

void LCD_THREAD_PrintRLOC(uint16_t rloc)
{
  sprintf(tempLcdBuffer, "0x%04X", rloc);
  SSD1306_DrawFilledRectangle(80,20,52,12,SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(82,22);
  SSD1306_Puts(tempLcdBuffer, &Font_7x10, SSD1306_COLOR_WHITE); 
  SSD1306_UpdateScreen();
}

void LCD_THREAD_PrintPanId(uint16_t panId)
{
  sprintf(tempLcdBuffer, "0x%04X", panId);
  SSD1306_DrawFilledRectangle(80,0,52,20,SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(82,7);
  SSD1306_Puts(tempLcdBuffer, &Font_7x10, SSD1306_COLOR_WHITE); 
  SSD1306_UpdateScreen();
}

void LCD_THREAD_PrintRole(char * role)
{
  SSD1306_DrawFilledRectangle(0,20,80,12,SSD1306_COLOR_BLACK);
  SSD1306_DrawFilledRectangle(0,20,5 + (strlen(role) * 7),12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(3,22);
  SSD1306_Puts(role, &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

void LCD_THREAD_PrintAddress(char * address, uint8_t length)
{
  sprintf(tempLcdBuffer, "x:%s", address);
  SSD1306_DrawFilledRectangle(80,20,52,12,SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(82,22);
  SSD1306_Puts(tempLcdBuffer, &Font_7x10, SSD1306_COLOR_WHITE); 
  SSD1306_UpdateScreen();
}

void LCD_BLE_PrintLocalName(const char * name)
{
  SSD1306_DrawFilledRectangle(31,0,80,19,SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(31,0);
  SSD1306_Puts(name + 1, &Font_11x18, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
}
  
void LCD_BLE_Indicate_Server1(void)
{
  SSD1306_DrawFilledRectangle(60,20,9,12,SSD1306_COLOR_BLACK);
  SSD1306_DrawFilledRectangle(60,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(61,22);
  SSD1306_Putc('1', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}


void LCD_BLE_Indicate_Server2(void)
{
  SSD1306_DrawFilledRectangle(71,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(72,22);
  SSD1306_Putc('2', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}


void LCD_BLE_Indicate_Server3(void)
{
  SSD1306_DrawFilledRectangle(82,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(83,22);
  SSD1306_Putc('3', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}


void LCD_BLE_Indicate_Server4(void)
{  
  SSD1306_DrawFilledRectangle(93,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(94,22);
  SSD1306_Putc('4', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}


void LCD_BLE_Indicate_Server5(void)
{
  SSD1306_DrawFilledRectangle(104,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(105,22);
  SSD1306_Putc('5', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}


void LCD_BLE_Indicate_Server6(void)
{  
  SSD1306_DrawFilledRectangle(115,20,9,12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(116,22);
  SSD1306_Putc('6', &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

void LCD_BLE_PrintLogo(void)
{
  uint16_t x, y;
  
  for (y = 0; y < BLUETOOTH_LOGO_HEIGHT; y++)
  {
    for (x = 0; x < (BLUETOOTH_LOGO_WIDTH); x++)
    {
      SSD1306_DrawPixel(22 - x, y, (SSD1306_COLOR_t)((bluetooth_logo[y] >> (x + 2)) & 0x00000001));
    }
  }
  SSD1306_UpdateScreen();
}

void LCD_BLE_PrintStatus(char * status)
{
  SSD1306_DrawFilledRectangle(31,20,28,12,SSD1306_COLOR_BLACK);
  SSD1306_DrawFilledRectangle(31,20,5 + (strlen(status) * 7),12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(34,22);
  SSD1306_Puts(status, &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

void LCD_BLE_HRS_PrintBPM(uint8_t BPM)
{
  SSD1306_DrawFilledRectangle(0,20,80,12,SSD1306_COLOR_BLACK);
  //SSD1306_DrawFilledRectangle(0,20,5 + (strlen(role) * 7),12,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(3,22);
  //SSD1306_Puts(role, &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

void LCD_BLE_HTS_PrintTemperature(uint8_t temperature)
{
  //sprintf(tempLcdBuffer, "#%02d", errId);
  sprintf(tempLcdBuffer, "%02d", temperature);
  SSD1306_DrawFilledRectangle(0,0,128,32,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(7,8);
  SSD1306_Puts("TEMP", &Font_11x18, SSD1306_COLOR_BLACK);
  SSD1306_GotoXY(66,9);
  SSD1306_Puts(tempLcdBuffer, &Font_11x18, SSD1306_COLOR_BLACK);
  SSD1306_UpdateScreen();
}

void LCD_BLE_TPS_PrintRSSI(uint8_t RSSI)
{
  //sprintf(tempLcdBuffer, "#%02d", errId);
  SSD1306_DrawFilledRectangle(0,0,128,32,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(7,8);
  SSD1306_Puts("ERROR", &Font_11x18, SSD1306_COLOR_BLACK); 
  SSD1306_GotoXY(66,9);
  SSD1306_Puts(tempLcdBuffer, &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

void LCD_PrintError(uint32_t errId)
{
  sprintf(tempLcdBuffer, "#%02lu", errId);
  SSD1306_DrawFilledRectangle(0,0,128,32,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(7,8);
  SSD1306_Puts("ERROR", &Font_11x18, SSD1306_COLOR_BLACK); 
  SSD1306_GotoXY(66,9);
  SSD1306_Puts(tempLcdBuffer, &Font_7x10, SSD1306_COLOR_BLACK); 
  SSD1306_UpdateScreen();
}

/* USER CODE END */
