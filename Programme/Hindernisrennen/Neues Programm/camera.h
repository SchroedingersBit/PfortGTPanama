#include <Pixy2.h>



class camera {
private:
  Pixy2 pixy;
  int blocks;
  int frameWidth;
  int x_pos;
  uint16_t signature;
  int width;

public:
  camera() {}

  void init() {
    pixy.init();
    frameWidth = pixy.frameWidth;  //316
  }

  void firstBlockData() {
    blocks = pixy.ccc.getBlocks();
    width = pixy.ccc.blocks[0].m_width;
    if (blocks) {
      if (width > 15) {
        x_pos = pixy.ccc.blocks[0].m_x - frameWidth / 2;
        signature = pixy.ccc.blocks[0].m_signature;
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
    return blocks;
  }
   void set_color() {
    signature=0;
  }
};
