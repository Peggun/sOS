using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sOS.vm.src.Memory
{
    public class Memory
    {
        byte[] _memory;

        public Memory(byte size)
        {
            _memory = new byte[size];
        }

        public byte Read(int address)
        {
            return _memory[address];
        }
    }
}
