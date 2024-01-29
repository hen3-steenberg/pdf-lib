
#ifndef PDF_LIB_DEFINITION
#define PDF_LIB_DEFINITION 1
#include <cstdint>
#include <ios>
#include <utility>
#include <string>
#include <string_view>
#include <ostream>
#include <initializer_list>
#include <type_traits>
#include <concepts>
#include <iomanip>

namespace pdf_lib{


#pragma region PDF_HEADER

    enum class pdf_version {
        PDF_1_7 //only supported pdf version at the moment.
    };

    template<pdf_version VERSION>
    std::ostream & write_header(std::ostream & pdf) = delete;

    template<>
    inline std::ostream & write_header<pdf_version::PDF_1_7>(std::ostream & pdf)
    {
        //PDF32000_2008 section 7.5.2
        pdf << "%PDF-1.7\n";
        return pdf;
    }

#pragma endregion

#pragma region PDF_OBJECT
    //PDF32000_2008 section 7.3
    struct pdf_object {
        virtual std::ostream & write_object(std::ostream & pdf) const = 0;
    };

    template<typename T>
    concept pdf_object_type = std::is_base_of_v<pdf_object, T>;

    template<pdf_object_type T>
    std::ostream& operator<<(std::ostream & os, T obj)
    {
        return obj.write_object(os);
    }

    template<typename T>
    struct pdf_value : pdf_object
    {
        using type = T;
        T value;

        constexpr pdf_value() noexcept
            : value() {};

        constexpr pdf_value(T _val) noexcept
            : value(_val) {};

        constexpr pdf_value(pdf_value const& other) noexcept
            : value(other.value) {}

        constexpr pdf_value& operator=(pdf_value const& other) noexcept
        {
            value = other;
        }

        constexpr pdf_value& operator=(T other) noexcept
        {
            value = other;
        }

        constexpr operator T() const noexcept
        {
            return value;
        }

        constexpr auto operator<=>(pdf_value other) const noexcept
        {
            return value <=> other.value;
        }

        constexpr auto operator<=>(T other) const noexcept
        {
            return value <=> other;
        }

        #pragma region pdf_bool
        using bool_t = std::enable_if_t<std::is_same_v<T, bool>, pdf_value<T>>;

        constexpr bool_t operator!() const noexcept
        {
            return bool_t{!value};
        }

        constexpr bool_t operator&&(bool_t other) const noexcept
        {
            return bool_t{value && other.value};
        }

        constexpr bool_t& operator&=(bool_t other) noexcept
        {
            value &= other.value;
            return *this;
        }

        constexpr bool_t operator||(bool_t other) const noexcept
        {
            return bool_t{value || other.value};
        }

        constexpr bool_t& operator|=(bool_t other) noexcept
        {
            value |= other.value;
            return *this;
        }

        constexpr bool_t operator^(bool_t other) const noexcept
        {
            return bool_t{value ^ other.value};
        }

        constexpr bool_t& operator^=(bool_t other) noexcept
        {
            value ^= other.value;
            return *this;
        }

        #pragma endregion

        #pragma region pdf_integer
        using int_t = std::enable_if<std::is_integral_v<T> && !std::is_same_v<T, bool>, pdf_value<T>>;

        constexpr int_t operator&(int_t other) const noexcept
        {
            return int_t{value & other.value};
        }

        constexpr int_t& operator&=(int_t other) noexcept
        {
            value &= other.value;
            return *this;
        }

        constexpr int_t operator|(int_t other) const noexcept
        {
            return int_t{value | other.value};
        }

        constexpr int_t& operator|=(int_t other) noexcept
        {
            value |= other.value;
            return *this;
        }

        constexpr int_t operator^(int_t other) const noexcept
        {
            return int_t{value ^ other.value};
        }

        constexpr int_t& operator^=(int_t other) noexcept
        {
            value ^= other.value;
            return *this;
        }

