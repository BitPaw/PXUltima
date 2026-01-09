#include "PXADAM7.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Error/PXActionResult.h>

/*Try the code, if it returns error, also return the error.*/
#define CERROR_TRY_RETURN(call){\
  unsigned error = call;\
  if(error) return error;\
}

/*shared values used by multiple Adam7 related functions*/

static const unsigned PXADAM7_IX[7] = { 0, 4, 0, 2, 0, 1, 0 }; /*x start values*/
static const unsigned PXADAM7_IY[7] = { 0, 0, 4, 0, 2, 0, 1 }; /*y start values*/
static const unsigned PXADAM7_DX[7] = { 8, 8, 4, 4, 2, 2, 1 }; /*x delta values*/
static const unsigned PXADAM7_DY[7] = { 8, 8, 8, 4, 4, 2, 2 }; /*y delta values*/

PXResult PXAPI PXADAM7ScanlinesDecode(PXADAM7 PXREF pxADAM7)
{
    /*
     This function converts the filtered-padded-interlaced data into pure 2D image buffer with the PNG's colortype.
     Steps:
     *) if no Adam7: 1) unfilter 2) remove padding bits (= possible extra bits per scanline if bpp < 8)
     *) if adam7: 1) 7x unfilter 2) 7x remove padding bits 3) Adam7_deinterlace
     NOTE: the in buffer will be overwritten with intermediate data!
     */

    // Input check
    {
        const PXBool isValidInput = pxADAM7->DataInput && pxADAM7->DataOutput && pxADAM7->Width && pxADAM7->Height && pxADAM7->BitsPerPixel;

        if(!isValidInput)
            return PXResultRefusedParameterInvalid; // error: invalid colortype
    }

    switch(pxADAM7->InterlaceMethod)
    {
    default:
    case PXPNGInterlaceInvalid:
        return PXResultRefusedParameterInvalid;

    case PXPNGInterlaceNone:
    {
        const PXBool additionalStep = pxADAM7->BitsPerPixel < 8 && pxADAM7->Width * pxADAM7->BitsPerPixel != ((pxADAM7->Width * pxADAM7->BitsPerPixel + 7u) / 8u) * 8u;

        if(additionalStep)
        {
            void* oldPosition = pxADAM7->DataOutput;

            pxADAM7->DataOutput = pxADAM7->DataInput;

            const PXResult pxActionResult = PXADAM7unfilter(pxADAM7);

            pxADAM7->DataOutput = (PXByte*)oldPosition;


            PXADAM7removePaddingBits
            (
                pxADAM7->DataOutput,
                pxADAM7->DataInput,
                pxADAM7->Width * pxADAM7->BitsPerPixel,
                ((pxADAM7->Width * pxADAM7->BitsPerPixel + 7u) / 8u) * 8u,
                pxADAM7->Height
            );
        }
        else
        {
            // we can immediately filter into the out buffer, no other steps needed

            const PXResult pxActionResult = PXADAM7unfilter(pxADAM7);
        }

        break;
    }
    case PXPNGInterlaceADAM7:
    {
        unsigned passw[7], passh[7];
        PXSize filter_passstart[8], padded_passstart[8], passstart[8];

        PXADAM7_getpassvalues
        (
            passw,
            passh,
            filter_passstart,
            padded_passstart,
            passstart,
            pxADAM7->Width,
            pxADAM7->Height,
            pxADAM7->BitsPerPixel
        );

        for(PXSize i = 0; i != 7u; ++i)
        {
            PXADAM7 pxADAM7sub;
            pxADAM7sub.DataInput = &pxADAM7->DataInput[filter_passstart[i]]; // Maybe wroong order?
            pxADAM7sub.InputSize;
            pxADAM7sub.DataOutput = &pxADAM7->DataInput[padded_passstart[i]];
            pxADAM7sub.OutputSize;
            pxADAM7sub.Width = passw[i];
            pxADAM7sub.Height = passh[i];
            pxADAM7sub.BitsPerPixel = pxADAM7->BitsPerPixel;
            pxADAM7sub.InterlaceMethod = pxADAM7->InterlaceMethod;

            const PXResult pxActionResult = PXADAM7unfilter(&pxADAM7sub);
            // TODO: possible efficiency improvement: if in this reduced image the bits fit nicely in 1 scanline,
            // move bytes instead of bits or move not at all

            if(pxADAM7->BitsPerPixel < 8)
            {
                // remove padding bits in scanlines; after this there still may be padding
                // bits between the different reduced images: each reduced image still starts nicely at a byte
                PXADAM7removePaddingBits
                (
                    &((PXByte*)pxADAM7->DataInput)[passstart[i]],
                    &((PXByte*)pxADAM7->DataInput)[padded_passstart[i]],
                    passw[i] * pxADAM7->BitsPerPixel,
                    ((passw[i] * pxADAM7->BitsPerPixel + 7u) / 8u) * 8u,
                    passh[i]
                );
            }
        }

        PXADAM7Deinterlace(pxADAM7);

        break;
    }
    }

    return PXResultOK;
}

