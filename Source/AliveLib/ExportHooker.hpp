#pragma once

#include "detours.h"
#include <assert.h>
#include <vector>
#include <set>
#include <sstream>
#include "logger.hpp"
#include "Function.hpp"

class ExportHooker
{
public:
    explicit ExportHooker(HINSTANCE instance) : mhInstance(instance)
    {
        mExports.reserve(5000);
    }

    void Apply(bool saveImplementedFuncs = false)
    {
        CheckVars(); // Check for dup vars or vars that overlap in address space

        if (!DetourEnumerateExports(mhInstance, this, EnumExports))
        {
            ALIVE_FATAL("Export enumeration failed");
        }

        if (saveImplementedFuncs)
        {
            std::ofstream implementedStream("decompiled_functions.txt");
            std::ofstream stubbedStream("stubbed_functions.txt");

            for (const auto& e : mExports)
            {
                if (e.mIsImplemented)
                {
                    implementedStream << e.mGameFunctionAddr << "\n";
                }
                else
                {
                    stubbedStream << e.mGameFunctionAddr << "\n";
                }
            }

        }

        if (IsAlive())
        {
            LoadDisabledHooks();
            ProcessExports();
        }
    }

private:
    void LoadDisabledHooks()
    {
        std::ifstream inFile("hook_map.txt");
        if (inFile)
        {
            std::string line;
            while (std::getline(inFile, line))
            {
                if (!line.empty())
                {
                    unsigned long addr = std::stoul(line, nullptr, 16);
                    mDisabledImpls.insert(addr);
                }
            }
        }
    }

    void ProcessExports()
    {
        TRACE_ENTRYEXIT;
        LONG err = DetourTransactionBegin();

        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourTransactionBegin failed");
        }

