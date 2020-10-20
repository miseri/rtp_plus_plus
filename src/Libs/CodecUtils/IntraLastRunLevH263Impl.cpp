/** @file

MODULE						: IntraLastRunLevH263Impl

TAG								: ILRLH263I

FILE NAME					: IntraLastRunLevH263Impl.cpp

DESCRIPTION				: A class to implement a last-run-level codec on the 
										2-D quantised dct coeffs of an Intra coded 8x8 block 
										as defined in the H.263 standard. It implements the 
										IRunLengthCodec interface.

REVISION HISTORY	: 16/11/2006 (KF): Modes of operation are added for the
									  Advanced Intra coding mode with different zigzag scans
										and the inclusion of the DC term or not.

COPYRIGHT					: 

RESTRICTIONS			: 
===========================================================================
*/
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdio.h>
#endif

#include "IntraLastRunLevH263Impl.h"
#include "LastRunLevelH263List.h"

typedef short coeffType;

/*
---------------------------------------------------------------------------
	Construction and Destruction.
---------------------------------------------------------------------------
*/
IntraLastRunLevH263Impl::IntraLastRunLevH263Impl(void)
{
	_mode			= NO_DC;
	_pZigzag	= zigZagPos;
}//end constructor.

IntraLastRunLevH263Impl::~IntraLastRunLevH263Impl(void)
{
}//end destructor.

/*
---------------------------------------------------------------------------
	Interface Methods.
---------------------------------------------------------------------------
*/
/** Set the mode of operation.
The default mode uses the standard zigzag and excludes the
DC term. The other modes are for the advanced Intra mode to
select differing zigzag orders and include the DC term.
@param mode	: Mode to set.
@return			: none.
*/
void IntraLastRunLevH263Impl::SetMode(int mode)
{
	switch(mode)
	{
		case NO_DC:
		case NORMAL_ZIGZAG:
			_pZigzag	= zigZagPos;
			break;
		case HORIZ_ZIGZAG:	// Vert. adjacent block uses horiz. scan.
			_pZigzag	= horizZigZagPos;
		case VERT_ZIGZAG:	// Horiz. adjacent block uses vert. scan.
			_pZigzag	= vertZigZagPos;
			break;
		default :	// Invalid mode.
			return;
			break;
	}//end switch mode...

	_mode = mode;

}//end SetMode.

/** Encode the input to a run-length structure.
Encode the input 2-D 8x8 block of Dct coeffs into the
output last-run-level struct list.
@param in		:	Input 8x8 block.
@param rle	:	Last-run-level struct generated by the encode.
@return			: none.
*/
void IntraLastRunLevH263Impl::Encode(void* in, void* rle)
{
	// Interpret the param list for this implementation.
	coeffType*						coeff		= (coeffType *)in;
	LastRunLevelH263List* lrlList = (LastRunLevelH263List *)rle;
	LastRunLevelH263*			lrl			= lrlList->GetList();
	// None encoded yet.
	lrlList->SetNumOfElements(0);
	int elements = 0;

	// Scanned in zigzag order and Intra (NO_DC) ignores DC coeff.
	int start = 0;
	if(_mode == NO_DC)
		start = 1;
	unsigned char run = 0; // Reset.
	for(int c = start; c < 64; c++)
	{
		coeffType x = coeff[_pZigzag[c]];
		if( x == 0)	// Most likely.
			run++;
		else				// Valid last-run-level.
		{
			lrl[elements].last	= 0;	// Assume not last until proven guilty.
			lrl[elements].run		= run;
			lrl[elements].level	= x;
			// Reset.
			run = 0;
			elements++;
		}//end else...
	}//end for c...
	// Set the last element and check that there were any non-zero elements.
	if(elements > 0)
		lrl[elements-1].last = 1;
	else	// Indicate no elements.
	{
		lrl[0].last		= 1;
		lrl[0].run		= run;
		lrl[0].level	= 0;
	}//end else...

	// Encoded length.
	lrlList->SetNumOfElements(elements);
}//end Encode.

/** Decode a run-length struct to the output.
Decode the input last-run-level struct list into the
output 2-D 8x8 block of Dct coeffs. Assumes there is
at least one element in the input list.
@param rle	:	Last-run-level struct to decode.
@param out	:	Output 8x8 block generated by the decode.
@return			: none.
*/
void IntraLastRunLevH263Impl::Decode(void* rle, void* out)
{
	// Interpret the param list for this implementation.
	LastRunLevelH263List* lrlList = (LastRunLevelH263List *)rle;
	LastRunLevelH263*			lrl			= lrlList->GetList();
	coeffType*						coeff		= (coeffType *)out;

	// Scanned in zigzag order and Intra (NO_DC) ignores DC coeff. Start
	// with first element and assume that it exists.
	int start = 0;
	if(_mode == NO_DC)
		start = 1;
	int						element = 0;
	unsigned char run			= lrl[0].run;
	unsigned char last		= lrl[0].last;
	coeffType			level		= lrl[0].level;
	for(int c = start; c < 64; c++)
	{
		if(run > 0) // Most likely.
		{
			coeff[_pZigzag[c]] = 0;
			run--;
		}//end if run...
		else				// Read element last-run-level and load next.
		{
			coeff[_pZigzag[c]] = level;
			if(!last)
			{
				// Next.
				element++;
				last	= lrl[element].last;
				run		= lrl[element].run;
				level	= lrl[element].level;
			}//end if !last...
			else
			{
				run = 255;	// Infinity.
			}//end else...
		}//end else...
	}//end for c...

}//end Decode.

/*
---------------------------------------------------------------------------
	Constants.
---------------------------------------------------------------------------
*/
const int IntraLastRunLevH263Impl::zigZagPos[64] =
{
	 0,	 1,	 8, 16,	 9,	 2,	 3,	10,
	17,	24,	32,	25,	18,	11,	 4,	 5,
	12,	19,	26,	33,	40,	48,	41,	34,
	27,	20,	13,	 6,	 7,	14,	21,	28,
	35,	42,	49,	56,	57,	50,	43,	36,
	29,	22,	15,	23,	30,	37,	44,	51,
	58,	59,	52,	45,	38,	31,	39,	46,
	53,	60,	61,	54,	47,	55,	62,	63
};

const int IntraLastRunLevH263Impl::horizZigZagPos[64] =
{
	 0,	 1,	 2,  3,	 8,	 9,	16,	17,
	10,	11,	 4,	 5,	 6,	 7,	15,	14,
	13,	12,	19,	18,	24,	25,	32,	33,
	26,	27,	20,	21,	22,	23,	28,	29,
	30,	31,	34,	35,	40,	41,	48,	49,
	42,	43,	36,	37,	38,	39,	44,	45,
	46,	47,	50,	51,	56,	57,	58,	59,
	52,	53,	54,	55,	60,	61,	62,	63
};

const int IntraLastRunLevH263Impl::vertZigZagPos[64] =
{
	 0,	 8,	16, 24,	 1,	 9,	 2,	10,
	17,	25,	32,	40,	48,	56,	57,	49,
	41,	33,	26,	18,	 3,	11,	 4,	12,
	19,	27,	34,	42,	50,	58,	35,	43,
	51,	59,	20,	28,	 5,	13,	 6,	14,
	21,	29,	36,	44,	52,	60,	37,	45,
	53,	61,	22,	30,	 7,	15,	23,	31,
	38,	46,	54,	62,	39,	47,	55,	63
};