PXResult PXAPI PXADAM7ScanlinesEncode(PXADAM7 PXREF pxADAM7)
{
    return PXActionRefusedNotImplemented;
}

PXSize PXAPI PXADAM7CaluclateExpectedSize(PXADAM7 PXREF pxADAM7)
{
    const PXSize n = pxADAM7->Width * pxADAM7->Height;

    return ((n / 8u) * pxADAM7->BitsPerPixel) + ((n & 7u) * pxADAM7->BitsPerPixel + 7u) / 8u;
}

unsigned char PXAPI PXADAM7paethPredictor(short a, short b, short c)
{
    short pa = PXMathAbsoluteI16(b - c);
    short pb = PXMathAbsoluteI16(a - c);
    short pc = PXMathAbsoluteI16(a + b - c - c);
    /* return input value associated with smallest of pa, pb, pc (with certain priority if equal) */
    if(pb < pa)
    {
        a = b;
        pa = pb;
    }

    return (pc < pa) ? c : a;
}

PXResult PXAPI PXADAM7unfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon, PXSize bytewidth, unsigned char filterType, PXSize length)
{
    /*
    For PNG filter method 0
    unfilter a PNG image scanline by scanline. when the pixels are smaller than 1 byte,
    the filter works byte per byte (bytewidth = 1)
    precon is the previous unfiltered scanline, recon the result, scanline the current one
    the incoming scanlines do NOT Included the filtertype byte, that one is given in the parameter filterType instead
    recon and scanline MAY be the same memory address! precon must be disjoint.
    */

    switch(filterType)
    {
    case 0:
        for(PXSize i = 0; i != length; ++i) recon[i] = scanline[i];
        break;
    case 1:
    {
        PXSize j = 0;
        for(PXSize i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
        for(PXSize i = bytewidth; i != length; ++i, ++j) recon[i] = scanline[i] + recon[j];
        break;
    }
    case 2:
        if(precon)
        {
            for(PXSize i = 0; i != length; ++i) recon[i] = scanline[i] + precon[i];
        }
        else
        {
            for(PXSize i = 0; i != length; ++i) recon[i] = scanline[i];
        }
        break;
    case 3:
        if(precon)
        {
            PXSize j = 0;
            PXSize i = 0;
            for(i = 0; i != bytewidth; ++i) recon[i] = scanline[i] + (precon[i] >> 1u);
            /* Unroll independent paths of this predictor. A 6x and 8x version is also possible but that adds
            too much code. Whether this speeds up anything depends on compiler and settings. */
            if(bytewidth >= 4)
            {
                for(; i + 3 < length; i += 4, j += 4)
                {
                    unsigned char s0 = scanline[i + 0], r0 = recon[j + 0], p0 = precon[i + 0];
                    unsigned char s1 = scanline[i + 1], r1 = recon[j + 1], p1 = precon[i + 1];
                    unsigned char s2 = scanline[i + 2], r2 = recon[j + 2], p2 = precon[i + 2];
                    unsigned char s3 = scanline[i + 3], r3 = recon[j + 3], p3 = precon[i + 3];
                    recon[i + 0] = s0 + ((r0 + p0) >> 1u);
                    recon[i + 1] = s1 + ((r1 + p1) >> 1u);
                    recon[i + 2] = s2 + ((r2 + p2) >> 1u);
                    recon[i + 3] = s3 + ((r3 + p3) >> 1u);
                }
            }
            else if(bytewidth >= 3)
            {
                for(; i + 2 < length; i += 3, j += 3)
                {
                    unsigned char s0 = scanline[i + 0], r0 = recon[j + 0], p0 = precon[i + 0];
                    unsigned char s1 = scanline[i + 1], r1 = recon[j + 1], p1 = precon[i + 1];
                    unsigned char s2 = scanline[i + 2], r2 = recon[j + 2], p2 = precon[i + 2];
                    recon[i + 0] = s0 + ((r0 + p0) >> 1u);
                    recon[i + 1] = s1 + ((r1 + p1) >> 1u);
                    recon[i + 2] = s2 + ((r2 + p2) >> 1u);
                }
            }
            else if(bytewidth >= 2)
            {
                for(; i + 1 < length; i += 2, j += 2)
                {
                    unsigned char s0 = scanline[i + 0], r0 = recon[j + 0], p0 = precon[i + 0];
                    unsigned char s1 = scanline[i + 1], r1 = recon[j + 1], p1 = precon[i + 1];
                    recon[i + 0] = s0 + ((r0 + p0) >> 1u);
                    recon[i + 1] = s1 + ((r1 + p1) >> 1u);
                }
            }
            for(; i != length; ++i, ++j) recon[i] = scanline[i] + ((recon[j] + precon[i]) >> 1u);
        }
        else
        {
            PXSize j = 0;
            for(PXSize i = 0; i != bytewidth; ++i) recon[i] = scanline[i];
            for(PXSize i = bytewidth; i != length; ++i, ++j) recon[i] = scanline[i] + (recon[j] >> 1u);
        }
        break;
    case 4:
        if(precon)
        {
            PXSize j = 0;
            PXSize i = 0;
            for(i = 0; i != bytewidth; ++i)
            {
                recon[i] = (scanline[i] + precon[i]); /*paethPredictor(0, precon[i], 0) is always precon[i]*/
            }

            /* Unroll independent paths of the paeth predictor. A 6x and 8x version is also possible but that
            adds too much code. Whether this speeds up anything depends on compiler and settings. */
            if(bytewidth >= 4)
            {
                for(; i + 3 < length; i += 4, j += 4)
                {
                    unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2], s3 = scanline[i + 3];
                    unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2], r3 = recon[j + 3];
                    unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2], p3 = precon[i + 3];
                    unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2], q3 = precon[j + 3];
                    recon[i + 0] = s0 + PXADAM7paethPredictor(r0, p0, q0);
                    recon[i + 1] = s1 + PXADAM7paethPredictor(r1, p1, q1);
                    recon[i + 2] = s2 + PXADAM7paethPredictor(r2, p2, q2);
                    recon[i + 3] = s3 + PXADAM7paethPredictor(r3, p3, q3);
                }
            }
            else if(bytewidth >= 3)
            {
                for(; i + 2 < length; i += 3, j += 3)
                {
                    unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1], s2 = scanline[i + 2];
                    unsigned char r0 = recon[j + 0], r1 = recon[j + 1], r2 = recon[j + 2];
                    unsigned char p0 = precon[i + 0], p1 = precon[i + 1], p2 = precon[i + 2];
                    unsigned char q0 = precon[j + 0], q1 = precon[j + 1], q2 = precon[j + 2];
                    recon[i + 0] = s0 + PXADAM7paethPredictor(r0, p0, q0);
                    recon[i + 1] = s1 + PXADAM7paethPredictor(r1, p1, q1);
                    recon[i + 2] = s2 + PXADAM7paethPredictor(r2, p2, q2);
                }
            }
            else if(bytewidth >= 2)
            {
                for(; i + 1 < length; i += 2, j += 2)
                {
                    unsigned char s0 = scanline[i + 0], s1 = scanline[i + 1];
                    unsigned char r0 = recon[j + 0], r1 = recon[j + 1];
                    unsigned char p0 = precon[i + 0], p1 = precon[i + 1];
                    unsigned char q0 = precon[j + 0], q1 = precon[j + 1];
                    recon[i + 0] = s0 + PXADAM7paethPredictor(r0, p0, q0);
                    recon[i + 1] = s1 + PXADAM7paethPredictor(r1, p1, q1);
                }
            }

            for(; i != length; ++i, ++j)
            {
                recon[i] = (scanline[i] + PXADAM7paethPredictor(recon[i - bytewidth], precon[i], precon[j]));
            }
        }
        else
        {
            PXSize j = 0;
            for(PXSize i = 0; i != bytewidth; ++i)
            {
                recon[i] = scanline[i];
            }
            for(PXSize i = bytewidth; i != length; ++i, ++j)
            {
                /*paethPredictor(recon[i - bytewidth], 0, 0) is always recon[i - bytewidth]*/
                recon[i] = (scanline[i] + recon[j]);
            }
        }
        break;
    default:
        return PXResultInvalid; /*error: invalid filter type given*/
    }
    return PXResultOK;
}

