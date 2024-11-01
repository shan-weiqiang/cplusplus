
void f();
void g();
int main(){

    // a has f(); b has both f() and g()
    // during compile-link time, the behavior is following:
    // 1. according to appearing order of lib a and b, searching for
    //  symbol f and g; the first one meeting requirement is written
    //  into binary of app
    // 2. for example, if a is linked before b, since a has f, so a
    //  is linked into app; but g is not in lib a, so it goes on to
    //  search in lib b; lib b has g, so lib b is also linked into
    //  app; even though f is also in lib b, but f is already resloved
    //  in lib a, so the linker ignores it; However, if b is linked
    //  before a, since lib b has both symbols: f and g, so linker
    //  will not go into a, because all symbols are resolved; In this
    //  case, app will only depend on lib b.

    // Summary:
    // 1. In global scope, inside one program, the variables and functions
    //    obey one definition rule(odr), both in compile time and runtime
    // 2. The linker tries to resolve the symbols, as long as the symbol is
    //    resolved, it will ignore all the applicable symbols appearing after
    // 3. The order of apperance of libraries is important since it might be
    //    determining which symbols are used in which libraries.
    f();
    g();
}
