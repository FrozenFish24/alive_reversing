#include "stdafx.h"
#include "Compression.hpp"
#include "Function.hpp"
#include "PtrStream.hpp"

void Compression_ForceLink() { }

static bool Expand3To4Bytes(int& remainingCount, PtrStream& stream, BYTE* ret, DWORD& dstPos)
{
    if (!remainingCount)
    {
        return false;
    }

    const DWORD src3Bytes = stream.ReadU8() | (stream.ReadU16() << 8);
    remainingCount--;

    DWORD value;

    // decompress each byte of the input value from least to most significant
    value =  (BYTE)src3Bytes & 0x3F;
    value |= ((DWORD)src3Bytes << 2) & 0x3F00;
    value |= (src3Bytes << 4) & 0x3F0000;
    value |= ((src3Bytes << 4) & 0x0FC00000) << 2;

    reinterpret_cast<DWORD*>(ret)[dstPos++] = value;

    return true;
}

EXPORT void CC CompressionType2_Decompress_40AA50(const BYTE* pSrc, BYTE* pDst, DWORD dataSize)
{
    PtrStream stream(&pSrc);

    int dwords_left = dataSize / 4;
    int remainder = dataSize % 4;

    DWORD dstPos = 0;
    while (dwords_left)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!Expand3To4Bytes(dwords_left, stream, pDst, dstPos))
            {
                break;
            }
        }
    }

    // TODO: Branch not tested - copies remainder bytes directly into output
    while (remainder > 0)
    {
        remainder--;
        pDst[dstPos++] = stream.ReadU8();
    }
}

template<typename T>
static void ReadNextSource(PtrStream& stream, int& control_byte, T& dstIndex)
{
    if (control_byte)
    {
        if (control_byte == 0xE) // Or 14
        {
            control_byte = 0x1Eu; // Or 30
            dstIndex |= stream.ReadU16() << 14;
        }
    }
    else
    {
        dstIndex = stream.ReadU32();
        control_byte = 0x20u; // 32
    }
    control_byte -= 6;
}

EXPORT void CC CompressionType_3Ae_Decompress_40A6A0(const BYTE* pData, BYTE* decompressedData)
{
    PtrStream stream(&pData);

    int dstPos = 0;
    int control_byte = 0;

    int width = stream.ReadU16();
    int height = stream.ReadU16();

    if (height > 0)
    {
        unsigned int dstIndex = 0;
        do
        {
            int columnNumber = 0;
            while (columnNumber < width)
            {
                ReadNextSource(stream, control_byte, dstIndex);

                const unsigned char blackBytes = dstIndex & 0x3F;
                unsigned int srcByte = dstIndex >> 6;

                const int bytesToWrite = blackBytes + columnNumber;

                for (int i = 0; i < blackBytes; i++)
                {
                    decompressedData[dstPos++] = 0;
                }

                ReadNextSource(stream, control_byte, srcByte);

                const unsigned char bytes = srcByte & 0x3F;
                dstIndex = srcByte >> 6;

                columnNumber = bytes + bytesToWrite;
                if (bytes > 0)
                {
                    int byteCount = bytes;
                    do
                    {
                        ReadNextSource(stream, control_byte, dstIndex);

                        const char dstByte = dstIndex & 0x3F;
                        dstIndex = dstIndex >> 6;

                        decompressedData[dstPos] = dstByte;
                        dstPos++;
                        --byteCount;
                    } while (byteCount);
                }
            }

            while (columnNumber & 3)
            {
                ++dstPos;
                ++columnNumber;
            }
        } while (height-- != 1);
    }

}

// 0xxx xxxx = string of literals (1 to 128)
// 1xxx xxyy yyyy yyyy = copy from y bytes back, x bytes
EXPORT void CC CompressionType_4Or5_Decompress_4ABAB0(const BYTE* pData, BYTE* decompressedData)
{
    PtrStream stream(&pData);

    // Get the length of the destination buffer
    DWORD nDestinationLength = 0;
    stream.Read(nDestinationLength);

    DWORD dstPos = 0;
    while (dstPos < nDestinationLength)
    {
        // get code byte
        const BYTE c = stream.ReadU8();

        // 0x80 = 0b10000000 = RLE flag
        // 0xc7 = 0b01111100 = bytes to use for length
        // 0x03 = 0b00000011
        if (c & 0x80)
        {
            // Figure out how many bytes to copy.
            const DWORD nCopyLength = ((c & 0x7C) >> 2) + 3;

            // The last 2 bits plus the next byte gives us the destination of the copy
            const BYTE c1 = stream.ReadU8();
            const DWORD nPosition = ((c & 0x03) << 8) + c1 + 1;
            const DWORD startIndex = dstPos - nPosition;

            for (DWORD i = 0; i < nCopyLength; i++)
            {
                decompressedData[dstPos++] = decompressedData[startIndex + i];
            }
        }
        else
        {
            // Here the value is the number of literals to copy
            for (int i = 0; i < c + 1; i++)
            {
                decompressedData[dstPos++] = stream.ReadU8();
            }
        }
    }
}

static BYTE NextNibble(PtrStream& stream, bool& readLo, BYTE& srcByte)
{
    if (readLo)
    {
        readLo = !readLo;
        return srcByte >> 4;
    }
    else
    {
        srcByte = stream.ReadU8();
        readLo = !readLo;
        return srcByte & 0xF;
    }
}

EXPORT void CC CompressionType6Ae_Decompress_40A8A0(const BYTE* pSrc, BYTE* pDst)
{
    PtrStream stream(&pSrc);

    bool bNibbleToRead = false;
    bool bSkip = false;
    DWORD dstPos = 0;

    const int w = stream.ReadU16();
    const int h = stream.ReadU16();

    if (h > 0)
    {
        BYTE srcByte = 0;
        int heightCounter = h;
        do
        {
            int widthCounter = 0;
            while (widthCounter < w)
            {
                BYTE nibble = NextNibble(stream, bNibbleToRead, srcByte);
                widthCounter += nibble;

                while (nibble)
                {
                    if (bSkip)
                    {
                        dstPos++;
                    }
                    else
                    {
                        pDst[dstPos] = 0;
                    }
                    bSkip = !bSkip;
                    --nibble;
                }

                nibble = NextNibble(stream, bNibbleToRead, srcByte);
                widthCounter += nibble;

                if (nibble > 0)
                {
                    do
                    {
                        const BYTE data = NextNibble(stream, bNibbleToRead, srcByte);
                        if (bSkip)
                        {
                            pDst[dstPos++] |= 16 * data;
                            bSkip = 0;
                        }
                        else
                        {
                            pDst[dstPos] = data;
                            bSkip = 1;
                        }
                    } while (--nibble);
                }
            }

            for (; widthCounter & 7; ++widthCounter)
            {
                if (bSkip)
                {
                    dstPos++;
                }
                else
                {
                    pDst[dstPos] = 0;
                }
                bSkip = !bSkip;
            }

        } while (heightCounter-- != 1);
    }
}
