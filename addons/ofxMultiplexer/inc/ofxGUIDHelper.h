#ifndef _OFX_GUID_HELPER_
#define _OFX_GUID_HELPER_

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
//#include <conio.h> //FIXME: is conio.h needed? is it windows-specific?

#ifdef TARGET_WIN32
#include <windows.h> //FIXME: should we #include "Rpc.h" instead?
#else
//Define the GUID type ourselves
typedef struct _GUID {          // size is 16
    unsigned int    Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char   Data4[8];
} GUID;

//...and the == operator
bool operator==(const GUID& lhs, const GUID& rhs) {
    if (lhs.Data1 != rhs.Data1) return false;
    if (lhs.Data2 != rhs.Data2) return false;
    if (lhs.Data3 != rhs.Data3) return false;
    for (int i=0; i<8; i++)
        if (lhs.Data4[i] != rhs.Data4[i]) return false;
    return true;
}
#endif

//Convert an hex string to a number
template<class T> T HexToInt(const std::string &str);
//Convert a number to an hex string
template<class T> std::string ToHex(const T &value);
//Convert a GUID to string
std::string GUIDToString(const GUID &guid);
//Convert a string to GUID
GUID StringToGUID(const std::string &str);
int StringToInt(const std::string &str);
std::string IntToString(const int &value);

#endif//_OFX_GUID_HELPER_
