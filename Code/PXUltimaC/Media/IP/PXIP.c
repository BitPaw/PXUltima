#include "PXIP.h"

PXIPVersion PXAPI PXIPAnalyseVersion(const char* ip)
{
    PXIPVersion ipVersion = IPVersionInvalid;

    char containsDot = 0;
    char containsDoubleDot = 0;
    char containsHexadezimal = 0;
    char textChars = 0;
    char containsNumbers = 0;

    for (int index = 0; ip[index] != '\0'; index++)
    {
        char character = ip[index];
        char isNumber = character >= '0' && character <= '9';
        char isBetweenAToF = (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F');
        char isDot = character == '.';
        char isDoubleDot = character == ':';
        char isTextChars = !isNumber && !isBetweenAToF && !isDot && !isDoubleDot;

        if (isTextChars)
            textChars++;

        if (isNumber)
            containsNumbers++;

        if (isBetweenAToF)
            containsHexadezimal++;

        if (isDot)
            containsDot++;

        if (isDoubleDot)
            containsDoubleDot++;
    }

    if (containsNumbers && containsDot && !containsDoubleDot && !containsHexadezimal && !textChars)
    {
        return IPVersion4;
    }

    if (containsNumbers && containsDoubleDot && containsHexadezimal && !textChars && !containsDot)
    {
        return IPVersion6;
    }

    if (!containsDoubleDot)
    {
        return IPVersionUnknownDomain;
    }


    return IPVersionInvalid;
}

PXByte PXAPI PXIPIsValid(const char* ipAdress)
{
    PXIPVersion ipVersion = PXIPAnalyseVersion(ipAdress);

    switch (ipVersion)
    {
    case IPVersion4:
    {
        return PXIPv4CheckIfValid(ipAdress) == 0;
    }
    case IPVersionUnknownDomain:
    {
        return 1;
    }
    case IPVersion6:
    {
        return PXIPv6CheckIfValid(ipAdress) == 0;
    }

    default:
    case IPVersionInvalid:
    {
        return 0;
    }
    }
}

PXByte PXAPI PXIPv4CheckIfValid(const char* ipAdress)
{
    const unsigned char resultIPv4OK = 0;
    const unsigned char resultIPv4NullPointer = 1;
    const unsigned char resultIPv4InvalidCharacter = 2;
    const unsigned char resultIPv4OctetTooLarge = 3;
    const unsigned char resultIPv4InvalidLength = 4;
    const unsigned char resultIPv4InvalidOctetAmount = 5;

    const unsigned char expectedDots = 3;
    const unsigned char minimalSize = 7; // 0.0.0.0
    const unsigned char maximalSize = 15; //255.255.255.255

    unsigned int index = 0;
    unsigned int length = 0;
    unsigned int countedDots = 0;
    unsigned char hasValidLength = 0;
    unsigned char hasEnoghDots = 0;

    unsigned short octetValue = 0;

    if (ipAdress == 0)
    {
        return resultIPv4NullPointer;
    }

    for (; ipAdress[index] != '\0' && index < maximalSize; index++)
    {
        char character = ipAdress[index];
        char isDot = character == '.';
        char isValidCharacter = (character >= '0' && character <= '9') || character;

        if (!isValidCharacter)
        {
            return resultIPv4InvalidCharacter;
        }

        if (isDot)
        {
            countedDots++;
            octetValue = 0;
        }
        else
        {
            octetValue *= 10; // "Shft number to left" Example 12 -> 120
            octetValue += (character - '0'); // ASCII character to actual number.

            if (octetValue > 255) // Is in 8-Bit range? Can be 0-255 only
            {
                return resultIPv4OctetTooLarge;
            }
        }

        length++;
    }

    hasValidLength = length >= minimalSize && length <= maximalSize;
    hasEnoghDots = countedDots == expectedDots;

    if (!hasValidLength)
    {
        return resultIPv4InvalidLength;
    }

    if (!hasEnoghDots)
    {
        return resultIPv4InvalidOctetAmount;
    }

    return resultIPv4OK;
}

unsigned char PXAPI PXIPv6CheckIfValid(const char* ipAdress)
{
    const char validIPv6Adress = 0;
    const char invalidAmountOfSeperators = 1;

    const char seperator = ':';
    const int expectedAmountOfSeperators = 7;
    unsigned char seperatorCounter = 0;
    unsigned int lengh = 0;
    unsigned int maxLength = 32;
    unsigned int minLength = 2;


    for (; ipAdress[lengh] != '\0' && lengh <= maxLength; lengh++)
    {
        char character = ipAdress[lengh];
        char isSeperator = character == seperator;

        if (isSeperator)
        {
            if (++seperatorCounter > 7)
            {
                return invalidAmountOfSeperators;
            }
        }
    }

    return validIPv6Adress;
}
