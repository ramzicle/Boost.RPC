#ifndef _BOOST_RPC_LOG_HPP_
#define _BOOST_RPC_LOG_HPP_
#include <boost/format.hpp>
#include <iostream>

#define  COLOR_CONSOLE 1
#include <boost/rpc/log/console_defines.h>

namespace boost { namespace rpc { namespace detail {

    inline void log( std::ostream& os, const char* color, const char* file, uint32_t line, const char* method, const char* text )
    {
        os<<color<<file<<":"<<line<<" "<<method<<"] "<<text<< CONSOLE_DEFAULT << std::endl; 
    }
    template<typename P1>
    inline void log( std::ostream& os, const char* color, const char* file, uint32_t line, const char* method, const std::string& format, const P1& p1 )
    {
        os<<color<<file<<":"<<line<<" "<<method<<"] "<< (boost::format(format) %p1) << CONSOLE_DEFAULT << std::endl;
    }
    template<typename P1, typename P2>
    inline void log( std::ostream& os, const char* color, const char* file, uint32_t line, const char* method, const std::string& format, const P1& p1, const P2& p2 )
    {
        os<<color<<file<<":"<<line<<" "<<method<<"] "<< boost::format(format) %p1 %p2 << CONSOLE_DEFAULT << std::endl;
    }
    template<typename P1, typename P2, typename P3>
    inline void log( std::ostream& os, const char* color, const char* file, uint32_t line, const char* method, const std::string& format, const P1& p1, const P2& p2, const P3& p3 )
    {
        os<<color<<file<<":"<<line<<" "<<method<<"] "<< (boost::format(format) %p1 %p2 %p3) << CONSOLE_DEFAULT << std::endl;
    }

} } } // boost::rpc::detail

#define dlog(...) boost::rpc::detail::log( std::cerr, CONSOLE_DEFAULT, __FILE__, __LINE__, __func__, __VA_ARGS__ )
#define slog(...) boost::rpc::detail::log( std::cerr, CONSOLE_DEFAULT, __FILE__, __LINE__, __func__, __VA_ARGS__ )
#define elog(...) boost::rpc::detail::log( std::cerr, CONSOLE_RED,     __FILE__, __LINE__, __func__, __VA_ARGS__ )
#define wlog(...) boost::rpc::detail::log( std::cerr, CONSOLE_BROWN,   __FILE__, __LINE__, __func__, __VA_ARGS__ )

#endif 