        constexpr int_t& operator++() noexcept
        {
            value++;
            return *this;
        }

        constexpr int_t operator++(int) noexcept
        {
            int_t result{value};
            value++;
            return result;
        }

        constexpr int_t& operator--() noexcept
        {
            value--;
            return *this;
        }

        constexpr int_t operator--(int) noexcept
        {
            int_t result{value};
            value--;
            return result;
        }

        constexpr int_t operator%(int_t other) const noexcept
        {
            return int_t{value % other.value};
        }

        constexpr int_t& operator%=(int_t other) noexcept
        {
            value %= other.value;
            return *this;
        }

        #pragma endregion

        #pragma region pdf_number
        using number_t = std::enable_if<std::is_floating_point_v<T> || (std::is_integral_v<T> && ! std::is_same_v<T, bool>), pdf_value<T>>;

        constexpr number_t operator+(number_t other) const noexcept
        {
            return number_t{value + other.value};
        }

        constexpr number_t& operator+=(number_t other) noexcept
        {
            value += other.value;
            return *this;
        }

        constexpr number_t operator-(number_t other) const noexcept
        {
            return number_t{value - other.value};
        }

        constexpr number_t& operator-=(number_t other) noexcept
        {
            value -= other.value;
            return *this;
        }

        constexpr number_t operator*(number_t other) const noexcept
        {
            return number_t{value * other.value};
        }

        constexpr number_t& operator*=(number_t other) noexcept
        {
            value *= other.value;
            return *this;
        }

        constexpr number_t operator/(number_t other) const noexcept
        {
            return number_t{value / other.value};
        }

        constexpr number_t& operator/=(number_t other) noexcept
        {
            value /= other.value;
            return *this;
        }

        #pragma endregion

        std::ostream& write_object(std::ostream & pdf) const
        {
            if constexpr(std::is_floating_point_v<T>)
            {
                pdf << std::fixed;
                pdf << value;
            }
            else if constexpr(std::is_same_v<bool, T>)
            {
                //PDF32000_2008 section 7.3.2
                pdf << (value? "true" : "false");
            }
            else
            {
                pdf << value;
            }
            
            return pdf;
        }
    };

    template<typename T>
    constexpr auto operator<=>(T other, pdf_value<T> val)
    {
        return other <=> val.value;
    }

    typedef pdf_value<bool> pdf_bool;
    typedef pdf_value<int64_t> pdf_integer;
    typedef pdf_value<double> pdf_real;
    
    struct pdf_string : pdf_object
    {
        std::string value;

        static char get_lower_hex_value(unsigned char c) noexcept
        {
            static const char hex_chars[] = "0123456789ABCDEF";
            return hex_chars[c];
        }

        std::ostream& write_object(std::ostream& pdf) const
        {
            pdf << '<';
            for(char c : value)
            {
                pdf << get_lower_hex_value(c & 0x0F);
                pdf << get_lower_hex_value(c >> 4);
            }
            pdf << '>';
            return pdf;
        }
    };

    template<pdf_object_type T>
    struct pdf_indirect_object : T{
        size_t object_number;
        size_t generation_number;

        template<typename ... Targs>
        pdf_indirect_object(Targs ... args)
            : T(args...) {} 

        std::ostream& write_object(std::ostream& pdf) const& override{
            pdf << object_number << ' ' << generation_number << " obj\n";
            write_object(pdf);
            pdf << "\nendobj\n";
            return pdf;
        }
    };
#pragma endregion

    template<pdf_version VERSION = pdf_version::PDF_1_7>
    struct pdf{

    };

    //TODO : pdf body

    //TODO : pdf xref

    //TODO : pdf trailer

    std::ostream & scan_to_pdf(std::ostream & destination, std::initializer_list<std::string_view> names);

    template<typename ... Images>
    std::ostream &scan_to_pdf(std::ostream & destination, Images ... image_names)
    {
        return scan_to_pdf(destination, {image_names...});
    }
}

#endif