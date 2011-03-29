#include <iostream>

using namespace std;

class A
{
public:
  int a;
  void g ()        { cout << "A::g" << endl; }
  void f ()        { cout << "A::f" << endl; }
};
 
class B
{
  int a;
  void h ( char )  { cout << "B::h - char" << endl; }
public:
  int g;
  void f ( int v ) { cout << "B::f" << endl; }
  void h ()        { cout << "B::h - void" << endl; }
  void h ( int )   { cout << "B::h - int"  << endl; }
};
 
class C: public A, public B
{
  int t;
  //...
};
 
void gg ( C *pc )
{
  //pc -> a = 11;     // неоднозначность
  //pc -> f ( 111 );  // неоднозначность
//  pc -> g ();       // неоднозначность
 // pc -> g = 12;     // неоднозначность
  pc -> h ();       // B::h()
  pc -> h ( 13 );   // B::h(int)
//  pc -> h ( 'a' );  // нет доступа к B::h(char)
}
 
int main()
{
  C c;
  C *pc = &c;
  gg ( pc );
//  c.a = 11;         // неоднозначность
//  c.f ( 111 );      // неоднозначность
//  c.g ();           // неоднозначность
//  c.g = 12;         // неоднозначность
  c.h ();           // B::h()
  c.h ( 13 );       // B::h(int)
 // c.h ( 'a' );      // нет доступа к B::h(char)
  return 0;
}
