#include <shoe.hpp>

void test() {
    Shoe shoe = Shoe(1, false);
    std::cout<<"TEST SHOE PRINTOUT"<<"\n";
    for(int i = 0; i < shoe.size(); i++) {
        std::cout<<shoe[i]<<"\n";
    }
}

int main() {
    test();
}