        err = DetourUpdateThread(GetCurrentThread());

        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourUpdateThread failed");
        }

        for (const auto& e : mExports)
        {
            const bool hookDisabledByConfig = mDisabledImpls.find(e.mGameFunctionAddr) != std::end(mDisabledImpls);
            /* So are so many hooks now that printing so much data to stdout out causes noticeable delay
            LOG_INFO("Hook: "
                << e.mName.c_str()
                << " From "
                << "0x" << std::hex << (e.mIsImplemented ? e.mGameFunctionAddr : (DWORD)e.mCode)
                << " To "
                << "0x" << std::hex << (e.mIsImplemented ? (DWORD)e.mCode : e.mGameFunctionAddr)
                << " Implemented: " << e.mIsImplemented
                << hookDisabledByConfig ? "(Override to OFF by config)" : "");
            */

            if (e.mIsImplemented && !hookDisabledByConfig)
            {
                // Redirect real game function to our impl
                err = DetourAttach(&(PVOID&)e.mGameFunctionAddr, e.mCode);
            }
            else
            {
                // Redirect our impl to real game function
                err = DetourAttach(&(PVOID&)e.mCode, (PVOID)e.mGameFunctionAddr);
            }

            if (err != NO_ERROR)
            {
                ALIVE_FATAL("DetourAttach failed");
            }
        }

        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourTransactionCommit failed");
        }
    }

    static bool IsHexDigit(char letter)
    {
        if (letter >= '0' && letter <= '9')
        {
            return true;
        }
        const char lower = ::tolower(letter);
        return (lower >= 'a' && lower <= 'f');
    }

    struct ExportInformation
    {
        bool mIsImplemented;
        std::string mExportedFunctionName;
        std::string mUnMangledFunctioName;

        const std::string& Name()
        {
            if (!mUnMangledFunctioName.empty())
            {
                return mUnMangledFunctioName;
            }
            return mExportedFunctionName;
        }
    };

    static ExportInformation GetExportInformation(PVOID pExportedFunctionAddress, const std::string& exportedFunctionName)
    {
        ExportInformation info = {};
        info.mIsImplemented = false;
        info.mExportedFunctionName = exportedFunctionName;
        
        // 4 nops, int 3, 4 nops
        const static BYTE kPatternToFind[] = { 0x90, 0x90, 0x90, 0x90, 0xCC, 0x90, 0x90, 0x90, 0x90 };
        BYTE codeBuffer[256] = {};
        memcpy(codeBuffer, pExportedFunctionAddress, sizeof(codeBuffer));
        for (int i = 0; i < sizeof(codeBuffer) - sizeof(kPatternToFind); i++)
        {
            if (codeBuffer[i] == kPatternToFind[0])
            {
                if (memcmp(&codeBuffer[i], kPatternToFind, sizeof(kPatternToFind)) == 0)
                {
                    // Go back 5 bytes and check the offset to the string at that location matches the real game function address
                    // that we got from the export name. The asm patter before kPatternToFind should be:
                    // push eax
                    // mov eax, offset to function name string
                    // pop eax
                    // Therefore extracting the pointer to unmangled offset to the function name tells us if we have the right function.
                    const char*** strAddr = reinterpret_cast<const char***>(&reinterpret_cast<BYTE*>(pExportedFunctionAddress)[i - 5]);

                    const char* pBothNames = **strAddr;
                    size_t len = strlen(pBothNames);
                    const char* mangledName = pBothNames + len + 1;

                    if (std::string(mangledName) == exportedFunctionName)
                    {
                        info.mUnMangledFunctioName = pBothNames;

                        if (!IsAlive())
                        {
                            BYTE* ptr = &reinterpret_cast<BYTE*>(pExportedFunctionAddress)[i + 4];
                            DWORD old = 0;
                            if (!::VirtualProtect(ptr, 1, PAGE_EXECUTE_READWRITE, &old))
                            {
                                ALIVE_FATAL("Failed to make memory writable");
                            }
                            *ptr = 0x90;
                            if (!::VirtualProtect(ptr, 1, old, &old))
                            {
                                ALIVE_FATAL("Failed to restore old memory protection");
                            }
                        }
                        return info;
                    }
                    else
                    {
                        LOG_INFO("didn't match address");
                    }
                   
                }
            }
        }
        info.mIsImplemented = true; // Didn't find not impl instruction pattern
        return info;
    }

    void OnExport(PCHAR pszName, PVOID pCode)
    {
        std::string exportedFunctionName(pszName);
        auto underScorePos = exportedFunctionName.find_first_of('_');
        while (underScorePos != std::string::npos)
        {
            int hexNumLen = 0;
            for (size_t i = underScorePos + 1; i < exportedFunctionName.length(); i++)
            {
                if (IsHexDigit(exportedFunctionName[i]))
                {
                    hexNumLen++;
                }
                else
                {
                    break;
                }
            }

            if (hexNumLen >= 6 && hexNumLen <= 8)
            {
                if (exportedFunctionName.find("?__done__@") == 0 || exportedFunctionName.find("?__kAddr__@") == 0)
                {
                    // The likes of ?__done__@?1??vUpdate_4E0030@Movie@@QAEXXZ@4_NA have been seen - even though the static bool done = false; isn't exported.
                    // The code below treats this var as a function causing false positive of duplicated addresses. Therefore this hack filters out global static
                    // done booleans that have been exported for unknown reasons.
                    // This can also have for kAddr.
                    LOG_WARNING("Ignoring done static boolean which has for some reason been exported " << exportedFunctionName);
                    return;
                }

                std::string addrStr = exportedFunctionName.substr(underScorePos + 1, hexNumLen);
                unsigned long addr = std::stoul(addrStr, nullptr, 16);

                auto it = mUsedAddrs.find(addr);
                if (it != std::end(mUsedAddrs))
                {
                    std::stringstream s;
                    s << "Duplicated function for address " << std::hex << "0x" << addr << " already used by " << it->second.Name() << " when checking " << exportedFunctionName;
                    ALIVE_FATAL(s.str().c_str());
                }


                ExportInformation exportInfo = GetExportInformation(pCode, exportedFunctionName);
                mExports.push_back({ exportedFunctionName, pCode, addr, exportInfo.mIsImplemented });
                mUsedAddrs.insert({ addr, exportInfo });
                return;
            }

            underScorePos = exportedFunctionName.find('_', underScorePos + 1);
        }
        LOG_WARNING(pszName << " was not hooked");
    }

    static BOOL CALLBACK EnumExports(PVOID pContext,
        ULONG /*nOrdinal*/,
        PCHAR pszName,
        PVOID pCode)
    {
        if (pszName && pCode)
        {
            // Resolve 1 level long jumps, not using DetourCodeFromPointer
            // as it appears to have a bug where it checks for 0xeb before 0xe9 and so
            // won't skip jmps that start with long jmps.
            BYTE* pbCode = (BYTE*)pCode;
            if (pbCode[0] == 0xe9)
            {
                // jmp +imm32
                PBYTE pbNew = pbCode + 5 + *(DWORD *)&pbCode[1];
                pCode = pbNew;
            }
            reinterpret_cast<ExportHooker*>(pContext)->OnExport(pszName, pCode);
        }
        return TRUE;
    }

    HINSTANCE mhInstance = nullptr;
    struct Export
    {
        std::string mName;
        LPVOID mCode;
        DWORD mGameFunctionAddr;
        bool mIsImplemented;
    };
    std::vector<Export> mExports;
    std::map<DWORD, ExportInformation> mUsedAddrs;
    std::set<DWORD> mDisabledImpls;
};
