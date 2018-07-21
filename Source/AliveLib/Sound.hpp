#pragma once

#include "FunctionFwd.hpp"
#include <mmeapi.h>
#include <dsound.h>

EXPORT void CC SND_4CB480();
EXPORT void CC SND_SsQuit_4EFD50();
EXPORT void CC SND_Clear_4CB4B0();
EXPORT signed int CC SND_CreateDS_4EEAA0(unsigned int sampleRate, int bitsPerSample, int isStereo);


struct SoundEntry
{
    int field_0_tableIdx;
    LPDIRECTSOUNDBUFFER field_4_pDSoundBuffer;
    BYTE* field_8_pSoundBuffer;
    int field_C_buffer_size_bytes;
    int field_10;
    int field_14_buffer_size_bytes;
    int field_18_sampleRate;
    char field_1C_bitsPerSample;
    unsigned __int8 field_1D_blockAlign;
    char field_1E;
    char field_1F;
    int field_20_isStereo;
};
ALIVE_ASSERT_SIZEOF(SoundEntry, 0x24);


