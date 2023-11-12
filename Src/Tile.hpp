#pragma once

#include "Defines.hpp"

struct Tile
{
	Tile() {}
	Tile(U8 wall, U8 block, U8 decoration) : wall{ wall }, block{ block } {}

	U8& operator[] (U8 i) { return (&wall)[i]; }
	const U8& operator[] (U8 i) const { return (&wall)[i]; }

	U8 wall{ U8_MAX };
	U8 block{ U8_MAX };
	U8 decoration{ U8_MAX };
	U8 liquidAmt{ 0 };
};