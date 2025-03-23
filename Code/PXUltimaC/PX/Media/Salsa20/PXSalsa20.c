#include "PXSalsa20.h"

#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

PXActionResult PXAPI PXSalsa20Encrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXSalsa20Decrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Salsa20",
        "Decrypt",
        "Section %i with %i bytes",
        pxSalsa20->DataSectionIndex,
        pxSalsa20->DataSize
    );
#endif

    // Read buffer

    //   byte[] decryptedData = decryptor.TransformFinalBlock(_reader.ReadBytes(size), 0, size);



    PXFileCursorAdvance(input, pxSalsa20->DataSize);

    return PXActionRefusedNotImplemented;
}
