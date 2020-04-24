#pragma once

#include <ostream>
#include "memory.h"

class Cpu65C02
{
    public:
        Cpu65C02(Memory& memory) : m_memory(memory) {}

        // Disassemble the current instruction.
        int disas(std::ostream& os, uint16_t pc) const;

    private:
        Memory&  m_memory;  // Reference to the external memory.

        uint32_t m_instCounter;

        // Perform a 16-bit read from memory
        uint16_t read16(uint16_t addr) const {
            return (m_memory.read(addr+1) << 8) | m_memory.read(addr);
        }
}; // end of class Cpu65C02

