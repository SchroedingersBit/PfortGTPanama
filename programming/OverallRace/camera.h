#include <Pixy2.h>

class camera {
private:
  Pixy2 pixy;
  uint16_t Width;
  uint16_t Height;
  int8_t Blocks;
  uint16_t signature;
  float m_x;
  uint16_t m_y;
  unsigned long time;
  int last_signature = 0;
  int new_signature;
  bool Block_test;
  bool timer = true;
  float size;

public:

  camera() {}

  void init() {
    pixy.init();
    Width = pixy.frameWidth / 2;
    Height = pixy.frameHeight;
  }

  void BlockData(bool Adder) {
    Block_test = true * timer;
    Blocks = pixy.ccc.getBlocks();
    if (Blocks && timer) {
      for (int i = 0; i < Blocks; i++) {
        m_y = pixy.ccc.blocks[i].m_y;
        if (m_y >= Height * 0.2 && m_y <= Height * 0.85) {
          new_signature = pixy.ccc.blocks[i].m_signature;
          m_x = pixy.ccc.blocks[i].m_x - Width;
          Block_test = false;
          size = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
          break;
        }
      }
    }
    if (Block_test && timer) {
      new_signature = 0;
      m_x = 0;
      size = 0;
    }
    if (last_signature != 0 && (Block_test || (!Block_test && last_signature != new_signature))) {
      signature = last_signature;
      timer = false;
      time = millis() + 100UL + 100UL * !Adder;
    } else if (timer) {
      signature = new_signature;
    }
    if (time < millis() && !timer)
      timer = true;

    last_signature = new_signature;
  }

  int get_color() {
    return signature;
  }

  int get_x_pos() {
    return m_x;
  }

  int get_frameWidth() {
    return Width;
  }

  int get_Blockcheck() {
    return Block_test;
  }

  int get_Blocksize() {
    return size;
  }
};

