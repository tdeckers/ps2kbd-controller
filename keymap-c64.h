/* 
 * File:   keymap-c64.h
 * Author: tom
 *
 * Created on January 23, 2021, 11:14 AM
 */

#ifndef KEYMAP_C64_H
#define	KEYMAP_C64_H

#ifdef	__cplusplus
extern "C" {
#endif

static const unsigned char keymap[0x80] = {
	 		0x00,		// 00 no key pressed	
	 		0x89,		// 01 F9	
	 		0x88,		// 02 relocated F7	
	 		0x87,		// 03 F5	
	 		0x86,		// 04 F3	
	 		0x85,		// 05 F1	
	 		0x82,		// 06 F2	
	 		0x8C,		// 07 F12	
	 		0x00,		// 08	
	 		0x8A,		// 09 F10	
	 		0x88,		// 0A F8	
	 		0x86,		// 0B F6	
	 		0x84,		// 0C F4	
	 		0x09,		// 0D tab	
	 		0x60,		// 0E `~	
	 		0x8F,		// 0F relocated Print Screen key	
	 		0x03,		// 10 relocated Pause/Break key	
	 		0xA0,		// 11 left alt (right alt too)	
	 		0x00,		// 12 left shift	
	 		0x00,		// 13 
	 		0x00,		// 14 left ctrl (right ctrl too)	
	 		0x71,		// 15 qQ	
	 		0x31,		// 16 1!	
	 		0x00,		// 17	
	 		0x00,		// 18	
	 		0x00,		// 19	
	 		0x7A,		// 1A zZ	
	 		0x73,		// 1B sS	
	 		0x61,		// 1C aA	
	 		0x77,		// 1D wW	
	 		0x32,		// 1E 2@	
	 		0xA1,		// 1F Windows 98 menu key (left side)	
	 		0x00,		// 20 	
	 		0x63,		// 21 cC	
	 		0x78,		// 22 xX	
	 		0x64,		// 23 dD	
	 		0x65,		// 24 eE	
	 		0x34,		// 25 40x	
	 		0x33,		// 26 3#	
	 		0xA2,		// 27 Windows 98 menu key (right side)	
	 		0x00,		// 28	
	 		0x20,		// 29 space	
	 		0x76,		// 2A vV	
	 		0x66,		// 2B fF	
	 		0x74,		// 2C tT	
	 		0x72,		// 2D rR	
	 		0x35,		// 2E 5%	
	 		0xA3,		// 2F Windows 98 option key (right click, right side)	
	 		0x00,		// 30	
	 		0x6E,		// 31 nN	
	 		0x62,		// 32 bB	
	 		0x68,		// 33 hH	
	 		0x67,		// 34 gG	
	 		0x79,		// 35 yY	
	 		0x36,		// 36 6^	
	 		0xA4,		// 37 Power Key 	
	 		0x00,		// 38	
	 		0x00,		// 39	
	 		0x6D,		// 3A mM	
	 		0x6A,		// 3B jJ	
	 		0x75,		// 3C uU	
	 		0x37,		// 3D 7&	
	 		0x38,		// 3E 8*	
	 		0xA5,		// 3F Sleep Key	
	 		0x00,		// 40	
	 		0x2C,		// 41 ,<	
	 		0x6B,		// 42 kK	
	 		0x69,		// 43 iI	
	 		0x6F,		// 44 oO	
	 		0x30,		// 45 0)	
	 		0x39,		// 46 9(	
	 		0x00,		// 47	
	 		0x00,		// 48	
	 		0x2E,		// 49 .>	
	 		0x2F,		// 4A /?	
	 		0x6C,		// 4B lL	
	 		0x3B,		// 4C //:	
	 		0x70,		// 4D pP	
	 		0x2D,		// 4E -_	
	 		0x00,		// 4F	
	 		0x00,		// 50	
	 		0x00,		// 51	
	 		0x27,		// 52 '"	
	 		0x00,		// 53	
	 		0x5B,		// 54 [{	
	 		0x3D,		// 55 =+	
	 		0x00,		// 56	
	 		0x00,		// 57	
	 		0x00,		// 58 caps	
	 		0x00,		// 59 r shift	
	 		0x0D,		// 5A <Enter>	
	 		0x5D,		// 5B ]}	
	 		0x00,		// 5C	
	 		0x5C,		// 5D \|	
	 		0xA6,		// 5E Wake Key	
	 		0x00,		// 5F	
	 		0x00,		// 60	
	 		0x00,		// 61	
	 		0x00,		// 62	
	 		0x00,		// 63	
	 		0x00,		// 64	
	 		0x00,		// 65	
	 		0x14,		// 66 bkspace	
	 		0x00,		// 67	
	 		0x00,		// 68	
	 		0x31,		// 69 kp 1 {End}
	 		0x2f,		// 6A kp / converted from E04A in code	
	 		0x34,		// 6B kp 4 {Left}	
	 		0x13,		// 6C kp 7 {Home}	
	 		0x00,		// 6D	
	 		0x00,		// 6E	
	 		0x00,		// 6F	
	 		0x30,		// 70 kp 0 {Ins}	
	 		0x2E,		// 71 kp . {Del}	
	 		0x32,		// 72 kp 2 {Down}	
	 		0x35,		// 73 kp 5	
	 		0x36,		// 74 kp 6 {Right}	
	 		0x38,		// 75 kp 8 {Up}	
	 		0x03,		// 76 esc	
	 		0x00,		// 77 num lock	
	 		0x8B,		// 78 F11	
	 		0x2B,		// 79 kp +	
	 		0x33,		// 7A kp 3 {PgDn}	
	 		0x2D,		// 7B kp -	
	 		0x2A,		// 7C kp *	
	 		0x39,		// 7D kp 9 {PgUp}	
	 		0x8D,		// 7E scroll lock	
	 		0x00		// 7F
};

static const unsigned char keymap_shifted[0x80] = {
	 		0x00,		// 00 no key pressed	
	 		0xC9,		// 01 F9	
	 		0xC7,		// 02 relocated F7	
	 		0xC5,		// 03 F5	
	 		0xC3,		// 04 F3	
	 		0xC1,		// 05 F1	
	 		0xC2,		// 06 F2	
	 		0xCC,		// 07 F12	
	 		0x00,		// 08	
	 		0xCA,		// 09 F10	
	 		0xC8,		// 0A F8	
	 		0xC6,		// 0B F6	
	 		0xC4,		// 0C F4	
	 		0x09,		// 0D tab	
	 		0x7E,		// 0E `~	
	 		0xCF,		// 0F relocated Print Screen key	
	 		0x03,		// 10 relocated Pause/Break key	
	 		0xA0,		// 11 left alt (right alt too)	
	 		0x00,		// 12 left shift	
	 		0x00,		// 13 
	 		0x00,		// 14 left ctrl (right ctrl too)	
	 		0x51,		// 15 qQ	
	 		0x21,		// 16 1!	
	 		0x00,		// 17	
	 		0x00,		// 18	
	 		0x00,		// 19	
	 		0x5A,		// 1A zZ	
	 		0x53,		// 1B sS	
	 		0x41,		// 1C aA	
	 		0x57,		// 1D wW	
	 		0x40,		// 1E 2@	
	 		0xE1,		// 1F Windows 98 menu key (left side)	
	 		0x00,		// 20 	
	 		0x43,		// 21 cC	
	 		0x58,		// 22 xX	
	 		0x44,		// 23 dD	
	 		0x45,		// 24 eE	
	 		0x24,		// 25 40x	
	 		0x23,		// 26 3#	
	 		0xE2,		// 27 Windows 98 menu key (right side)	
	 		0x00,		// 28	
	 		0x20,		// 29 space	
	 		0x56,		// 2A vV	
	 		0x46,		// 2B fF	
	 		0x54,		// 2C tT	
	 		0x52,		// 2D rR	
	 		0x25,		// 2E 5%	
	 		0xE3,		// 2F Windows 98 option key (right click, right side)	
	 		0x00,		// 30	
	 		0x4E,		// 31 nN	
	 		0x42,		// 32 bB	
	 		0x48,		// 33 hH	
	 		0x47,		// 34 gG	
	 		0x59,		// 35 yY	
	 		0x5E,		// 36 6^	
	 		0xE4,		// 37 Power Key 	
	 		0x00,		// 38	
	 		0x00,		// 39	
	 		0x4D,		// 3A mM	
	 		0x4A,		// 3B jJ	
	 		0x55,		// 3C uU	
	 		0x26,		// 3D 7&	
	 		0x2A,		// 3E 8*	
	 		0xE5,		// 3F Sleep Key	
	 		0x00,		// 40	
	 		0x3C,		// 41 ,<	
	 		0x4B,		// 42 kK	
	 		0x49,		// 43 iI	
	 		0x4F,		// 44 oO	
	 		0x29,		// 45 0)	
	 		0x28,		// 46 9(	
	 		0x00,		// 47	
	 		0x00,		// 48	
	 		0x3E,		// 49 .>	
	 		0x3F,		// 4A /?	
	 		0x4C,		// 4B lL	
	 		0x3A,		// 4C ;:	
	 		0x50,		// 4D pP	
	 		0x5F,		// 4E -_	
	 		0x00,		// 4F	
	 		0x00,		// 50	
	 		0x00,		// 51	
	 		0x22,		// 52 '"	
	 		0x00,		// 53	
	 		0x7B,		// 54 [{	
	 		0x2B,		// 55 =+	
	 		0x00,		// 56	
	 		0x00,		// 57	
	 		0x00,		// 58 caps	
	 		0x00,		// 59 r shift	
	 		0x0D,		// 5A <Enter>	
	 		0x7D,		// 5B ]}	
	 		0x00,		// 5C	
	 		0x7C,		// 5D \|	
	 		0xE6,		// 5E Wake Key	
	 		0x00,		// 5F	
	 		0x00,		// 60	
	 		0x00,		// 61	
	 		0x00,		// 62	
	 		0x00,		// 63	
	 		0x00,		// 64	
	 		0x00,		// 65	
	 		0x14,		// 66 bkspace	
	 		0x00,		// 67	
	 		0x00,		// 68	
	 		0x91,		// 69 kp 1 {End}
	 		0x2f,		// 6A kp / converted from E04A in code	
	 		0x9D,		// 6B kp 4 {Left}	
	 		0x13,		// 6C kp 7 {Home}	
	 		0x00,		// 6D	
	 		0x00,		// 6E	
	 		0x00,		// 6F	
	 		0x94,		// 70 kp 0 {Ins}	
	 		0x14,		// 71 kp . {Del}	
	 		0x11,		// 72 kp 2 {Down}	
	 		0x95,		// 73 kp 5	
	 		0x1D,		// 74 kp 6 {Right}	
	 		0x91,		// 75 kp 8 {Up}	
	 		0x03,		// 76 esc	
	 		0x00,		// 77 num lock	
	 		0xCB,		// 78 F11	
	 		0x2B,		// 79 kp +	
	 		0x99,		// 7A kp 3 {PgDn}	
	 		0x2D,		// 7B kp -	
	 		0x2A,		// 7C kp *	
	 		0x19,		// 7D kp 9 {PgUp}	
	 		0xCD,		// 7E scroll lock	
	 		0x00		// 7F
};


#ifdef	__cplusplus
}
#endif

#endif	/* KEYMAP_C64_H */

