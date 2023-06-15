//Inkludieren der erforderlichen Bibliotheken.
#include <Pixy2.h>

// Doku!!!
class camera {
private:
  Pixy2 pixy;
  int blocks;
  int frameWidth;
  int frameHeight;
  int Quader;
  int age;
  int y_pos;
  int x_pos;
  uint16_t signature;
  int width;
  int velocity;

public:
  camera() {}

  void init() {
    pixy.init();
    frameWidth = pixy.frameWidth;  //316
    frameHeight = pixy.frameHeight;
  }

  void firstBlockData() {
    Quader = 0;
    x_pos = 10;
    blocks = pixy.ccc.getBlocks();
    if (blocks) {
      y_pos = pixy.ccc.blocks[0].m_y;
      width = pixy.ccc.blocks[0].m_width;
      if (width > 15 && y_pos > frameHeight / 2) {
        age = pixy.ccc.blocks[0].m_age;
        if (age > 10) {
          x_pos = abs(pixy.ccc.blocks[0].m_x - frameWidth / 2);
          signature = pixy.ccc.blocks[0].m_signature;
          Quader = 1;
        }
      }
    }
  }

  int getX_pos() {
    return x_pos;
  }
  int get_color() {
    return signature;
  }
  float get_width() {
    return width;
  }
  float get_blocks() {
    return Quader;
  }
   void set_color() {
    signature=0;
  }
};