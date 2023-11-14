#pragma once

#include <OS/File/PXDirectory.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	enum class FileElementInfoType
	{
		Invalid,
		Unkown, // DT_UNKNOWN

		File, // DT_REG
		Dictionary, // DT_DIR

		NamedPipeFIFO, // DT_FIFO
		Link, // DT_LNK
		Socket, // DT_SOCK

		DeviceCharacter, // DT_CHR
		DeviceBlock, // DT_BLK

		DictionaryRoot, // '.'
		DictionaryParent // '..'
	};

	class Directory
	{
		public:
		static PX::ActionResult Create(PX::Text& text);
		static PX::ActionResult Delete(PX::Text& text);
	};

	class DirectoryIterator : protected PXDirectoryIterator
	{
		public:
		PX::ActionResult Open(PX::Text& text);
		PX::ActionResult Next();
		PX::ActionResult Close();
	};
}