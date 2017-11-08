// Executable size report utility.
// Aras Pranckevicius, http://aras-p.info/projSizer.html
// Based on code by Fabian "ryg" Giesen, http://farbrausch.com/~fg/
// Public domain.

#ifndef __DEBUGINFO_HPP__
#define __DEBUGINFO_HPP__

#include "types.hpp"
#include <map>

using std::string;

/****************************************************************************/

#define DIC_END     0
#define DIC_CODE    1
#define DIC_DATA    2
#define DIC_BSS     3 // uninitialized data
#define DIC_UNKNOWN 4

struct DISymFile // File
{
    sInt  fileName;
    sU32  codeSize;
    sU32  dataSize;
};

struct DISymNameSp // Namespace
{
    sInt  name;
    sU32  codeSize;
    sU32  dataSize;
};

struct DISymbol
{
    sInt name;
    sInt mangledName;
    sInt NameSpNum;
    sInt objFileNum;
    sU32 VA;
    sU32 Size;
    sInt Class;
};

struct TemplateSymbol
{
    string  name;
    sU32  size;
    sU32  count;
};

class DebugInfo
{
    typedef std::vector<string>   StringByIndexVector;
    typedef std::map<string, sInt> IndexByStringMap;

    StringByIndexVector m_StringByIndex;
    IndexByStringMap  m_IndexByString;
    sU32 BaseAddress;

    sU32 CountSizeInClass(sInt type) const;

public:
    sArray<DISymbol>  Symbols;
    sArray<TemplateSymbol>  Templates;
    sArray<DISymFile> m_Files;
    sArray<DISymNameSp> NameSps;

    void Init();
    void Exit();

    // only use those before reading is finished!!
    sInt MakeString(sChar *s);
    const char* GetStringPrep(sInt index) const { return m_StringByIndex[index].c_str(); }
    void SetBaseAddress(sU32 base)            { BaseAddress = base; }

    void FinishedReading();

    sInt GetFile(sInt fileName);
    sInt GetFileByName(sChar *objName);

    sInt GetNameSpace(sInt name);
    sInt GetNameSpaceByName(sChar *name);

    void StartAnalyze();
    void FinishAnalyze();
    sBool FindSymbol(sU32 VA, DISymbol **sym);

    std::string WriteReport();
};

class DebugInfoReader
{
public:
    virtual sBool ReadDebugInfo(sChar *fileName, DebugInfo &to) = 0;
};


#endif
