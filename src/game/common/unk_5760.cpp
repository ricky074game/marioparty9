/* Shared by most REL modules. Reference copy: mg9101 (fn_18_5760). */
#include "types.h"

void operator delete(void*);

class Object {
public:
    virtual ~Object();
};

/* Owning pointer to a polymorphic object */
class ObjectPtr {
public:
    ObjectPtr();
    ~ObjectPtr();

    Object* mPtr;
};

ObjectPtr::ObjectPtr() : mPtr(0) {}

ObjectPtr::~ObjectPtr() {
    delete mPtr;
}
