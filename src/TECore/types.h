// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: TECore\types.h $ 
   $Created: 21-11-2016 16h29m54s $ 
   $Modified: 20-02-2017 17h34m39s $
   $Revision: $ 
   $Creator : TOFFANIN Marc $
   $Notice: Licensed under GNU GPL $
   ======================================================================== */ 
#ifndef TYPES_H
#define TYPES_H 

#include <cstdint> // platform independent literals
#include <xmmintrin.h> // SIMD (intrinsics)

// Differentiate static keyword
#define instance_shared    static
#define internal           static
#define global_variable    static
#define local_persist      static
#define compile_time_const constexpr
#define may_compile_const  constexpr

// Integer
using u8 = std::uint8_t;     // Unsigned 8 bits value
using s8 = std::int8_t;      // Signed 8 bits value
using u16 = std::uint16_t;   // Unsigned 16 bits value
using s16 = std::int16_t;    // Signed 16 bits value
using u32 = std::uint32_t;   // Unsigned 32 bits value
using s32 = std::int32_t;    // Signed 32 bits value
using u64 = std::uint64_t;   // Unsigned 64 bits value
using s64 = std::int64_t;    // Signed 64 bits value

// Floating point
using f32 = float;           // 32 bits floating point value
using f64 = double;          // 64 bits floating point value

// SIMD Vector definition
#ifdef _MSC_VER
using VF32 = __m128;
#endif 
#ifdef GCC
using VF32 = vector float;
#endif

#define SHUFFLE_PARAM(x, y, z, w) \ ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))
#define _mm_replicate_x_ps(v) \ _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))
#define _mm_replicate_y_ps(v) \ _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))
#define _mm_replicate_z_ps(v) \ _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))
#define _mm_replicate_w_ps(v) \ _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))
#define _mm_madd_ps(a, b, c) \ _mm_add_ps(_mm_mul_ps((a), (b)), (c))


template <typename bit_type, typename mask_type = u32 >
class flags
{
    public:
        flags()
            : m_mask(0)
        {
        }

        flags(bit_type bit)
            : m_mask(static_cast<mask_type>(bit))
        {
        }

        flags(flags<bit_type> const& rhs)
            : m_mask(rhs.m_mask)
        {
        }


        flags<bit_type>& operator=(flags<bit_type>& rhs)
        {
            m_mask = rhs.m_mask;
            return *this;
        }


        flags<bit_type> & operator=(flags<bit_type> const& rhs)
        {
            m_mask = rhs.m_mask;
            return *this;
        }

        flags<bit_type> & operator|=(flags<bit_type> const& rhs)
        {
            m_mask |= rhs.m_mask;
            return *this;
        }

        flags<bit_type> & operator&=(flags<bit_type> const& rhs)
        {
            m_mask &= rhs.m_mask;
            return *this;
        }

        flags<bit_type> & operator^=(flags<bit_type> const& rhs)
        {
            m_mask ^= rhs.m_mask;
            return *this;
        }

        flags<bit_type> operator|(flags<bit_type> const& rhs) const
        {
            flags<bit_type> result(*this);
            result |= rhs;
            return result;
        }

        flags<bit_type> operator&(flags<bit_type> const& rhs) const
        {
            flags<bit_type> result(*this);
            result &= rhs;
            return result;
        }

        flags<bit_type> operator^(flags<bit_type> const& rhs) const
        {
            flags<bit_type> result(*this);
            result ^= rhs;
            return result;
        }

        bool operator!() const
        {
            return !m_mask;
        }

        flags<bit_type> operator~() const
        {
            // @Incomplete @Cleanup
            // Is this really needed ? FlagTraits is not defined yet, do we need to define
            // all traits ?

            /*
            flags<bit_type> result(*this);
            result.m_mask ^= FlagTraits<bit_type>::allflags;
            return result;
            */
        }

        bool operator==(flags<bit_type> const& rhs) const
        {
            return m_mask == rhs.m_mask;
        }

        bool operator!=(flags<bit_type> const& rhs) const
        {
            return m_mask != rhs.m_mask;
        }

        explicit operator bool() const
        {
            return !!m_mask;
        }

        explicit operator mask_type() const
        {
            return m_mask;
        }

    private:
        mask_type  m_mask;
};

    template <typename bit_type>
flags<bit_type> operator|(bit_type bit, flags<bit_type> const& flags)
{
    return flags | bit;
}

    template <typename bit_type>
flags<bit_type> operator&(bit_type bit, flags<bit_type> const& flags)
{
    return flags & bit;
}

    template <typename bit_type>
flags<bit_type> operator^(bit_type bit, flags<bit_type> const& flags)
{
    return flags ^ bit;
}



#endif /* TYPES_H */
