/** @file

MODULE						: VectorQuantiserVlcEncoder

TAG								: VQVE

FILE NAME					: VectorQuantiserVlcEncoder.cpp

DESCRIPTION				: A class to implement a vector quantiser encoder where the
										table fully contains all indeces. There is no Esc coding
										requirements. It implements the IVlcEncoder interface.

REVISION HISTORY	:
									: 

COPYRIGHT					: (c)VICS 2000-2006  all rights resevered - info@videocoding.com

RESTRICTIONS			: The information/data/code contained within this file is 
										the property of VICS limited and has been classified as 
										CONFIDENTIAL.
===========================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include "VectorQuantiserVlcEncoder.h"

/*
---------------------------------------------------------------------------
	Constants.
---------------------------------------------------------------------------
*/
const int VectorQuantiserVlcEncoder::VLC_TABLE[VQVE_TABLE_SIZE][2] =
{
  {	 4,   0x000F }, //0
  {	 4,   0x0007 }, //1
  {	 4,   0x000B }, //2
  {	 4,   0x0003 }, //3
  {	 4,   0x0001 }, //4
  {	 4,   0x000A }, //5
  {	 4,   0x000C }, //6
  {	 5,   0x0019 }, //7
  {	 6,   0x0035 }, //8
  {	 6,   0x000E }, //9
  {	 6,   0x0032 }, //10
  {	 6,   0x0002 }, //11
  {	 6,   0x0024 }, //12
  {	 6,   0x0018 }, //13
  {	 6,   0x0010 }, //14
  {	 7,   0x003D }, //15
  {	 7,   0x005D }, //16
  {	 7,   0x000D }, //17
  {	 7,   0x0055 }, //18
  {	 7,   0x0065 }, //19
  {	 7,   0x0045 }, //20
  {	 7,   0x0049 }, //21
  {	 7,   0x003E }, //22
  {	 7,   0x0016 }, //23
  {	 7,   0x0046 }, //24
  {	 7,   0x0074 }, //25
  {	 7,   0x0034 }, //26
  {	 7,   0x0028 }, //27
  {	 7,   0x0040 }, //28
  {	 8,   0x006D }, //29
  {	 8,   0x002D }, //30
  {	 8,   0x0085 }, //31
  {	 8,   0x00E9 }, //32
  {	 8,   0x0069 }, //33
  {	 8,   0x0029 }, //34
  {	 8,   0x0089 }, //35
  {	 8,   0x00FE }, //36
  {	 8,   0x00DE }, //37
  {	 8,   0x001E }, //38
  {	 8,   0x006E }, //39
  {	 8,   0x0076 }, //40
  {	 8,   0x0036 }, //41
  {	 8,   0x00D6 }, //42
  {	 8,   0x00E6 }, //43
  {	 8,   0x00D2 }, //44
  {	 8,   0x0012 }, //45
  {	 8,   0x0062 }, //46
  {	 8,   0x0022 }, //47
  {	 8,   0x0054 }, //48
  {	 8,   0x0014 }, //49
  {	 8,   0x00C4 }, //50
  {	 8,   0x0004 }, //51
  {	 8,   0x0084 }, //52
  {	 8,   0x0078 }, //53
  {	 8,   0x00B8 }, //54
  {	 8,   0x0038 }, //55
  {	 8,   0x00C8 }, //56
  {	 8,   0x0048 }, //57
  {	 8,   0x00F0 }, //58
  {	 8,   0x0008 }, //59
  {	 8,   0x0070 }, //60
  {	 8,   0x0030 }, //61
  {	 8,   0x00A0 }, //62
  {	 8,   0x0080 }, //63
  {	 9,   0x01FD }, //64
  {	 9,   0x017D }, //65
  {	 9,   0x009D }, //66
  {	 9,   0x01AD }, //67
  {	 9,   0x00AD }, //68
  {	 9,   0x01CD }, //69
  {	 9,   0x00CD }, //70
  {	 9,   0x014D }, //71
  {	 9,   0x0095 }, //72
  {	 9,   0x0195 }, //73
  {	 9,   0x01A5 }, //74
  {	 9,   0x00A5 }, //75
  {	 9,   0x0125 }, //76
  {	 9,   0x0105 }, //77
  {	 9,   0x00A9 }, //78
  {	 9,   0x0009 }, //79
  {	 9,   0x005E }, //80
  {	 9,   0x009E }, //81
  {	 9,   0x00AE }, //82
  {	 9,   0x01AE }, //83
  {	 9,   0x012E }, //84
  {	 9,   0x01F6 }, //85
  {	 9,   0x00F6 }, //86
  {	 9,   0x01B6 }, //87
  {	 9,   0x0056 }, //88
  {	 9,   0x0156 }, //89
  {	 9,   0x00A6 }, //90
  {	 9,   0x0026 }, //91
  {	 9,   0x0186 }, //92
  {	 9,   0x0106 }, //93
  {	 9,   0x0006 }, //94
  {	 9,   0x0086 }, //95
  {	 9,   0x0052 }, //96
  {	 9,   0x0192 }, //97
  {	 9,   0x0092 }, //98
  {	 9,   0x00E2 }, //99
  {	 9,   0x01A2 }, //100
  {	 9,   0x0044 }, //101
  {	 9,   0x0144 }, //102
  {	 9,   0x00E8 }, //103
  {	 9,   0x0168 }, //104
  {	 9,   0x00B0 }, //105
  {	 9,   0x0088 }, //106
  {	 9,   0x0160 }, //107
  {	 9,   0x01E0 }, //108
  {	 9,   0x0060 }, //109
  {	 9,   0x0120 }, //110
  {	 9,   0x0000 }, //111
  {	10,   0x00FD }, //112
  {	10,   0x02FD }, //113
  {	10,   0x027D }, //114
  {	10,   0x039D }, //115
  {	10,   0x007D }, //116
  {	10,   0x031D }, //117
  {	10,   0x011D }, //118
  {	10,   0x001D }, //119
  {	10,   0x02ED }, //120
  {	10,   0x01ED }, //121
  {	10,   0x0215 }, //122
  {	10,   0x0315 }, //123
  {	10,   0x0115 }, //124
  {	10,   0x0225 }, //125
  {	10,   0x0005 }, //126
  {	10,   0x01A9 }, //127
  {	10,   0x037E }, //128
  {	10,   0x007E }, //129
  {	10,   0x0109 }, //130
  {	10,   0x027E }, //131
  {	10,   0x035E }, //132
  {	10,   0x039E }, //133
  {	10,   0x03EE }, //134
  {	10,   0x02EE }, //135
  {	10,   0x01EE }, //136
  {	10,   0x022E }, //137
  {	10,   0x002E }, //138
  {	10,   0x0366 }, //139
  {	10,   0x03A6 }, //140
  {	10,   0x0266 }, //141
  {	10,   0x0166 }, //142
  {	10,   0x0066 }, //143
  {	10,   0x01A6 }, //144
  {	10,   0x0352 }, //145
  {	10,   0x0126 }, //146
  {	10,   0x0152 }, //147
  {	10,   0x02A2 }, //148
  {	10,   0x01E2 }, //149
  {	10,   0x03D4 }, //150
  {	10,   0x00A2 }, //151
  {	10,   0x00D4 }, //152
  {	10,   0x0294 }, //153
  {	10,   0x01D4 }, //154
  {	10,   0x0194 }, //155
  {	10,   0x0394 }, //156
  {	10,   0x0094 }, //157
  {	10,   0x03F8 }, //158
  {	10,   0x02F8 }, //159
  {	10,   0x03E8 }, //160
  {	10,   0x00F8 }, //161
  {	10,   0x0268 }, //162
  {	10,   0x0388 }, //163
  {	10,   0x0068 }, //164
  {	10,   0x0188 }, //165
  {	10,   0x01B0 }, //166
  {	10,   0x02E0 }, //167
  {	10,   0x00E0 }, //168
  {	10,   0x0300 }, //169
  {	10,   0x0220 }, //170
  {	10,   0x0020 }, //171
  {	11,   0x059D }, //172
  {	11,   0x019D }, //173
  {	11,   0x061D }, //174
  {	11,   0x03ED }, //175
  {	11,   0x07ED }, //176
  {	11,   0x021D }, //177
  {	11,   0x04ED }, //178
  {	11,   0x064D }, //179
  {	11,   0x004D }, //180
  {	11,   0x024D }, //181
  {	11,   0x0415 }, //182
  {	11,   0x0425 }, //183
  {	11,   0x0015 }, //184
  {	11,   0x07A9 }, //185
  {	11,   0x0025 }, //186
  {	11,   0x0309 }, //187
  {	11,   0x03A9 }, //188
  {	11,   0x0709 }, //189
  {	11,   0x055E }, //190
  {	11,   0x017E }, //191
  {	11,   0x057E }, //192
  {	11,   0x015E }, //193
  {	11,   0x019E }, //194
  {	11,   0x00EE }, //195
  {	11,   0x04EE }, //196
  {	11,   0x00B6 }, //197
  {	11,   0x06B6 }, //198
  {	11,   0x04B6 }, //199
  {	11,   0x0326 }, //200
  {	11,   0x0726 }, //201
  {	11,   0x07E2 }, //202
  {	11,   0x03E2 }, //203
  {	11,   0x02D4 }, //204
  {	11,   0x05F8 }, //205
  {	11,   0x06D4 }, //206
  {	11,   0x01E8 }, //207
  {	11,   0x07B0 }, //208
  {	11,   0x03B0 }, //209
  {	11,   0x0500 }, //210
  {	11,   0x0100 }, //211
  {	12,   0x0C4D }, //212
  {	12,   0x08ED }, //213
  {	12,   0x044D }, //214
  {	12,   0x0A05 }, //215
  {	12,   0x0605 }, //216
  {	12,   0x0E05 }, //217
  {	12,   0x0205 }, //218
  {	12,   0x01F8 }, //219
  {	12,   0x05E8 }, //220
  {	13,   0x10ED }, //221
  {	13,   0x1D9E }, //222
  {	13,   0x1AB6 }, //223
  {	13,   0x0AB6 }, //224
  {	13,   0x159E }, //225
  {	13,   0x02B6 }, //226
  {	14,   0x32B6 }, //227
  {	14,   0x00ED }, //228
  {	14,   0x0DE8 }, //229
  {	14,   0x2DE8 }, //230
  {	14,   0x3DE8 }, //231
  {	14,   0x19F8 }, //232
  {	14,   0x39F8 }, //233
  {	14,   0x12B6 }, //234
  {	15,   0x20ED }, //235
  {	15,   0x459E }, //236
  {	15,   0x5DE8 }, //237
  {	15,   0x49F8 }, //238
  {	15,   0x69F8 }, //239
  {	15,   0x259E }, //240
  {	15,   0x0D9E }, //241
  {	15,   0x09F8 }, //242
  {	15,   0x4D9E }, //243
  {	15,   0x1DE8 }, //244
  {	15,   0x29F8 }, //245
  {	16,   0xE0ED }, //246
  {	16,   0x2D9E }, //247
  {	16,   0xAD9E }, //248
  {	16,   0x659E }, //249
  {	16,   0x6D9E }, //250
  {	16,   0xED9E }, //251
  {	16,   0x059E }, //252
  {	16,   0x859E }, //253
  {	16,   0xE59E }, //254
  {	16,   0x60ED } //255
};

/*
---------------------------------------------------------------------------
	Private Methods.
---------------------------------------------------------------------------
*/
int VectorQuantiserVlcEncoder::DoEncode(int index, int *codeWord)
{
  if(index < VQVE_TABLE_SIZE)
  {
    *codeWord = VLC_TABLE[index][VQVE_BIT_CODE];
    return( VLC_TABLE[index][VQVE_NUM_BITS] );
  }//end if index...
  else
  {
    *codeWord = 0;
    return(0);
  }//end else...
}//end DoEncode.

