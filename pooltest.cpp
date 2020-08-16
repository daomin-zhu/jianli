#include "pool.h"
#include <memory>
using namespace std;

void func(){
    cout<<__FILE__<<endl;
}

int main(){
    Pool* p = new Pool("test");
    p->start(4);
    p->run(func);
    p->stop();
    delete p;
    return 0;
}