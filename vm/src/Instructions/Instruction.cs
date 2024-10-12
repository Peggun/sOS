using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sOS.vm.src.Instructions
{
    public enum Instruction
    {
        Add = 0x01,
        Sub = 0x02,
        Mul = 0x03,
        Div = 0x04,
        And = 0x05,
        Or = 0x06,
        Not = 0x07
    }
}