PXSize PXAPI PXADAM7lodepng_get_raw_size_idat(PXSize w, PXSize h, PXSize bpp)
{
    /* + 1 for the filter byte, and possibly plus padding bits per line. */
    /* Ignoring casts, the expression is equal to (w * bpp + 7) / 8 + 1, but avoids overflow of w * bpp */
    PXSize line = ((PXSize)(w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u;
    return (PXSize)h * line;
}

PXResult PXAPI PXADAM7unfilter(PXADAM7 PXREF pxADAM7)
{
    /*
    For PNG filter method 0
    this function unfilters a single image (e.g. without interlacing this is called once, with Adam7 seven times)
    out must have enough bytes allocated already, in must have the scanlines + 1 filtertype byte per scanline
    w and h are image dimensions or dimensions of reduced image, bpp is bits per pixel
    in and out are allowed to be the same memory address (but aren't the same size since in has the extra filter bytes)
    */


    /*bytewidth is used for filtering, is 1 when bpp < 8, number of bytes per pixel otherwise*/
    const PXSize bytewidth = (pxADAM7->BitsPerPixel + 7u) / 8u;
    /*the width of a scanline in bytes, not including the filter type*/
    const PXSize linebytes = PXADAM7lodepng_get_raw_size_idat(pxADAM7->Width, 1, pxADAM7->BitsPerPixel) - 1u;

    PXByte* prevline = 0;

    for(PXSize y = 0; y < pxADAM7->Height; ++y)
    {
        const PXSize outindex = linebytes * y;
        const PXSize inindex = (1 + linebytes) * y; /*the extra filterbyte added to each row*/
        const PXByte filterType = pxADAM7->DataInput[inindex];

        const PXResult pxActionResult = PXADAM7unfilterScanline
        (
            &pxADAM7->DataOutput[outindex],
            &pxADAM7->DataInput[inindex + 1], 
            prevline,
            bytewidth,
            filterType, 
            linebytes
        );

        prevline = &pxADAM7->DataOutput[outindex];
    }

    return PXResultOK;
}

void PXAPI PXADAM7removePaddingBits(unsigned char* out, const unsigned char* in, PXSize olinebits, PXSize ilinebits, PXSize h)
{
    /*
    After filtering there are still padding bits if scanlines have non multiple of 8 bit amounts. They need
    to be removed (except at last scanline of (Adam7-reduced) image) before working with pure image buffers
    for the Adam7 code, the color convert code and the output to the user.
    in and out are allowed to be the same buffer, in may also be higher but still overlapping; in must
    have >= ilinebits*h bits, out must have >= olinebits*h bits, olinebits must be <= ilinebits
    also used to move bits after earlier such operations happened, e.g. in a sequence of reduced images from Adam7
    only useful if (ilinebits - olinebits) is a value in the range 1..7
    */

    PXSize diff = ilinebits - olinebits;
    PXSize ibp = 0;
    PXSize obp = 0; /*input and output bit pointers*/

    for(PXSize y = 0; y < h; ++y)
    {
        for(PXSize x = 0; x < olinebits; ++x)
        {
            unsigned char bit = readBitFromReversedStream(&ibp, in);
            PXADAM7setBitOfReversedStream(&obp, out, bit);
        }
        ibp += diff;
    }
}

unsigned char PXAPI PXADAM7readBitFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream)
{
    unsigned char result = (unsigned char)((bitstream[(*bitpointer) >> 3] >> (7 - ((*bitpointer) & 0x7))) & 1);
    ++(*bitpointer);
    return result;
}

void PXAPI PXADAM7setBitOfReversedStream(PXSize* bitpointer, unsigned char* bitstream, unsigned char bit)
{
    /*the current bit in bitstream may be 0 or 1 for this to work*/
    if(bit == 0) bitstream[(*bitpointer) >> 3u] &= (unsigned char)(~(1u << (7u - ((*bitpointer) & 7u))));
    else         bitstream[(*bitpointer) >> 3u] |= (1u << (7u - ((*bitpointer) & 7u)));
    ++(*bitpointer);
}

void PXAPI PXADAM7_getpassvalues(unsigned passw[7], unsigned passh[7], PXSize filter_passstart[8], PXSize padded_passstart[8], PXSize passstart[8], PXSize w, PXSize h, PXSize bpp)
{


    /*the passstart values have 8 values: the 8th one indicates the byte after the end of the 7th (= last) pass*/
    //unsigned i;

    /*calculate width and height in pixels of each pass*/
    for(PXSize i = 0; i != 7; ++i)
    {
        passw[i] = (w + PXADAM7_DX[i] - PXADAM7_IX[i] - 1) / PXADAM7_DX[i];
        passh[i] = (h + PXADAM7_DY[i] - PXADAM7_IY[i] - 1) / PXADAM7_DY[i];
        if(passw[i] == 0) passh[i] = 0;
        if(passh[i] == 0) passw[i] = 0;
    }

    filter_passstart[0] = padded_passstart[0] = passstart[0] = 0;

    for(PXSize i = 0; i != 7; ++i)
    {
        /*if passw[i] is 0, it's 0 bytes, not 1 (no filtertype-byte)*/
        filter_passstart[i + 1] = filter_passstart[i] + ((passw[i] && passh[i]) ? passh[i] * (1u + (passw[i] * bpp + 7u) / 8u) : 0);
        /*bits padded if needed to fill full byte at end of each scanline*/
        padded_passstart[i + 1] = padded_passstart[i] + passh[i] * ((passw[i] * bpp + 7u) / 8u);
        /*only padded at end of reduced image*/
        passstart[i + 1] = passstart[i] + (passh[i] * passw[i] * bpp + 7u) / 8u;
    }
}

void PXAPI PXADAM7Deinterlace(PXADAM7 PXREF pxADAM7)
{
    unsigned int passw[7];
    unsigned int passh[7];
    PXSize filter_passstart[8], padded_passstart[8], passstart[8];

    PXADAM7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, pxADAM7->Width, pxADAM7->Height, pxADAM7->BitsPerPixel);

    if(pxADAM7->BitsPerPixel >= 8)
    {
        for(PXSize i = 0; i != 7; ++i)
        {
            PXSize bytewidth = pxADAM7->BitsPerPixel / 8u;

            for(PXSize y = 0; y < passh[i]; ++y)
            {
                for(PXSize x = 0; x < passw[i]; ++x)
                {
                    PXSize pixelinstart = passstart[i] + (y * passw[i] + x) * bytewidth;
                    PXSize pixeloutstart = ((PXADAM7_IY[i] + y * PXADAM7_DY[i]) * pxADAM7->Width + PXADAM7_IX[i] + x * PXADAM7_DX[i]) * bytewidth;

                    for(PXSize b = 0; b < bytewidth; ++b)
                    {
                        ((char*)pxADAM7->DataOutput)[pixeloutstart + b] = ((char*)pxADAM7->DataInput)[pixelinstart + b];
                    }
                }
            }
        }
    }
    else /*bpp < 8: Adam7 with pixels < 8 bit is a bit trickier: with bit pointers*/
    {
        for(PXI8U i = 0; i != 7u; ++i)
        {
            const PXSize ilinebits = pxADAM7->BitsPerPixel * passw[i];
            const PXSize olinebits = pxADAM7->BitsPerPixel * pxADAM7->Width;

            for(PXSize y = 0; y < passh[i]; ++y)
            {
                for(PXSize x = 0; x < passw[i]; ++x)
                {
                    // bit pointers (for out and in buffer)
                    PXSize ibp = (8 * passstart[i]) + (y * ilinebits + x * pxADAM7->BitsPerPixel);
                    PXSize obp = (PXADAM7_IY[i] + (PXSize)y * PXADAM7_DY[i]) * olinebits + (PXADAM7_IX[i] + (PXSize)x * PXADAM7_DX[i]) * pxADAM7->BitsPerPixel;

                    for(PXSize b = 0; b < pxADAM7->BitsPerPixel; ++b)
                    {
                        unsigned char bit = readBitFromReversedStream(&ibp, pxADAM7->DataInput);
                        PXADAM7setBitOfReversedStream(&obp, pxADAM7->DataOutput, bit);
                    }
                }
            }
        }
    }
}
