#pragma once

#include <Media/ISBN/PXISBN.h>

namespace PX
{
	class ISBN : public PXISBN
	{
		public:
		ISBN();
		~ISBN();

		bool IsValid();
	};
}