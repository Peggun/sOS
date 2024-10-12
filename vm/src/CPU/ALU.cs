using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace sOS.vm.src.CPU
{
    public class ALU
    {
        public void Add(byte r1, byte r2, ref byte rToSet)
        {
            int result = r1 + r2;
            if (result > 255)
            {
                result = result % 256;
            }
            rToSet = (byte)result;
        }
        public void Sub(byte r1, byte r2, ref byte rToSet)
        {
            int result = r1 - r2;
            if (result > 255)
            {
                result = result % 256;
            }
            rToSet = (byte)result;
        }
        public void Mul(byte r1, byte r2, ref byte rToSet)
        {
            int result = r1 * r2;
            if (result > 255)
            {
                result = result % 256;
            }
            rToSet = (byte)result;
        }
        public void Div(byte r1, byte r2, ref byte rToSet)
        {
            int result = r1 / r2;
            if (result > 255)
            {
                result = result % 256;
            }
            rToSet = (byte)result;
        }
        public void And(byte r1, byte r2, ref byte rToSet)
        {
            
        }
        public bool Or(bool a, bool b) => a || b;
        public bool Not(bool a) => !a;
    }
}
