#pragma once

#include "BaseGameObject.hpp"

enum class CameraSwapEffects : __int16;

class ScreenClipper;

class CameraSwapper : public BaseGameObject
{
public:
    EXPORT void ctor_4E5000(BYTE** ppCamRes, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos);
    EXPORT void dtor_4E5790();
    EXPORT void vdtor_4E4D90(signed int flags);
    EXPORT void Init_4E50C0(BYTE** ppCamRes, CameraSwapEffects changeEffect);
    EXPORT void vUpdate_4E5850();

    virtual void VUpdate() override;
    virtual void VDestructor(signed int flags) override;
    
private:
    int field_20_movie;
    int field_24_movie_id;
    int field_28_movie;
    int field_2C_movie_id;
    BYTE** field_30_ppCamRes;
    ScreenClipper* field_34_pSubObject;
    CameraSwapEffects field_38_changeEffect;
    __int16 field_3A_current_slice;
    __int16 field_3C_slices_per_tick;
    __int16 field_3E_total_slices;
    __int16 field_40_movie;
    __int16 field_42_movie;
    __int16 field_44_movie_vol;
    __int16 field_46_movie;
    __int16 field_48_movie;
    __int16 field_4A_movie_vol;
    __int16 field_4C_movie_next;
    __int16 field_4E_xpos_converted;
    __int16 field_50_ypos_converted;
    __int16 field_52_XSlices;
    __int16 field_54_YSlices;
    __int16 field_56_slice_width;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x58);

ALIVE_VAR_EXTERN(int, sMovie_ref_count_BB4AE4);