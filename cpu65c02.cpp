#include <iomanip>
#include "cpu65c02.h"
#include "decode.h"

static uint16_t sign_extend(uint8_t arg)
{
    return (arg < 0x80) ? arg : 0xFF00 | arg;
}

// Disassemble the current instruction.
int Cpu65C02::disas(std::ostream& os, uint16_t pc) const
{
    uint8_t inst = m_memory.read(pc);
    switch (instructions[inst])
    {
        case I_RES: os << "???"; break;
        case I_ORA: os << "ORA"; break;
        case I_AND: os << "AND"; break;
        case I_EOR: os << "EOR"; break;
        case I_ADC: os << "ADC"; break;
        case I_STA: os << "STA"; break;
        case I_LDA: os << "LDA"; break;
        case I_CMP: os << "CMP"; break;
        case I_SBC: os << "SBC"; break;
        case I_CLC: os << "CLC"; break;
        case I_SEC: os << "SEC"; break;
        case I_CLI: os << "CLI"; break;
        case I_SEI: os << "SEI"; break;
        case I_CLV: os << "CLV"; break;
        case I_CLD: os << "CLD"; break;
        case I_SED: os << "SED"; break;
        case I_BPL: os << "BPL"; break;
        case I_BMI: os << "BMI"; break;
        case I_BVC: os << "BVC"; break;
        case I_BVS: os << "BVS"; break;
        case I_BCC: os << "BCC"; break;
        case I_BCS: os << "BCS"; break;
        case I_BNE: os << "BNE"; break;
        case I_BEQ: os << "BEQ"; break;
        case I_JMP: os << "JMP"; break;
        case I_PHP: os << "PHP"; break;
        case I_JSR: os << "JSR"; break;
        case I_PLP: os << "PLP"; break;
        case I_PHA: os << "PHA"; break;
        case I_RTS: os << "RTS"; break;
        case I_PLA: os << "PLA"; break;
        case I_ASL: os << "ASL"; break;
        case I_ASLA:os << "ASL"; break;
        case I_ROL: os << "ROL"; break;
        case I_ROLA:os << "ROL"; break;
        case I_LSR: os << "LSR"; break;
        case I_LSRA:os << "LSR"; break;
        case I_ROR: os << "ROR"; break;
        case I_RORA:os << "ROR"; break;
        case I_DEC: os << "DEC"; break;
        case I_DECA:os << "DEC"; break;
        case I_INC: os << "INC"; break;
        case I_INCA:os << "INC"; break;
        case I_BIT: os << "BIT"; break;
        case I_STX: os << "STX"; break;
        case I_LDX: os << "LDX"; break;
        case I_CPX: os << "CPX"; break;
        case I_INX: os << "INX"; break;
        case I_DEX: os << "DEX"; break;
        case I_TAX: os << "TAX"; break;
        case I_TXA: os << "TXA"; break;
        case I_TSX: os << "TSX"; break;
        case I_TXS: os << "TXS"; break;
        case I_STY: os << "STY"; break;
        case I_LDY: os << "LDY"; break;
        case I_CPY: os << "CPY"; break;
        case I_INY: os << "INY"; break;
        case I_DEY: os << "DEY"; break;
        case I_TAY: os << "TAY"; break;
        case I_TYA: os << "TYA"; break;
        case I_NOP: os << "NOP"; break;
        case I_BRK: os << "BRK"; break;
        case I_RTI: os << "RTI"; break;

        // Extra instructions for the 65C02
        case I_PHX: os << "PHX"; break;
        case I_PHY: os << "PHY"; break;
        case I_PLX: os << "PLX"; break;
        case I_PLY: os << "PLY"; break;
        case I_BRA: os << "BRA"; break;
        case I_BBR: os << "BBR"; break;
        case I_BBS: os << "BBS"; break;
        case I_STZ: os << "STZ"; break;
        case I_TRB: os << "TRB"; break;
        case I_TSB: os << "TSB"; break;
        case I_RMB: os << "RMB"; break;
        case I_SMB: os << "SMB"; break;
    }

    int len = 0;
    switch (addrModes[inst])
    {
        case AM_IMM  : os << " #$" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1); len = 2; break;
        case AM_ABS  : os << " $" << std::hex << std::setfill('0') << std::setw(4) << read16(pc+1); len = 3; break;
        case AM_ZP   : os << " $" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1); len = 2; break;
        case AM_NONE : len = 1; break;
        case AM_ACC  : os << " A"; len = 1; break;
        case AM_ZPX  : os << " $" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << ",X"; len = 2; break;
        case AM_ZPY  : os << " $" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << ",Y"; len = 2; break;
        case AM_ABSX : os << " $" << std::hex << std::setfill('0') << std::setw(4) << read16(pc+1) << ",X"; len = 3; break;
        case AM_ABSY : os << " $" << std::hex << std::setfill('0') << std::setw(4) << read16(pc+1) << ",Y"; len = 3; break;
        case AM_IND  : os << " ($" << std::hex << std::setfill('0') << std::setw(4) << read16(pc+1) << ")"; len = 3; break;
        case AM_INDX : os << " ($" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << ",X)"; len = 2; break;
        case AM_INDY : os << " ($" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << "),Y"; len = 2; break;
        case AM_REL  : os << " $" << std::hex << std::setfill('0') << std::setw(4) << ((pc + 2 + sign_extend(m_memory.read(pc+1))) & 0xFFFF); len = 2; break;

        case AM_IZP  : os << " ($" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << ")"; len = 2; break;
        case AM_IABSX: os << " ($" << std::hex << std::setfill('0') << std::setw(4) << read16(pc+1) << ",X)"; len = 3; break;
        case AM_ZR   : os << (uint16_t) ((m_memory.read(pc) >> 4) & 7);
                       os << " $" << std::hex << std::setfill('0') << std::setw(2) << (uint16_t) m_memory.read(pc+1) << ", ";
                       os << std::hex << std::setfill('0') << std::setw(4) << ((pc + 3 + sign_extend(m_memory.read(pc+2))) & 0xFFFF); len = 3; break;
        case AM_RES  : os << "???"; len = 1; break;
    } // switch (addrModes[inst])

    os << std::dec << std::endl;

    return len;
} // disas

