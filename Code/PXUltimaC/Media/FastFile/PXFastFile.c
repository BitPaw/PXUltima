#include "PXFastFile.h"

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>
#include <Media/Salsa20/PXSalsa20.h>
#include <OS/Console/PXConsole.h>

const char PXSignature[8] = {'T', 'A', 'f' , 'f' , '0' , '1' , '0' , '0' };

const char PXFastFileKey[32] =
{
    0x64, 0x1D, 0x8A, 0x2F, 0xE3, 0x1D, 0x3A, 0xA6, 0x36, 0x22, 0xBB, 0xC9, 0xCE,
    0x85, 0x87, 0x22, 0x9D, 0x42, 0xB0, 0xF8, 0xED, 0x9B, 0x92, 0x41, 0x30, 0xBF,
    0x88, 0xB6, 0x5E, 0xDC, 0x50, 0xBE
};








void UpdateIVTable(int index, char* hash, char* ivTable, int* ivCounter)
{
    for(int i = 0; i < 20; i += 5)
    {
        int value = (index + 4 * ivCounter[index]) % 800 * 5;
        for(int x = 0; x < 5; x++)
            ivTable[4 * value + x + i] ^= hash[i + x];
    }
    ivCounter[index]++;
}

void GetIV(char* iv, int index, char* ivTable, int* ivCounter)
{
    int arrayIndex = (index + 4 * (ivCounter[index] - 1)) % 800 * 20;
   
    char* source = &ivTable[arrayIndex];

    PXCopyList(char, 8, source, iv);
    
  //  Array.Copy(ivTable, arrayIndex, iv, 0, 8);
}

void SetupIVCounter(int* ivCounter)
{
    for(int i = 0; i < 4; i++)
        ivCounter[i] = 1;
}

void FillIVTable(char* ivTable, int ivTableLength, char* nameKey)
{
    int nameKeyLength = PXTextLengthA(nameKey, 32);

    int addDiv = 0;
    for(int i = 0; i < ivTableLength; i += nameKeyLength * 4)
    {
        for(int x = 0; x < nameKeyLength * 4; x += 4)
        {
            if((i + addDiv) >= ivTableLength || i + x >= ivTableLength)
                return;

            if(x > 0)
                addDiv = x / 4;
            else
                addDiv = 0;

            for(int y = 0; y < 4; y++)
                ivTable[i + x + y] = nameKey[addDiv];
        }
    }
}








PXActionResult PXAPI PXFastFileLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "FastFile",
        "Parse",
        "--- Start ---"
    );
#endif


	PXFastFile pxFastFile;
	PXClear(PXFastFile, &pxFastFile);

	PXFile* const pxFile = pxResourceLoadInfo->FileReference;

	// Signature
	{
		const PXBool success = PXFileReadAndCompare(pxFile, PXSignature, 8);

		if(!success)
		{
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "FastFile",
                "Parse",
                "Signature invalid"
            );
#endif

			return PXActionRefusedInvalidHeaderSignature;
		}
	}
	
	PXFileReadI32U(pxFile, &pxFastFile.Version);
  
    switch(pxFastFile.Version)
    {
        case 147: // 0x97
        {
            // Skip the PHEEBs71 magic.
            const char buffer[] = {'P','H','E','E','B','s','7','1','\0','\0','\0','\0'};
            const PXBool equal = PXFileReadAndCompare(pxFile, buffer, 12);

            if(!equal)
            {
                return PXActionInvalid;
            }


            // Initialize the IV table.
            char ivTable[16000];
            int ivCounter[4];

            char name[32];
            PXFileReadB(pxFile, name, 32); // 0x20


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "FastFile",
                "Parse",
                "Name %s",
                name
            );
#endif

            FillIVTable(ivTable, 16000, name);
            SetupIVCounter(ivCounter);

            // Skip the RSA sig.
            //  _stream.Position += 0x100;
            PXFileCursorAdvance(pxFile, 0x100); 
           

            // Create a file containing the decompressed/decrypted zone file.
        

            PXFile outputFile;
            PXClear(PXFile, &outputFile);

            PXSalsa20 pxSalsa20;
            PXClear(PXSalsa20, &pxSalsa20);

            pxSalsa20.KeyData = PXFastFileKey;
            pxSalsa20.KeyLength = sizeof(PXFastFileKey);

            for(;;)
            {
                PXFileReadI32U(pxFile, &pxSalsa20.DataSize);

                const PXBool isLastSection = 0 == pxSalsa20.DataSize;

                if(isLastSection)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "FastFile",
                        "Parse",
                        "Final section reacherd"
                    );
#endif    
                    break;
                }
               

                // Get the IV for the current section.
                GetIV(pxSalsa20.IV, pxSalsa20.DataSectionIndex % 4, ivTable, ivCounter);

                // Generate a decryptor to decrypt the data.
               // var decryptor = salsa.CreateDecryptor();

                // Decrypt the data
                PXSalsa20Decrypt(&pxSalsa20, pxFile, &outputFile);


                // Uncompress the decrypted data.
                //byte[] uncompressedData = DeflateStream.UncompressBuffer(decryptedData);
                //outputStream.Write(uncompressedData, 0, uncompressedData.Length);

                // Calculate the SHA-1 of the decrypted data and update the IV table.
               // using (var sha1 = SHA1.Create())
               //     UpdateIVTable(sectionIndex % 4, sha1.ComputeHash(decryptedData), ivTable, ivCounter);

                ++(pxSalsa20.DataSectionIndex);
            }


            break;
        }

        default:
            break;
    }

    /*
    string path = CreateZoneFile();

    // Close the streams.
    _reader.Close();


    // Create new streams for the zone file.
    _stream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
    _reader = new BinaryReader(_stream);

    // Read the zone information.
    _stream.Position = 0x28;
    int numberOfStrings = _reader.ReadInt32();
    _stream.Position += 16;

    // Skip extra 8 bytes if string table is present.
    if(numberOfStrings != 0)
        _stream.Position += 8;

    // Skip the FF padding if a string table exists.
    byte b = _reader.ReadByte();
    while(b == 0xFF)
        b = _reader.ReadByte();
    _stream.Position--;

    // Read the string table (some reason it goes 1 over)
    for(int i = 0; i < numberOfStrings - 1; i++)
        Strings.Add(ReadASCIIZString(_reader));

    */
	return PXActionSuccessful;
}

PXActionResult PXAPI PXFastFileSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
	return PXActionRefusedNotImplemented;
}