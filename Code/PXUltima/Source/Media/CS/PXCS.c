#include "PXCS.h"

#include <Media/C/PXC.h>
#include <Text/PXText.h>
#include <Math/PXMath.h>

PXCSKeyWord PXCSFetchNext(PXDataStream* const inputSteam)
{
	PXCSKeyWord keyWord = PXCSKeyWordInvalid;

	{
		PXInt8U keyID = 0;

		PXDataStreamReadI8U(inputSteam, &keyID);

		keyWord = (PXInt8U)keyID;
	}

	return keyWord;
}

void PXCSSerialize(PXDataStream* const inputSteam, PXDataStream* const outputStream)
{
	while (!PXDataStreamIsAtEnd(inputSteam))
	{
		const PXCSKeyWord keyword = PXCSFetchNext(inputSteam);

		switch (keyword)
		{
			case PXCSKeyWordNameSpace:
			{
				PXDataStreamWriteB(outputStream, "namespace", 9);
				PXDataStreamWriteB(outputStream, "namespace", 9);
				PXDataStreamWriteB(outputStream, "{", 1);
			}

			default:
				break;
		}
	}
}

void PXCSCreateWrapperFromCSource(PXDataStream* const inputSteam, PXDataStream* const outputStream)
{
	PXDataStreamWriteB(outputStream, "namespace PX.Wrapper\n{\n", 23);

	while (!PXDataStreamIsAtEnd(inputSteam))
	{
		CKeyWord keyWord = CKeyWordInvalid;

		{
			PXInt8U keyID = 0;

			PXDataStreamReadI8U(inputSteam, &keyID);

			keyWord = (PXInt8U)keyID;
		}

		switch (keyWord)
		{
			case CKeyWordTypeDefinition:
			{
				PXInt8U enumNameBufferLength = 0;
				char enumNameBuffer[64];

				PXInt8U enumAliasBufferLength = 0;
				char enumAliasBuffer[64];

				PXInt8U enumElementBufferLength = 0;
				char enumElementBuffer[64];

				{
					PXInt8U keyID = 0;

					PXDataStreamReadI8U(inputSteam, &keyID);

					keyWord = (PXInt8U)keyID;
				}


				MemorySet(enumNameBuffer, 0, sizeof(enumNameBuffer));

				// Fake name
				PXDataStreamReadI8U(inputSteam, &enumNameBufferLength);
				PXDataStreamReadB(inputSteam, enumNameBuffer, enumNameBufferLength);

				MemorySet(enumNameBuffer, 0, sizeof(enumNameBuffer));

				// Alias
				PXDataStreamReadI8U(inputSteam, &enumNameBufferLength);
				PXDataStreamReadB(inputSteam, enumNameBuffer, enumNameBufferLength);


				PXInt16U amount = 0;

				PXDataStreamReadI16U(inputSteam, &amount);

				switch (keyWord)
				{
					case CKeyWordEnum:
					{
						PXDataStreamWriteB(outputStream, "\tpublic enum ", 13);
						PXDataStreamWriteB(outputStream, enumNameBuffer, enumNameBufferLength);
						PXDataStreamWriteB(outputStream, "\n\t{\n", 4);

						for (PXSize i = 0; i < amount; ++i)
						{						
							PXDataStreamReadI8U(inputSteam, &enumElementBufferLength);
							PXDataStreamReadB(inputSteam, enumElementBuffer, enumElementBufferLength);

							// if the name contains the same name as the enum, remove it

							PXBool minLength = MathMinimumIU(enumNameBufferLength, enumElementBufferLength);

							PXBool isSame = PXTextCompareA(enumNameBuffer, minLength, enumElementBuffer, minLength);

							PXSize offset = 0;

							if (isSame)
							{
								offset += enumNameBufferLength;
							}

							PXDataStreamWriteB(outputStream, "\t\t", 2);
							PXDataStreamWriteB(outputStream, enumElementBuffer+ offset, enumElementBufferLength- offset);

							if (i < (amount-1))
							{
								PXDataStreamWriteB(outputStream, ",\n", 2);
							}
							else
							{
								PXDataStreamWriteB(outputStream, "\n", 1);
							}
						}

						PXDataStreamWriteB(outputStream, "\t}", 1);

						break;
					}

					default:
						break;
				}
			}

			default:
				break;
		}
	
	}

	PXDataStreamWriteB(outputStream, "}", 2);

	outputStream->DataSize = outputStream->DataCursor;
	outputStream->DataCursor = 0;
}