/*****************************************************************************
 *
 * tms70op.inc (Op code functions)
 *
 *****************************************************************************/

// addressing modes (not all opcodes have a write cycle)
#define WB_NO -1
#define AM_WB(write_func, address, param1, param2) \
	int result = (this->*op)(param1, param2); \
	if (result > WB_NO) write_func(address, result)

void Ctms7000::am_a(op_func op)
{
    info.m_icount -= 5;
	AM_WB(write_r8, 0, read_r8(0), 0);
}

void Ctms7000::am_b(op_func op)
{
    info.m_icount -= 5;
	AM_WB(write_r8, 1, read_r8(1), 0);
}

void Ctms7000::am_r(op_func op)
{
    info.m_icount -= 7;
	UINT8 r = imm8();
	AM_WB(write_r8, r, read_r8(r), 0);
}

void Ctms7000::am_a2a(op_func op)
{
    info.m_icount -= 6;
	AM_WB(write_r8, 0, read_r8(0), read_r8(0));
}

void Ctms7000::am_a2b(op_func op)
{
    info.m_icount -= 6;
	AM_WB(write_r8, 1, read_r8(1), read_r8(0));
}

void Ctms7000::am_a2p(op_func op)
{
    info.m_icount -= 10;
	UINT8 r = imm8();
	AM_WB(write_p, r, read_p(r), read_r8(0));
}

void Ctms7000::am_a2r(op_func op)
{
    info.m_icount -= 8;
	UINT8 r = imm8();
	AM_WB(write_r8, r, read_r8(r), read_r8(0));
}

void Ctms7000::am_b2a(op_func op)
{
    info.m_icount -= 5;
	AM_WB(write_r8, 0, read_r8(0), read_r8(1));
}

void Ctms7000::am_b2b(op_func op)
{
    info.m_icount -= 6;
	AM_WB(write_r8, 1, read_r8(1), read_r8(1));
}

void Ctms7000::am_b2r(op_func op)
{
    info.m_icount -= 7;
	UINT8 r = imm8();
	AM_WB(write_r8, r, read_r8(r), read_r8(1));
}

void Ctms7000::am_b2p(op_func op)
{
    info.m_icount -= 9;
	UINT8 r = imm8();
	AM_WB(write_p, r, read_p(r), read_r8(1));
}

void Ctms7000::am_r2a(op_func op)
{
    info.m_icount -= 8;
	AM_WB(write_r8, 0, read_r8(0), read_r8(imm8()));
}

void Ctms7000::am_r2b(op_func op)
{
    info.m_icount -= 8;
	AM_WB(write_r8, 1, read_r8(1), read_r8(imm8()));
}

void Ctms7000::am_r2r(op_func op)
{
    info.m_icount -= 10;
	UINT8 param2 = read_r8(imm8());
	UINT8 r = imm8();
	AM_WB(write_r8, r, read_r8(r), param2);
}

void Ctms7000::am_i2a(op_func op)
{
    info.m_icount -= 7;
	AM_WB(write_r8, 0, read_r8(0), imm8());
}

void Ctms7000::am_i2b(op_func op)
{
    info.m_icount -= 7;
	AM_WB(write_r8, 1, read_r8(1), imm8());
}

void Ctms7000::am_i2r(op_func op)
{
    info.m_icount -= 9;
	UINT8 param2 = imm8();
	UINT8 r = imm8();
	AM_WB(write_r8, r, read_r8(r), param2);
}

void Ctms7000::am_i2p(op_func op)
{
    info.m_icount -= 11;
	UINT8 param2 = imm8();
	UINT8 r = imm8();
	AM_WB(write_p, r, read_p(r), param2);
}

void Ctms7000::am_p2a(op_func op)
{
    info.m_icount -= 9;
	AM_WB(write_r8, 0, read_r8(0), read_p(imm8()));
}

void Ctms7000::am_p2b(op_func op)
{
    info.m_icount -= 8;
	AM_WB(write_r8, 1, read_r8(1), read_p(imm8()));
}



