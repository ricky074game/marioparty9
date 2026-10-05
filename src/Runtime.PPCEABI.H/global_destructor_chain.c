/*
 * Keeps a list of global objects with destructors, so they can be
 * destroyed in reverse order of construction at program exit.
 */

typedef void (*DestructorFunc)(void* object, short flag);

typedef struct DestructorChain {
    struct DestructorChain* next;
    DestructorFunc destructor;
    void* object;
} DestructorChain;

DestructorChain* __global_destructor_chain;

void* __register_global_object(void* object, void* destructor, void* regmem) {
    ((DestructorChain*)regmem)->next = __global_destructor_chain;
    ((DestructorChain*)regmem)->destructor = (DestructorFunc)destructor;
    ((DestructorChain*)regmem)->object = object;
    __global_destructor_chain = (DestructorChain*)regmem;

    return object;
}

void __destroy_global_chain(void) {
    DestructorChain* iter;

    while ((iter = __global_destructor_chain) != 0) {
        __global_destructor_chain = iter->next;
        iter->destructor(iter->object, -1);
    }
}
