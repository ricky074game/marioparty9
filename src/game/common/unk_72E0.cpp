/* Shared by most REL modules. Reference copy: mg9101 (fn_18_72E0). */
#include "types.h"

void operator delete(void*);

/* main.dol model object */
class Model {
public:
    ~Model(); /* fn_8003F020 */
};

/* Owning pointer to a Model */
class ModelPtr {
public:
    ModelPtr();
    ~ModelPtr();

    Model* mPtr;
};

ModelPtr::ModelPtr() : mPtr(0) {}

ModelPtr::~ModelPtr() {
    delete mPtr;
}
