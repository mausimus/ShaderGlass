/*
ShaderGlass: Safe numeric parsing utilities
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <string>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <cmath>

// Safe integer parsing with overflow protection
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
SafeParseInt(const std::string& str, T defaultValue = 0)
{
    static_assert(std::is_integral<T>::value, "SafeParseInt requires an integral type");

    if(str.empty())
        return defaultValue;

    try
    {
        // Remove leading/trailing whitespace
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        if(start == std::string::npos)
            return defaultValue;

        std::string trimmed = str.substr(start, end - start + 1);

        // Parse as long long to detect overflow
        size_t pos = 0;
        long long value = std::stoll(trimmed, &pos);

        // Check if entire string was consumed
        if(pos != trimmed.length())
            return defaultValue;

        // Check bounds for target type
        if(value < std::numeric_limits<T>::min() || value > std::numeric_limits<T>::max())
        {
            throw std::out_of_range("Value out of range for target type");
        }

        return static_cast<T>(value);
    }
    catch(const std::invalid_argument&)
    {
        return defaultValue;
    }
    catch(const std::out_of_range&)
    {
        // Overflow - return max or min depending on sign
        if(!str.empty() && str[0] == '-')
            return std::numeric_limits<T>::min();
        else
            return std::numeric_limits<T>::max();
    }
}

// Safe floating point parsing with overflow protection
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
SafeParseFloat(const std::string& str, T defaultValue = 0.0)
{
    static_assert(std::is_floating_point<T>::value, "SafeParseFloat requires a floating point type");

    if(str.empty())
        return defaultValue;

    try
    {
        // Remove leading/trailing whitespace
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        if(start == std::string::npos)
            return defaultValue;

        std::string trimmed = str.substr(start, end - start + 1);

        // Parse as long double to handle any floating point type
        size_t pos = 0;
        long double value = std::stold(trimmed, &pos);

        // Check if entire string was consumed
        if(pos != trimmed.length())
            return defaultValue;

        // Check for infinity and NaN
        if(std::isinf(value) || std::isnan(value))
        {
            return defaultValue;
        }

        // Check bounds for target type (if not double/long double)
        if(sizeof(T) < sizeof(long double))
        {
            if(value < -std::numeric_limits<T>::max() || value > std::numeric_limits<T>::max())
            {
                return (value < 0) ? -std::numeric_limits<T>::max() : std::numeric_limits<T>::max();
            }
        }

        return static_cast<T>(value);
    }
    catch(const std::invalid_argument&)
    {
        return defaultValue;
    }
    catch(const std::out_of_range&)
    {
        // Overflow - return max or min depending on sign
        if(!str.empty() && str[0] == '-')
            return -std::numeric_limits<T>::max();
        else
            return std::numeric_limits<T>::max();
    }
}

// Compatibility aliases for common use cases
inline int SafeAtoi(const char* str, int defaultValue = 0)
{
    return SafeParseInt<int>(str ? str : "", defaultValue);
}

inline float SafeAtof(const char* str, float defaultValue = 0.0f)
{
    return SafeParseFloat<float>(str ? str : "", defaultValue);
}

inline double SafeAtof(const std::string& str, double defaultValue = 0.0)
{
    return SafeParseFloat<double>(str, defaultValue);
}

// Safe division with zero check
template<typename T>
inline T SafeDivide(T numerator, T denominator, T defaultValue = 0)
{
    if(denominator == 0)
        return defaultValue;
    return numerator / denominator;
}

// Specialized for floating point with epsilon check
template<>
inline float SafeDivide(float numerator, float denominator, float defaultValue)
{
    if(std::abs(denominator) < std::numeric_limits<float>::epsilon())
        return defaultValue;
    return numerator / denominator;
}

template<>
inline double SafeDivide(double numerator, double denominator, double defaultValue)
{
    if(std::abs(denominator) < std::numeric_limits<double>::epsilon())
        return defaultValue;
    return numerator / denominator;
}
