/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2AE90). */

struct Object;

bool fn_80042D70(Object* obj);

class Handle {
public:
    Handle(Object* obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    virtual ~Handle() {}

    Object* mObj;
};

void fn_8004A4B0(Handle handle, float time);
void fn_80049DC0(Handle handle, int a, int b);

class HandleList {
public:
    Handle Get(int idx) {
        if (mCount == 0) {
            return Handle(0);
        }
        return mEntries[idx];
    }
    void fn_18_2AE90(int idx);

    int unk0;
    Handle* mEntries;
    int mCount;
};

void HandleList::fn_18_2AE90(int idx) {
    const Handle& handle = Get(idx);
    fn_8004A4B0(handle, 0.0f);
    fn_80049DC0(handle, 1, 1);
}
