
#include <amp.h>
#include <stdio.h>

#define FAIL 1

struct print_functor {

  int * data ;

  print_functor() : data( new int[10] ) {}

  void operator() (const concurrency::index<1> & i) const restrict(amp) {
    if ( i[0] < 10 ) data[i[0] ] = i[0] + 10 ;
  }

  inline ~print_functor() {}
  inline print_functor( const print_functor & ) {}
  inline print_functor & operator = ( const print_functor & ) { return *this ;}
};

#if FAIL
template< class FunctorType >
#endif
struct foo {
#if FAIL
  FunctorType pf ;
#else
  print_functor pf ;
#endif

  inline
  void operator() (const concurrency::index<1> & i) const restrict(amp) {
    if ( i[0] < 10 ) pf.data[i[0] ] = i[0] + 10 ;
  }

  foo() : pf( ) {}

  void run() {
  concurrency::parallel_for_each( concurrency::extent<1>(10) , *this );
  }
};

int main()
{
#if FAIL
  foo<print_functor> pf ;
#else
  foo pf ;
#endif

  for ( int i = 0 ; i < 10 ; ++i ) pf.pf.data[i] = 0 ;

  pf.run();

  for ( int i = 0 ; i < 10 ; ++i ) {
    printf("f.data[%d] = %d\n",i,pf.pf.data[i]);
  }
   
  return 0 ;
}

