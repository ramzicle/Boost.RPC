#ifndef _BOOST_IDL_JSON_HPP_
#define _BOOST_IDL_JSON_HPP_
#include <boost/idl/reflect.hpp>
#include <boost/rpc/datastream.hpp>
#include <boost/rpc/varint.hpp>
#include <boost/rpc/required.hpp>
#include <boost/rpc/debug.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace boost { namespace rpc {

class json_visitor
{
    public:
    json_visitor(std::ostream& os):depth(0),m_os(os),first(true){}
    int depth;

    template<typename Class>
    void start( Class, const char* anem )
    {
        //m_os << (first ? "" : ",\n");
        if( !first )
            m_os<< ",\n"<<std::setw(4*depth)<<" "<<"{";
        else
            m_os<< "{";
        ++depth;
    }

    template<typename Class>
    void end( Class, const char* anem )
    {
            --depth;
        m_os << std::endl;
        m_os<<std::setw(4*depth)<<" "<<"}";
    }

    template<typename Flags>
    inline void add_field( const signed_int& str, const char* name, Flags f )
    {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : ";
        first = true;
        m_os << (first ? "" : ",\n");
        m_os<<std::setw(4*depth)<<" "<<str.value;
        first = false;
    }
    template<typename Flags>
    inline void add_field( const unsigned_int& str, const char* name, Flags f )
    {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : ";
        first = true;
        m_os << (first ? "" : ",\n");
        m_os<<std::setw(4*depth)<<" "<<str.value;
        first = false;
    }
    template<typename Flags>
    inline void add_field( const std::string& str, const char* name, Flags f )
    { 
        if( name != "" )
        {
            m_os << (first ? "\n" : ",\n");
            m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : ";
            first = true;
            m_os << (first ? "" : ",\n");
        }
        m_os<<std::setw(0*depth)<<" "<<'"'<<str<<'"';
        first = false;
    }
    template <typename T, typename Flags>
    inline void add_field( const boost::optional<T>& o, const char* name, Flags f )
    { 
        if( !!o ) add_field( *o, name, f );
        else
        {
            dlog( "%1% was not set", name );
        }
    }
    template <typename T, typename Flags>
    inline void add_field( const boost::rpc::required<T>& r, const char* name, Flags f )
    { 
        if( !!r ) add_field( *r, name, f );
        else
        {
            dlog( "%1% was not set", name );
        }
    }
    
   template <typename T, typename Flags>
   void add_field( T f, const char* name, Flags  )
   {
        if( name != "" )
        {
            m_os << (first ? "\n" : ",\n");
            m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : ";
        }
        first = true;
        boost::idl::reflector<T>::visit( f, *this );
        first = false;
   }

   template<typename Class, typename T, typename Flags>
   void accept_member( Class c, T (Class::*p), const char* name, Flags f )
   {
        add_field(c.*p, name, f);
   }
    template<typename Class, typename Field, typename Alloc, template<typename,typename> class Container, typename Flags>
    void accept_member( Class c, Container<Field,Alloc> (Class::*p), const char* name, Flags key )
    {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : [\n" << std::setw(4*(depth+1))<<" ";
        typename Container<Field,Alloc>::const_iterator itr = (c.*p).begin();
        typename Container<Field,Alloc>::const_iterator end = (c.*p).end();
        ++depth;
        first = true;
        while( itr != end )
        {
            m_os << (first ? "" : ",\n");
            if( !first ) 
            m_os<<std::setw(4*depth)<<" ";// << "{";
            first = true;
            add_field( *itr, (const char*)"", key );
 //           boost::idl::reflector<Field>::visit( *itr, *this );
 //           m_os<<"\n"<<std::setw(4*depth)<<" "<<"}";
            ++itr;
            first  = false;
        }
        --depth;
        m_os<<"\n"<<std::setw(4*depth)<<" "<<"]";
    }
    template<typename Class, typename Flags>
    void accept_member( Class c, signed_int (Class::*p), const char* name, Flags key )
    {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p).value ;
        first = false;
    }
    
    template<typename Class, typename Flags>
    void accept_member( Class c, unsigned_int (Class::*p), const char* name, Flags key )
    {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p).value ;
        first = false;
    }
   template<typename Class, typename Flags>
   void accept_member( Class c, int8_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << int(c.*p) ;
        first = false;
   }
   template<typename Class, typename Flags>
   void accept_member( const Class& c, uint8_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << int(c.*p) ;
        first = false;
   }
    
   template<typename Class, typename Flags>
   void accept_member( Class c, int16_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }
   template<typename Class, typename Flags>
   void accept_member( const Class& c, uint16_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }
   template<typename Class, typename Flags>
   void accept_member( Class c, int32_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }
   template<typename Class, typename Flags>
   void accept_member( const Class& c, uint32_t (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }
   template<typename Class, typename Flags>
   void accept_member( Class c, std::string (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : \"" << (c.*p)<< "\"";
        first = false;
   }
   template<typename Class,typename Flags>
   void accept_member( Class c, double (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }

   template<typename Class,typename Flags>
   void accept_member( Class c, float (Class::*p), const char* name, Flags f )
   {
        m_os << (first ? "\n" : ",\n");
        m_os<<std::setw(4*depth)<<" "<< '"'<<name<< "\" : " << (c.*p) ;
        first = false;
   }

   template<typename Class>
    void not_found( Class c, uint32_t field )
    {
        m_os<<"NOT FOUND FIELD "<< field <<std::endl;
    }
    bool          first;
    std::ostream& m_os;
};

template<typename T>
std::string to_json( const T& v )
{
    std::ostringstream ss;
    json_visitor jv(ss);
    boost::idl::reflector<T>::visit( v,jv);
    return ss.str();
}

} } // namespace boost::rpc
#endif