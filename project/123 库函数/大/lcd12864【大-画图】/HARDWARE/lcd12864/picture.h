
/*--  调入了一幅图像  --*/
/*--  宽度x高度=128x64  --*/
/*此图像是初始化界面*/
unsigned char Photo1[] = 
{
      0x1F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF8,
      0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFC,
      0x7F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFE,
      0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,
      0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,
      0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,
      0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x06,0x00,0x00,0x06,0x00,0x00,
      0x06,0x00,0x00,0x00,0xC0,0x00,0x00,0x0F,
      0xF0,0x00,0x06,0x00,0x0C,0x0E,0x00,0x01,
      0x02,0x00,0x00,0x1F,0xE0,0x03,0x80,0x0F,
      0xE0,0x00,0x04,0x00,0x06,0x08,0x00,0x01,
      0x82,0x00,0x03,0xF8,0x70,0x03,0x80,0x07,
      0xC0,0x00,0x04,0x00,0x02,0x10,0x70,0x03,
      0x02,0xE0,0x01,0x18,0x20,0x03,0x80,0x03,
      0x80,0x01,0x8C,0x40,0x00,0x63,0x90,0x02,
      0x1F,0x80,0x01,0x18,0x20,0x03,0x80,0x01,
      0x00,0x3F,0x89,0xF0,0x00,0x21,0x10,0x06,
      0x02,0x00,0x01,0x08,0x20,0x03,0x00,0x00,
      0x00,0x01,0x96,0x60,0x00,0x21,0x10,0x04,
      0x02,0xF0,0x01,0x1E,0x20,0x03,0x00,0x00,
      0x00,0x01,0x20,0x40,0x06,0x21,0x30,0x0C,
      0x7F,0x30,0x01,0x28,0x20,0x03,0x00,0x00,
      0x00,0x09,0x0C,0x00,0x1E,0x2D,0xB0,0x0A,
      0x22,0x20,0x01,0x08,0x20,0x03,0x00,0x00,
      0x00,0x0D,0x04,0x00,0x04,0x31,0x60,0x12,
      0x22,0x60,0x01,0x0A,0x20,0x03,0x00,0x00,
      0x00,0x07,0x04,0x00,0x04,0x71,0x20,0x22,
      0x1F,0x80,0x03,0x3E,0x20,0x03,0x00,0x00,
      0x00,0x03,0x0C,0x00,0x06,0x61,0x00,0x42,
      0x06,0x00,0x03,0xF8,0x20,0x01,0x00,0x00,
      0x00,0x07,0x8E,0x00,0x02,0x03,0x00,0x02,
      0x34,0x00,0x02,0x18,0x20,0x00,0x00,0x00,
      0x00,0x05,0x89,0x00,0x02,0x03,0x00,0x02,
      0x0C,0x00,0x02,0x18,0x20,0x00,0x00,0x00,
      0x00,0x08,0x89,0x00,0x7F,0x02,0x00,0x06,
      0x06,0x00,0x02,0x18,0x20,0x00,0x00,0x00,
      0x00,0x10,0x10,0x80,0x00,0xE2,0x00,0x06,
      0x0B,0x00,0x06,0x10,0x30,0x03,0x80,0x00,
      0x00,0x20,0x20,0xE0,0x00,0x1E,0x00,0x06,
      0x31,0xC0,0x04,0x11,0x30,0x03,0x80,0x00,
      0x00,0x00,0x40,0x78,0x00,0x07,0xFC,0x06,
      0x60,0x7E,0x08,0x00,0xF0,0x03,0x00,0x00,
      0x00,0x00,0x00,0x3C,0x00,0x01,0xE0,0x04,
      0x00,0x30,0x10,0x00,0x60,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x02,0xA0,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x1D,0x58,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x20,0x06,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0xC0,0x03,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x80,0x00,0x84,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x01,0x00,0x00,0x9A,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x03,0x00,0x20,0x42,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x0C,0x70,0x72,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x06,0x1C,0x70,0x0A,0x00,0x01,
      0x14,0x30,0x00,0x40,0x00,0x00,0x00,0x00,
      0x00,0x00,0x04,0x0E,0x30,0x20,0x00,0x07,
      0xD2,0x08,0x00,0x40,0x00,0x00,0x00,0x00,
      0x00,0x00,0x04,0x0C,0x00,0x30,0x00,0x01,
      0x10,0x04,0x00,0x40,0x00,0x00,0x00,0x00,
      0x00,0x00,0x04,0x00,0x00,0x10,0x00,0x0F,
      0xFE,0x04,0x07,0xFC,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x06,0x20,0x00,0x02,
      0x10,0x0A,0x04,0x44,0x00,0x00,0x00,0x00,
      0x00,0x00,0x04,0x60,0x02,0x10,0x00,0x0F,
      0xD4,0x0A,0x04,0x44,0x00,0x00,0x00,0x00,
      0x00,0x00,0x06,0x40,0x06,0x20,0x00,0x05,
      0x14,0x0A,0x04,0x44,0x00,0x00,0x00,0x00,
      0x00,0x00,0x02,0x60,0x04,0xB0,0x00,0x07,
      0xD4,0x11,0x07,0xFC,0x00,0x00,0x00,0x00,
      0x80,0x00,0x02,0x20,0x09,0x08,0x00,0x01,
      0x08,0x11,0x04,0x44,0x00,0x00,0x00,0x01,
      0xC0,0x00,0x02,0x18,0x12,0x04,0x00,0x01,
      0xCA,0x20,0x80,0x40,0x08,0x20,0x80,0x03,
      0xE0,0x00,0x01,0x0F,0xF2,0xF4,0x00,0x0F,
      0x16,0x40,0x40,0x40,0x00,0x00,0x00,0x07,
      0xF0,0x00,0x0D,0x81,0x04,0x04,0x00,0x01,
      0x22,0x80,0x20,0x40,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x08,0xC0,0x03,0x12,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x01,0x20,0x06,0x14,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x0D,0x34,0x5B,0x0C,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x00,0x47,0x61,0x88,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x00,0x30,0x00,0x60,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,
      0xF0,0x00,0x00,0x83,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,
      0xF0,0x00,0x00,0x62,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,
      0xFF,0xF0,0x00,0x22,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,
      0x7F,0xF8,0x00,0x6C,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFE,
      0x3F,0xFC,0x00,0x10,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFC,
      0x1F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF8
};
