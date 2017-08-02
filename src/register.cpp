#include "register.h"

Registers::Registers()
{
    init();
}

void Registers::init()
{
    _pc  = 0;
    _ir  = 0;
    _lo  = 0;
    _hi  = 0;
    _exit = false;
    for (int i = 0; i < MAX_REG; ++i)
        _r[i] = 0;    
}

/* Set the $pc register to the value of the label 'main' */
void Registers::init(std::vector<Label>& labels)
{
    for (auto it = labels.begin(); it != labels.end(); ++it)
    {
        if (it->name == "main")
        {
            _pc = it->loc;
            break;
        }
    }
}

Register& Registers::operator[](int i)
{
    return i == pc ? _pc : i == lo ? _lo : i == hi ? _hi : _r[i];
}

bool Registers::get_exit() const
{
    return _exit;
}

Instruction Registers::get_ir() const
{
    return _ir;
}

void Registers::set_exit(bool e)
{
    _exit = e;
}

void Registers::set_ir(Instruction i)
{
    _ir = i;
}

std::ostream& operator<<(std::ostream& o, Registers& r)
{
    o << "   PC: " << std::setw(10) << r._pc      << ' '     
      << "   IR: " << std::setw(10) << r._ir      << '\n'
      << "$zero: " << std::setw(10) << r._r[zero] << ' '
      << "  $at: " << std::setw(10) << r._r[at]   << ' '
      << "  $v0: " << std::setw(10) << r._r[v0]   << ' '
      << "  $v1: " << std::setw(10) << r._r[v1]   << '\n'
      << "  $a0: " << std::setw(10) << r._r[a0]   << ' '
      << "  $a1: " << std::setw(10) << r._r[a1]   << ' '
      << "  $a2: " << std::setw(10) << r._r[a2]   << ' '
      << "  $a3: " << std::setw(10) << r._r[a3]   << '\n'
      << "  $t0: " << std::setw(10) << r._r[t0]   << ' '
      << "  $t1: " << std::setw(10) << r._r[t1]   << ' '
      << "  $t2: " << std::setw(10) << r._r[t2]   << ' '
      << "  $t3: " << std::setw(10) << r._r[t3]   << '\n'
      << "  $t4: " << std::setw(10) << r._r[t4]   << ' '
      << "  $t5: " << std::setw(10) << r._r[t5]   << ' '
      << "  $t6: " << std::setw(10) << r._r[t6]   << ' '
      << "  $t7: " << std::setw(10) << r._r[t7]   << '\n'
      << "  $s0: " << std::setw(10) << r._r[s0]   << ' '
      << "  $s1: " << std::setw(10) << r._r[s1]   << ' '
      << "  $s2: " << std::setw(10) << r._r[s2]   << ' '
      << "  $s3: " << std::setw(10) << r._r[s3]   << '\n'
      << "  $s4: " << std::setw(10) << r._r[s4]   << ' '
      << "  $s5: " << std::setw(10) << r._r[s5]   << ' '
      << "  $s6: " << std::setw(10) << r._r[s6]   << ' '
      << "  $s7: " << std::setw(10) << r._r[s7]   << '\n'
      << "  $t8: " << std::setw(10) << r._r[t8]   << ' '
      << "  $t9: " << std::setw(10) << r._r[t9]   << ' '
      << "  $k0: " << std::setw(10) << r._r[k0]   << ' '
      << "  $k1: " << std::setw(10) << r._r[k1]   << '\n'
      << "  $gp: " << std::setw(10) << r._r[gp]   << ' '
      << "  $sp: " << std::setw(10) << r._r[sp]   << ' '
      << "  $fp: " << std::setw(10) << r._r[fp]   << ' '
      << "  $ra: " << std::setw(10) << r._r[ra]   << '\n'
      << "  $lo: " << std::setw(10) << r._lo      << ' '
      << "  $hi: " << std::setw(10) << r._hi      << '\n';

    return o;
}