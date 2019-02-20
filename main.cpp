// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <tao/pegtl.hpp>

// Include the analyze function that checks
// a grammar for possible infinite cycles.

#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "Query.h"
#include "double.hpp"

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace db{

//------------------RULES-----------------//

	struct select 
		: pegtl::string< 'S', 'E', 'L', 'E', 'C', 'T', ' ' > {};

	struct from
		: pegtl::string< 'F', 'R', 'O', 'M', ' ' > {};
	
	struct where	
		: pegtl::string< 'W', 'H', 'E', 'R', 'E', ' ' > {};

	struct and_
		: pegtl::string< 'A', 'N', 'D', ' ' > {};
	
	struct or_
		: pegtl::string< 'O', 'R', ' ' > {};

	struct relation
		: pegtl::sor< pegtl::one< '<' >, pegtl::one< '=' >, pegtl::one< '>' > > {};

	struct attrname
		: pegtl::seq< pegtl::plus< pegtl::alpha >, pegtl::one< '.' >, pegtl::plus< pegtl::alpha > > {}; 

	struct tablename
		: pegtl::plus< pegtl::alpha > {};

	struct word
		: pegtl::seq< pegtl::one< '"' >, pegtl::plus< pegtl::alpha >, pegtl::one< '"' > > {};

	struct selparams
		: pegtl::list< attrname, pegtl::one< ',' > > {};

	struct frparams
		: pegtl::list< tablename, pegtl::one< ',' > > {};


	struct whparams;

	struct predicate
		: pegtl::seq< attrname, relation, pegtl::sor< attrname, double_::grammar, word > > {};

	struct bracket_whparam
		: pegtl::if_must< pegtl::one< '(' >, whparams, pegtl::one< ')' > > {};

	struct atomic_whparam
		: pegtl::sor< predicate, bracket_whparam > {};

	struct whparams
		: pegtl::list< atomic_whparam, pegtl::sor< and_, or_ > > {};

	struct basic_query
		: pegtl::seq< select, selparams, pegtl::one< ' ' >, from, frparams, pegtl::one< ' ' > > {};
	
	struct query
		: pegtl::if_must< pegtl::seq< basic_query, whparams, pegtl::one< ';' > > > {};


//------------------ACTIONS-----------------//

	template < typename Rule >
	struct action 
		: pegtl::nothing< Rule > {};

	template<>
	struct action< selparams >
	{
		template< typename Input >
		static void apply( const Input& in, Query& q )
		{
			q.selattrs.push_back(in.string());
		}
	}; 

	template<>
	struct action< tablename >
	{
		template< typename Input >
		static void apply( const Input& in, Query& q )
		{
			q.from.push_back(in.string());
		}
	};

}


int main( int argc, char* argv[] )
{  
	Query q;

	pegtl::read_input in( argv[ 1 ] );
	pegtl::parse< db::query, db::action >(in, q);

	q.print();

	return 0;
}
