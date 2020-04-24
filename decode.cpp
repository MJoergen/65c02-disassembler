#include "decode.h"

const addrMode_t addrModes[256] = 
{//   0x00     0x01     0x02     0x03     0x04     0x05     0x06     0x07      0x08     0x09     0x0A     0x0B     0x0C     0x0D     0x0E     0x0F
    AM_IMM,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_ACC,  AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0x00
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_ZP,   AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_ACC,  AM_NONE, AM_ABS,  AM_ABSX, AM_ABSX, AM_ZR,  // 0x10
    AM_ABS,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_ACC,  AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0x20
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_ZPX,  AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_ACC,  AM_NONE, AM_ABSX, AM_ABSX, AM_ABSX, AM_ZR,  // 0x30
    AM_NONE, AM_INDX, AM_IMM,  AM_NONE,  AM_IMM,  AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_ACC,  AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0x40
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_IMM,  AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_ABS,  AM_ABSX, AM_ABSX, AM_ZR,  // 0x50
    AM_NONE, AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_ACC,  AM_NONE, AM_IND,  AM_ABS,  AM_ABS,  AM_ZR,  // 0x60
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_ZPX,  AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_IABSX,AM_ABSX, AM_ABSX, AM_ZR,  // 0x70
    AM_REL,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_NONE, AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0x80
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_ZPX,  AM_ZPX,  AM_ZPY,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_ABS,  AM_ABSX, AM_ABSX, AM_ZR,  // 0x90
    AM_IMM,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_NONE, AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0xA0
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_ZPX,  AM_ZPX,  AM_ZPY,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_ABSX, AM_ABSX, AM_ABSY, AM_ZR,  // 0xB0
    AM_IMM,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_NONE, AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0xC0
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_IMM,  AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_ABS,  AM_ABSX, AM_ABSX, AM_ZR,  // 0xD0
    AM_IMM,  AM_INDX, AM_IMM,  AM_NONE,  AM_ZP,   AM_ZP,   AM_ZP,   AM_ZP,    AM_NONE, AM_IMM,  AM_NONE, AM_NONE, AM_ABS,  AM_ABS,  AM_ABS,  AM_ZR,  // 0xE0
    AM_REL,  AM_INDY, AM_IZP,  AM_NONE,  AM_IMM,  AM_ZPX,  AM_ZPX,  AM_ZP,    AM_NONE, AM_ABSY, AM_NONE, AM_NONE, AM_ABS,  AM_ABSX, AM_ABSX, AM_ZR   // 0xF0
}; // addrModes

const instruction_t instructions[256] = 
{//  0x00   0x01   0x02   0x03   0x04   0x05   0x06   0x07    0x08   0x09   0x0A   0x0B   0x0C   0x0D   0x0E   0x0F
    I_BRK, I_ORA, I_NOP, I_NOP, I_TSB, I_ORA, I_ASL, I_RMB,  I_PHP, I_ORA, I_ASLA,I_NOP, I_TSB, I_ORA, I_ASL, I_BBR, // 0x00
    I_BPL, I_ORA, I_ORA, I_NOP, I_TRB, I_ORA, I_ASL, I_RMB,  I_CLC, I_ORA, I_INCA,I_NOP, I_TRB, I_ORA, I_ASL, I_BBR, // 0x10
    I_JSR, I_AND, I_NOP, I_NOP, I_BIT, I_AND, I_ROL, I_RMB,  I_PLP, I_AND, I_ROLA,I_NOP, I_BIT, I_AND, I_ROL, I_BBR, // 0x20
    I_BMI, I_AND, I_AND, I_NOP, I_BIT, I_AND, I_ROL, I_RMB,  I_SEC, I_AND, I_DECA,I_NOP, I_BIT, I_AND, I_ROL, I_BBR, // 0x30
    I_RTI, I_EOR, I_NOP, I_NOP, I_NOP, I_EOR, I_LSR, I_RMB,  I_PHA, I_EOR, I_LSRA,I_NOP, I_JMP, I_EOR, I_LSR, I_BBR, // 0x40
    I_BVC, I_EOR, I_EOR, I_NOP, I_NOP, I_EOR, I_LSR, I_RMB,  I_CLI, I_EOR, I_PHY, I_NOP, I_NOP, I_EOR, I_LSR, I_BBR, // 0x50
    I_RTS, I_ADC, I_NOP, I_NOP, I_STZ, I_ADC, I_ROR, I_RMB,  I_PLA, I_ADC, I_RORA,I_NOP, I_JMP, I_ADC, I_ROR, I_BBR, // 0x60
    I_BVS, I_ADC, I_ADC, I_NOP, I_STZ, I_ADC, I_ROR, I_RMB,  I_SEI, I_ADC, I_PLY, I_NOP, I_JMP, I_ADC, I_ROR, I_BBR, // 0x70
    I_BRA, I_STA, I_NOP, I_NOP, I_STY, I_STA, I_STX, I_SMB,  I_DEY, I_BIT, I_TXA, I_NOP, I_STY, I_STA, I_STX, I_BBS, // 0x80
    I_BCC, I_STA, I_STA, I_NOP, I_STY, I_STA, I_STX, I_SMB,  I_TYA, I_STA, I_TXS, I_NOP, I_STZ, I_STA, I_STZ, I_BBS, // 0x90
    I_LDY, I_LDA, I_LDX, I_NOP, I_LDY, I_LDA, I_LDX, I_SMB,  I_TAY, I_LDA, I_TAX, I_NOP, I_LDY, I_LDA, I_LDX, I_BBS, // 0xA0
    I_BCS, I_LDA, I_LDA, I_NOP, I_LDY, I_LDA, I_LDX, I_SMB,  I_CLV, I_LDA, I_TSX, I_NOP, I_LDY, I_LDA, I_LDX, I_BBS, // 0xB0
    I_CPY, I_CMP, I_NOP, I_NOP, I_CPY, I_CMP, I_DEC, I_SMB,  I_INY, I_CMP, I_DEX, I_NOP, I_CPY, I_CMP, I_DEC, I_BBS, // 0xC0
    I_BNE, I_CMP, I_CMP, I_NOP, I_NOP, I_CMP, I_DEC, I_SMB,  I_CLD, I_CMP, I_PHX, I_NOP, I_NOP, I_CMP, I_DEC, I_BBS, // 0xD0
    I_CPX, I_SBC, I_NOP, I_NOP, I_CPX, I_SBC, I_INC, I_SMB,  I_INX, I_SBC, I_NOP, I_NOP, I_CPX, I_SBC, I_INC, I_BBS, // 0xE0
    I_BEQ, I_SBC, I_SBC, I_NOP, I_NOP, I_SBC, I_INC, I_SMB,  I_SED, I_SBC, I_PLX, I_NOP, I_NOP, I_SBC, I_INC, I_BBS  // 0xF0
}; // instructions

const int instLen[256] = 
{
    2, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0x00
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0x10
    3, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0x20
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0x30
    1, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0x40
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0x50
    1, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0x60
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0x70
    2, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0x80
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0x90
    2, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0xA0
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0xB0
    2, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0xC0
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3,  // 0xD0
    2, 2, 2, 1, 2, 2, 2, 2,    1, 2, 1, 1, 3, 3, 3, 3,  // 0xE0
    2, 2, 2, 1, 2, 2, 2, 2,    1, 3, 1, 1, 3, 3, 3, 3   // 0xF0
}; // instLen

