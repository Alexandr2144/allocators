#include "data/math.h"


namespace Data {
	int log2(int x)
	{
		int r = (((x > 0xFFFF)) ? 0x10 : 0);
		x >>= r;

		int shift = ((x > 0xFF) ? 0x8 : 0);
		x >>= shift;
		r |= shift;

		shift = ((x > 0xF) ? 0x4 : 0);
		x >>= shift;

		r |= shift;
		shift = ((x > 0x3) ? 0x2 : 0);

		x >>= shift;
		r |= shift;
		r |= (x >> 1);
		return r;
	}

} // namespace Data