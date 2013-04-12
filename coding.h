#include "gmatrix.h"

using namespace std;


class Coding{
private:
    int k;
    int n;
    Gmatrix cmat;

public:
    Coding();
    ~Coding();
    int Encode(method, fromPath, toPath);
    int Decode(method, fromPath, toPath);
};
