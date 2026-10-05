#ifndef GAME_COMMON_UNK_170B0_H
#define GAME_COMMON_UNK_170B0_H

/* Shared scene/process classes linked into most REL modules. */

void* operator new(unsigned long size);
void operator delete(void* p);

/* Inline destructors get stray out-of-line weak copies from MWCC (the
   original linker dead-stripped them); keep those in a separate section so
   .text only holds the real functions. */
#pragma section RX ".dtext"
#define WEAK_DTOR __declspec(section ".dtext")

/* main.dol base class (ctor fn_80071350, dtor fn_80071420) */
class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

    void fn_800714A0(int);
    void fn_800714E0(int);
    void fn_80071580(int);

private:
    int mUnk4;
};

/* Empty base that publishes the most recently constructed instance. */
template <typename T>
class Singleton {
public:
    Singleton(T* inst) { sInstance = inst; }
    WEAK_DTOR ~Singleton() { sInstance = 0; }

    static T* sInstance;
};

/* boost::noncopyable-style empty base */
class noncopyable {
protected:
    noncopyable() {}
    WEAK_DTOR ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

template <class T>
class ScopedPtr : noncopyable {
public:
    ScopedPtr(T* p = 0) : mPtr(p) {}
    WEAK_DTOR ~ScopedPtr() { delete mPtr; }

    T* mPtr;
};

struct Unk1 {};

class SubProc : public ProcBase {
public:
    SubProc() : mBuf(new Unk1) {
        fn_800714E0(6);
        fn_80071580(0x10);
        fn_800714A0(0x7FFFFFFF);
    }
    virtual ~SubProc();
    virtual void Run();

private:
    ScopedPtr<Unk1> mBuf;
};

class SceneBase : public ProcBase, public Singleton<SceneBase> {
public:
    SceneBase();
    virtual ~SceneBase();
    virtual void Unk10();

    SubProc* mSub;
};

void fn_8008CB30(SceneBase*);
void fn_18_17270(SceneBase*, int);

extern "C" unsigned long strlen(const char*);

struct StrTag {
    StrTag() {}
};

/* MSL-style string with short-string optimization (12 bytes).
   Short: bit 31 of word 0 clear, characters from byte 1.
   Long: bit 31 of word 0 set, size at +4, heap buffer at +8. */
class String {
public:
    enum { kWords = 3 };

    String(const char* s) {
        Zero();
        unsigned long n = strlen(s);
        reserve(n);
        Replace(0, 0, s, s + n, StrTag());
    }
    WEAK_DTOR ~String() {
        if (IsLong()) {
            operator delete(mData);
        }
    }

    void Zero() {
        unsigned long* p = &mWord0;
        for (int i = 0; i < kWords; i++) {
            *p++ = 0;
        }
    }

    bool IsLong() const { return mWord0 >> 31; }
    const char* c_str() const { return !IsLong() ? (const char*)this + 1 : mData; }

    void reserve(unsigned long n);                                         /* fn_18_15E80 */
    void Replace(int, int, const char* first, const char* last, StrTag tag); /* fn_18_16040 */

private:
    unsigned long mWord0;
    unsigned long mSize;
    char* mData;
};

String fn_8008D0A0();

class Scene : public SceneBase, public Singleton<Scene> {
public:
    Scene();
    virtual ~Scene();
    virtual void Run();
    virtual void Unk10();
    virtual void Unk14();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Unk20();
};

#endif
