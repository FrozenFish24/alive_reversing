#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

class BellSong : public BaseGameObject
{
public:
    EXPORT BellSong* ctor_4760B0(__int16 type, unsigned int code);

    virtual void VUpdate() override;

    EXPORT void VUpdate_476130();

    virtual BaseGameObject* VDestructor(signed int flags) override;


    int field_10_code_idx;
    __int16 field_14_bDone;
    __int16 field_16_type;
    int field_18_code;
    __int16 field_1C_code_len;
    __int16 field_1E;
};
ALIVE_ASSERT_SIZEOF(BellSong, 0x20);

END_NS_AO

