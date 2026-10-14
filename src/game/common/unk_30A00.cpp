/* Shared by most REL modules. Reference copy: mg9101 (fn_18_30A00). */
#include "game/common/unk_15E80.h"
#include "game/common/unk_30170.h"

int fn_80074EA0();
int fn_800751E0();
void* fn_80070680(int);
int fn_8006DBC0(void*, int);
String fn_8008D150();
void fn_8008D020(int);
void fn_18_17270(MgScene*, int);

/* basic_string::compare(const char*) against the "menu_title" literal */
static inline int CompareMenuTitle(const String& str) {
    size_t n1;
    size_t len2;
    size_t n2;
    n2 = strlen("menu_title");
    len2 = n2;
    n1 = str.size();
    size_t tmp;
    const char* p = str.get(tmp);
    size_t sz = tmp;
    size_t rlen = Min(sz, n1);
    int result = memcmp(p, "menu_title", Min(rlen, len2));
    if (result == 0) {
        if (rlen < n2) {
            return -1;
        }
        if (rlen == n2) {
            return 0;
        }
        return 1;
    }
    return result;
}

void FrameWait::Run() {
    if (m10) {
        if (m8 == fn_80070DC0()) {
            fn_80070DB0(mC);
            m10 = 0;
        }
    }
}

/* Task driving the exit fade and the pause countdown of a scene */
class SceneTask : public ProcBase {
public:
    virtual ~SceneTask();
    virtual void Run();

    MgScene* mOwner;
};

void SceneTask::Run() {
    MgScene* scene = mOwner;
    scene->TickFader();
    if (scene->m0C == 4 && scene->m24C) {
        if (scene->m254 > 0 && !scene->m258) {
            if (--scene->m254 <= 0) {
                scene->m24C.get()->v28(0);
            }
        }
    }
}

void Fader::Run() {
    switch (mC) {
    case 1:
        if (fn_80074EA0()) {
            if (m10 == 3) {
                while (true) {
                    String name = fn_8008D150();
                    if (name.size() == 0) {
                        break;
                    }
                    if (CompareMenuTitle(name) == 0) {
                        break;
                    }
                    fn_8008D020(1);
                }
            }
            fn_80070DB0(1);
            fn_18_17270(m14, 0);
        }
        break;
    case 0:
        if (fn_8006DBC0(fn_80070680(0), 0x1F10)) {
            bool ready = false;
            if (fn_80074EA0()) {
                if (fn_800751E0() == 0) {
                    ready = true;
                }
            }
            if (ready) {
                if (m14->m0C != 0) {
                    Request(3);
                }
            }
        }
        break;
    }
}

/* ScopedPtr<Fader>::get */
Fader* fn_18_30E90(Owned<Fader>* p) {
    return p->mPtr;
}

/* ScopedPtr<Obj274>::get */
Obj274* fn_18_30EA0(Owned<Obj274>* p) {
    return p->mPtr;
}

struct FrameRate {
    f32 value;
    FrameRate() : value(60.0f) {}
};

static FrameRate sFrameRate;
