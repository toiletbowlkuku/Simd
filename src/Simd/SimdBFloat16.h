/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2022 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __SimdBFloat16_h__
#define __SimdBFloat16_h__

#include "Simd/SimdInit.h"
#include "Simd/SimdInit.h"

namespace Simd
{
    namespace Base
    {
        namespace Bf16
        {
            union Bits
            {
                float f32;
                uint32_t u32;

                SIMD_INLINE Bits(float val) : f32(val) { }
                SIMD_INLINE Bits(uint32_t val) : u32(val) { }
            };

            const int SHIFT = 16;
            const uint32_t ROUND = 0x00008000;
            const uint32_t MASK = 0xFFFF0000;
        }

        SIMD_INLINE float RoundToBFloat16(float value)
        {
            return Bf16::Bits((Bf16::Bits(value).u32 + Bf16::ROUND) & Bf16::MASK).f32;
        }

        SIMD_INLINE uint16_t Float32ToBFloat16(float value)
        {
            return uint16_t((Bf16::Bits(value).u32 + Bf16::ROUND) >> Bf16::SHIFT);
        }

        SIMD_INLINE float BFloat16ToFloat32(uint16_t value)
        {
            return Bf16::Bits(uint32_t(value) << Bf16::SHIFT).f32;
        }
    }

#ifdef SIMD_SSE41_ENABLE    
    namespace Sse41
    {
        namespace Bf16
        {
            const __m128i ROUND = SIMD_MM_SET1_EPI32(Base::Bf16::ROUND);
            const __m128i MASK = SIMD_MM_SET1_EPI32(Base::Bf16::MASK);
        }

        SIMD_INLINE __m128i Float32ToBFloat16(__m128 value)
        {
            return _mm_srli_epi32(_mm_add_epi32(_mm_castps_si128(value), Bf16::ROUND), Base::Bf16::SHIFT);
        }
    }
#endif   

#ifdef SIMD_AVX2_ENABLE    
    namespace Avx2
    {
        namespace Bf16
        {
            const __m256i ROUND = SIMD_MM256_SET1_EPI32(Base::Bf16::ROUND);
            const __m256i MASK = SIMD_MM256_SET1_EPI32(Base::Bf16::MASK);
        }

        SIMD_INLINE __m256i Float32ToBFloat16(__m256 value)
        {
            return _mm256_srli_epi32(_mm256_add_epi32(_mm256_castps_si256(value), Bf16::ROUND), Base::Bf16::SHIFT);
        }
    }
#endif  

#ifdef SIMD_AVX512BW_ENABLE    
    namespace Avx512bw
    {
        namespace Bf16
        {
            const __m512i ROUND = SIMD_MM512_SET1_EPI32(Base::Bf16::ROUND);
            const __m512i MASK = SIMD_MM512_SET1_EPI32(Base::Bf16::MASK);
        }

        SIMD_INLINE __m512i Float32ToBFloat16(__m512 value)
        {
            return _mm512_srli_epi32(_mm512_add_epi32(_mm512_castps_si512(value), Bf16::ROUND), Base::Bf16::SHIFT);
        }
    }
#endif 
}

#endif//__SimdBFloat16_h__