// common opcodes
// 1 param
int Ctms7000::op_clr(UINT8 param1, UINT8 param2)
{
	UINT8 t = 0;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_dec(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 - 1;
	SET_NZ(t);
	SET_C(~t);
	return t;
}

int Ctms7000::op_inc(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 + 1;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_inv(UINT8 param1, UINT8 param2)
{
	UINT8 t = ~param1;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_rl(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 << 1 | param1 >> 7;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_rlc(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 << 1 | GET_C();
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_rr(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 >> 1 | param1 << 8 | (param1 << 7 & 0x80);
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_rrc(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 >> 1 | param1 << 8 | GET_C() << 7;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_swap(UINT8 param1, UINT8 param2)
{
    info.m_icount -= 3;
	UINT16 t = param1 >> 4 | param1 << 4;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_xchb(UINT8 param1, UINT8 param2)
{
    info.m_icount -= 1;
	UINT8 t = read_r8(1);
	SET_CNZ(t);
	write_r8(1, param1);
	return t;
}

// 2 params
int Ctms7000::op_adc(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 + param2 + GET_C();
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_add(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 + param2;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_and(UINT8 param1, UINT8 param2)
{
	UINT8 t = param1 & param2;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_cmp(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 - param2;
	SET_NZ(t);
	SET_C(~t);
	return WB_NO;
}

int Ctms7000::op_mpy(UINT8 param1, UINT8 param2)
{
    info.m_icount -= 39;
	UINT16 t = param1 * param2;
	SET_CNZ(t >> 8 & 0xff);
	write_mem16(0, t); // always writes result to regs A-B
	return WB_NO;
}

int Ctms7000::op_mov(UINT8 param1, UINT8 param2)
{
	UINT8 t = param2;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_or(UINT8 param1, UINT8 param2)
{
	UINT8 t = param1 | param2;
	SET_CNZ(t);
	return t;
}

int Ctms7000::op_sbb(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 - param2 - (!GET_C());
	SET_NZ(t);
	SET_C(~t);
	return t;
}

int Ctms7000::op_sub(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 - param2;
	SET_NZ(t);
	SET_C(~t);
	return t;
}

int Ctms7000::op_xor(UINT8 param1, UINT8 param2)
{
	UINT8 t = param1 ^ param2;
	SET_CNZ(t);
	return t;
}

// BCD arthrimetic handling
static const UINT8 lut_bcd_out[6] = { 0x00, 0x06, 0x00, 0x66, 0x60, 0x66 };

int Ctms7000::op_dac(UINT8 param1, UINT8 param2)
{
    info.m_icount -= 2;
	int c = GET_C();

	UINT8 h1 = param1 >> 4 & 0xf;
	UINT8 l1 = param1 >> 0 & 0xf;
	UINT8 h2 = param2 >> 4 & 0xf;
	UINT8 l2 = param2 >> 0 & 0xf;

	// compute bcd constant
	UINT8 d = ((l1 + l2 + c) < 10) ? 0 : 1;
	if ((h1 + h2) == 9)
		d |= 2;
	else if ((h1 + h2) > 9)
		d |= 4;

	UINT8 t = param1 + param2 + c + lut_bcd_out[d];
	SET_CNZ(t);
	if (d > 2)
        info.m_sr |= SR_C;

	return t;
}

int Ctms7000::op_dsb(UINT8 param1, UINT8 param2)
{
    info.m_icount -= 2;
	int c = !GET_C();

	UINT8 h1 = param1 >> 4 & 0xf;
	UINT8 l1 = param1 >> 0 & 0xf;
	UINT8 h2 = param2 >> 4 & 0xf;
	UINT8 l2 = param2 >> 0 & 0xf;

	// compute bcd constant
	UINT8 d = ((l1 - c) >= l2) ? 0 : 1;
	if (h1 == h2)
		d |= 2;
	else if (h1 < h2)
		d |= 4;

	UINT8 t = param1 - param2 - c - lut_bcd_out[d];
	SET_CNZ(t);
	if (d <= 2)
        info.m_sr |= SR_C;

	return t;
}

// branches
inline void Ctms7000::shortbranch(bool check)
{
    info.m_icount -= 2;
	INT8 d = (INT8)imm8();

	if (check)
	{
        info.m_pc += d;
        info.m_icount -= 2;
	}
}

inline void Ctms7000::jmp(bool check)
{
    info.m_icount -= 3;
	shortbranch(check);
}

int Ctms7000::op_djnz(UINT8 param1, UINT8 param2)
{
	UINT16 t = param1 - 1;
	shortbranch(t != 0);
	return t;
}

int Ctms7000::op_btjo(UINT8 param1, UINT8 param2)
{
	UINT8 t = param1 & param2;
	SET_CNZ(t);
	shortbranch(t != 0);
	return WB_NO;
}

int Ctms7000::op_btjz(UINT8 param1, UINT8 param2)
{
	UINT8 t = ~param1 & param2;
	SET_CNZ(t);
	shortbranch(t != 0);
	return WB_NO;
}



// other opcodes
// dec double
void Ctms7000::decd_a()
{
    info.m_icount -= 9;
	UINT32 t = read_r16(0) - 1;
	write_r16(0, t);
	SET_NZ(t >> 8);
	SET_C(~(t >> 8));
}

void Ctms7000::decd_b()
{
    info.m_icount -= 9;
	UINT32 t = read_r16(1) - 1;
	write_r16(1, t);
	SET_NZ(t >> 8);
	SET_C(~(t >> 8));
}

void Ctms7000::decd_r()
{
    info.m_icount -= 11;
	UINT8 r = imm8();
	UINT32 t = read_r16(r) - 1;
	write_r16(r, t);
	SET_NZ(t >> 8);
	SET_C(~(t >> 8));
}

// cmpa extended
void Ctms7000::cmpa_dir()
{
    info.m_icount -= 12;
	UINT16 t = read_r8(0) - read_mem8(imm16());
	SET_NZ(t);
	SET_C(~t);
}

void Ctms7000::cmpa_inx()
{
    info.m_icount -= 14;
	UINT16 t = read_r8(0) - read_mem8(imm16() + read_r8(1));
	SET_NZ(t);
	SET_C(~t);
}

void Ctms7000::cmpa_ind()
{
    info.m_icount -= 11;
	UINT16 t = read_r8(0) - read_mem8(read_r16(imm8()));
	SET_NZ(t);
	SET_C(~t);
}

// lda extended
void Ctms7000::lda_dir()
{
    info.m_icount -= 11;
	UINT8 t = read_mem8(imm16());
	write_r8(0, t);
	SET_CNZ(t);
}

void Ctms7000::lda_inx()
{
    info.m_icount -= 13;
	UINT8 t = read_mem8(imm16() + read_r8(1));
	write_r8(0, t);
	SET_CNZ(t);
}

void Ctms7000::lda_ind()
{
    info.m_icount -= 10;
	UINT8 t = read_mem8(read_r16(imm8()));
	write_r8(0, t);
	SET_CNZ(t);
}

// sta extended
void Ctms7000::sta_dir()
{
    info.m_icount -= 11;
	UINT8 t = read_r8(0);
	write_mem8(imm16(), t);
	SET_CNZ(t);
}

void Ctms7000::sta_inx()
{
    info.m_icount -= 13;
	UINT8 t = read_r8(0);
	write_mem8(imm16() + read_r8(1), t);
	SET_CNZ(t);
}

void Ctms7000::sta_ind()
{
    info.m_icount -= 10;
	UINT8 t = read_r8(0);
	write_mem8(read_r16(imm8()), t);
	SET_CNZ(t);
}

// mov double
void Ctms7000::movd_dir()
{
    info.m_icount -= 15;
	UINT16 t = imm16();
	write_r16(imm8(), t);
	SET_CNZ(t >> 8 & 0xff);
}

void Ctms7000::movd_inx()
{
    info.m_icount -= 17;
	UINT16 t = imm16() + read_r8(1);
	write_r16(imm8(), t);
	SET_CNZ(t >> 8 & 0xff);
}

void Ctms7000::movd_ind()
{
    info.m_icount -= 14;
	UINT16 t = read_r16(imm8());
	write_r16(imm8(), t);
	SET_CNZ(t >> 8 & 0xff);
}

// long branch
void Ctms7000::br_dir()
{
    info.m_icount -= 10;
    info.m_pc = imm16();
}

void Ctms7000::br_inx()
{
    info.m_icount -= 12;
    info.m_pc = imm16() + read_r8(1);
}

void Ctms7000::br_ind()
{
    info.m_icount -= 9;
    info.m_pc = read_r16(imm8());
}

// call/return
void Ctms7000::call_dir()
{
    info.m_icount -= 14;
	UINT16 t = imm16();
    push16(info.m_pc);
    CallSubLevel++;
    info.m_pc = t;
}

void Ctms7000::call_inx()
{
    info.m_icount -= 16;
	UINT16 t = imm16() + read_r8(1);
    push16(info.m_pc);
    CallSubLevel++;
    info.m_pc = t;
}

void Ctms7000::call_ind()
{
    info.m_icount -= 13;
	UINT16 t = read_r16(imm8());
    push16(info.m_pc);
    CallSubLevel++;
    info.m_pc = t;
}

void Ctms7000::trap(UINT8 address)
{
    info.m_icount -= 14;
    push16(info.m_pc);
    CallSubLevel++;
    info.m_pc = read_mem16(0xff00 | address);
}

void Ctms7000::reti()
{
    info.m_icount -= 9;
    info.m_pc = pull16();
    CallSubLevel--;
    info.m_sr = pull8() & 0xf0;
	check_interrupts();
}

void Ctms7000::rets()
{
    info.m_icount -= 7;
    info.m_pc = pull16();
    CallSubLevel--;
}

// pop
void Ctms7000::pop_a()
{
    info.m_icount -= 6;
	UINT8 t = pull8();
	write_r8(0, t);
	SET_CNZ(t);
}

void Ctms7000::pop_b()
{
    info.m_icount -= 6;
	UINT8 t = pull8();
	write_r8(1, t);
	SET_CNZ(t);
}

void Ctms7000::pop_r()
{
    info.m_icount -= 8;
	UINT8 t = pull8();
	write_r8(imm8(), t);
	SET_CNZ(t);
}

void Ctms7000::pop_st()
{
    info.m_icount -= 6;
    info.m_sr = pull8() & 0xf0;
	check_interrupts();
}

// push
void Ctms7000::push_a()
{
    info.m_icount -= 6;
	UINT8 t = read_r8(0);
	push8(t);
	SET_CNZ(t);
}

void Ctms7000::push_b()
{
    info.m_icount -= 6;
	UINT8 t = read_r8(1);
	push8(t);
	SET_CNZ(t);
}

void Ctms7000::push_r()
{
    info.m_icount -= 8;
	UINT8 t = read_r8(imm8());
	push8(t);
	SET_CNZ(t);
}

void Ctms7000::push_st()
{
    info.m_icount -= 6;
    push8(info.m_sr);
}

// other
void Ctms7000::nop()
{
    info.m_icount -= 5;
}

void Ctms7000::idle()
{
//    qWarning()<<"IDLE";
    info.m_icount -= 6;
    info.m_pc--;
    info.m_idle_state = true;
}

void Ctms7000::dint()
{
    info.m_icount -= 5;
    info.m_sr &= ~(SR_N | SR_Z | SR_C | SR_I);
}

void Ctms7000::eint()
{
    info.m_icount -= 5;
    info.m_sr |= (SR_N | SR_Z | SR_C | SR_I);
	check_interrupts();
}

void Ctms7000::ldsp()
{
    info.m_icount -= 5;
    info.m_sp = read_r8(1);
}

void Ctms7000::stsp()
{
    info.m_icount -= 6;
    write_r8(1, info.m_sp);
}

void Ctms7000::setc()
{
    info.m_icount -= 5;
    info.m_sr = (info.m_sr & ~SR_N) | SR_C | SR_Z;
}

// not standard
void Ctms7020_exl::lvdp()
{
	/* on EXL100, opcode D7 ?? (SWAP R) was changed to LVDP, mostly equivalent to:
	* MOVP P40,xx
	* MOVP P36,A
	*/
    info.m_icount -= 10; // TODO: check real timing
	imm8(); // always 0x28? discarded?
	read_p(0x28);
	UINT8 t = read_p(0x24);
	write_r8(0, t);
	SET_CNZ(t);
}

// illegal opcode handling
void Ctms7000::illegal(UINT8 op)
{
    info.m_icount -= 5; // guessed
        qWarning()<<"illegal opcode "<< op <<"@"<< info.m_pc;
}
