#include <fstream>
#include "memory.h"
#include "cpu65c02.h"
#include "decode.h"

int main(int argc, const char *argv[])
{
   if (argc < 2)
   {
      std::cout << "Usage: 65c02-disassembler <file>" << std::endl;
      exit(-1);
   }

   // Read ROM contents from binary file.
   std::ifstream stream(argv[1], std::ios::in | std::ios::binary);
   std::vector<uint8_t> initial((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

   // Initialize memory and CPU
   Memory mem(initial);
   Cpu65C02 cpu65c02(mem);

   // Go into an infinite loop.
   // The program will terminate, when a JMP or B?? instruction jumps to itself.
   uint16_t pc = 0x0801;
   while (true)
   {
      std::cout << std::hex << std::setw(4) << std::setfill('0') << (uint16_t) pc << " : ";

      int len = instLen[mem.read(pc)];
      for (int i=0; i<3; ++i)
      {
         if (i<len)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (uint16_t) mem.read(pc+i) << " ";
         else
            std::cout << "   ";
      }
      std::cout << " ";

      cpu65c02.disas(std::cout, pc);
      pc += len;
   }
} // main

