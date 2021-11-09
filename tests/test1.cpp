#define BOOST_TEST_MAIN
#define BOOST_TEST_STATIC_LINK

#include <iostream>
#include "boost/test/unit_test.hpp"

using namespace boost::unit_test;
using namespace std;

int dodaj( int i, int j )
{
    return i + j;
}

BOOST_AUTO_TEST_CASE( testDodaj )
        {
                BOOST_CHECK( dodaj( 2, 2 ) == 4 );
        }
