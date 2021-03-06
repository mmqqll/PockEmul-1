#ifndef KEYB1280_H
#define KEYB1280_H

BYTE scandef_pc1280[] = {
//+0		+1			+2			+3			+4			+5			+6			+7
K_SQR,		K_LA,		K_DA,       K_RA,		K_UA,		'7',		K_BS,		K_CAL,
'(',		'i',		')',		'4',		'5',		'8',		K_DEL,		K_MOD,
NUL,		'k',		'o',		'p',		'6',		'9',		K_INS,		K_CAL_PER,
NUL,		' ',		'l',		'=',		'*',		'/',		K_MPLUS,	K_CLR,
NUL,		',',		'y',		';',		'$',		K_SML,		K_SHT,		K_CTRL,
NUL,		K_F1,		'h',		't',		K_F3,		'#',		'"',		'!',
NUL,		'%',		'u',		'j',		'r',		'e',		'w',		'q',
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,
NUL,		'm',		'n',		'g',		'f',		'd',		's',		'a',
NUL,		NUL,		K_F2,		'b',		'v',		'c',		'x',		'z',
NUL,		NUL,		NUL,		K_RET,		'1',		'2',		'3',		'-',
NUL,		NUL,		NUL,		K_F4,		'0',		'.',		K_CAL_RES,	'+',
NUL,		NUL,		NUL,		NUL,		NUL,		K_F5,		K_F6,		K_F7,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,
NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL,		NUL
};

TransMap KeyMap1280[]={
{1,	"  -         ",	'-',	542,	218,	2},		//OK
{2,	"  (         ",	'(',	542,	107,	2},		//OK
{3,	"  )         ",	')',	584,	107,	2},		//OK
{4,	"  *         ",	'*',	542,	181,	2},		//OK
{5,	"  ,         ",	',',	584,	218,	2},		//OK
{6,	"  .         ",	'.',	458,	218,	2},		//OK
{7,	"  /         ",	'/',	542,	144,	2},		//OK
{8,	"  :         ",	':',	584,	144,	2},		//OK
{9,	"  ;         ",	';',	584,	181,	2},		//OK
{11,	"  +         ",	'+',	500,	218,	2},		//OK
{12,	" CLS        ",	K_CLR,	556,	48,		10},		//OK
{13,	" DEF        ",	K_CTRL,	22,		189,	10},		//OK
{14,	" DEL        ",	K_DEL,	486,	77,		10},		//OK
{15,	" Down Arrow ",	K_DA,	416,	48,		10},		//OK
{16,	" ENTER      ",	K_RET,	336,	221,	3},		//OK
{17,	" Equal      ",	'=',	371,	189,	10},		//OK
{18,	" INS        ",	K_INS,	522,	77,		10},		//OK
{19,	" Left Arrow ",	K_LA ,	416,	77,		10},		//OK
{20,	" MODE       ",	K_MOD,	522,	48,		10},		//OK
{21,	" On/Brk     ",	K_BRK,	556,	77,		10},		//OK
{22,	" On/Off     ",	K_OF ,	594,	32,		4},		//OK
{23,	" Rigth Arrow",	K_RA ,	451,	77,		10},		//OK
{24,	" SHIFT1     ",	K_SHT,	571,	77,		10},
{25,	" SHIFT2     ",	K_SHT,	22,		157,	10},
{26,	" SML        ",	K_SML,	22,		221,	10},
{27,	" Space      ",	' ',	291,	221,	10},
{28,	" Up Arrow   ",	K_UA ,	451,	48,		10},		//OK
{29,	"0           ",	'0',	416,	218,	2},		//OK
{30,	"1           ",	'1',	416,	181,	2},		//OK
{31,	"2           ",	'2',	458,	181,	2},		//OK
{32,	"3           ",	'3',	500,	181,	2},		//OK
{33,	"4           ",	'4',	416,	144,	2},		//OK
{34,	"5           ",	'5',	458,	144,	2},		//OK
{35,	"6           ",	'6',	500,	144,	2},		//OK
{36,	"7           ",	'7',	416,	107,	2},		//OK
{37,	"8           ",	'8',	458,	107,	2},		//OK
{38,	"9           ",	'9',	500,	107,	2},		//OK
{39,	"A           ",	'a',	57,		189,	10},		//OK
{40,	"B           ",	'b',	190,	221,	10},
{41,	"C           ",	'c',	123,	221,	10},
{42,	"D           ",	'd',	123,	189,	10},
{43,	"E           ",	'e',	123,	157,	10},
{44,	"F           ",	'f',	156,	189,	10},
{45,	"G           ",	'g',	190,	189,	10},
{46,	"H           ",	'h',	224,	189,	10},
{47,	"I           ",	'i',	291,	157,	10},
{48,	"J           ",	'j',	257,	189,	10},
{49,	"K           ",	'k',	291,	189,	10},
{50,	"L           ",	'l',	325,	189,	10},
{51,	"M           ",	'm',	257,	221,	10},
{52,	"N           ",	'n',	224,	221,	10},
{53,	"O           ",	'o',	325,	157,	10},
{54,	"P           ",	'p',	359,	157,	10},
{55,	"Q           ",	'q',	57,		157,	10},		//OK
{56,	"R           ",	'r',	156,	157,	10},
{57,	"S           ",	's',	88,		189,	10},
{58,	"T           ",	't',	190,	157,	10},
{59,	"U           ",	'u',	257,	157,	10},
{60,	"V           ",	'v',	156,	221,	10},
{61,	"W           ",	'w',	88,		157,	10},
{62,	"X           ",	'x',	88,		221,	10},
{63,	"Y           ",	'y',	224,	157,	10},
{64,	"Z           ",	'z',	57,		221,	10},		//OK
{65,	"CAL         ",	K_CAL,	57,		221,	10},
{66,	"CLOSE       ",	K_CLOSE,57,		221,	10}
};
int KeyMap1280Lenght = 66;


#endif // KEYB1280_H
