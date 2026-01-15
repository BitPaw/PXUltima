#include "PXSalsa20.h"

#include <PX/OS/Error/PXResult.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

PXResult PXAPI PXSalsa20Encrypt(PXSalsa20 PXREF pxSalsa20, PXFile PXREF input, PXFile PXREF output)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXSalsa20Decrypt(PXSalsa20 PXREF pxSalsa20, PXFile PXREF input, PXFile PXREF output)
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
