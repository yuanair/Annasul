module;
#include <iostream>
#include <utility>

export module main;

template <typename Ty>
concept integral = std::is_integral_v<std::remove_cvref_t<Ty>>;

export template <integral Ty>
class integer {
    Ty value;

   public:
   integer() = delete; 
    explicit integer(auto value) : value(value) {
        static_assert(std::is_same_v<Ty, decltype(value)>, "Implicit Type Conversion");
    }
};

export using i8 = integer<std::int8_t>;
export using i16 = integer<std::int16_t>;
export using i32 = integer<std::int32_t>;
export using i64 = integer<std::int64_t>;
export using u8 = integer<std::uint8_t>;
export using u16 = integer<std::uint16_t>;
export using u32 = integer<std::uint32_t>;
export using u64 = integer<std::uint64_t>;

export int main() {
    i32 i{42i16};
    return 0;
}
