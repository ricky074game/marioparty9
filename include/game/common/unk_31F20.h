#ifndef GAME_COMMON_UNK_31F20_H
#define GAME_COMMON_UNK_31F20_H

#include "types.h"

/* Text box object (0x1BC bytes) used by the layout classes in most REL modules. */

struct Color {
    u8 r, g, b, a;
    Color(u8 r_, u8 g_, u8 b_, u8 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class TextBox {
public:
    TextBox(int);
    virtual ~TextBox();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void SetText(const char*); /* 0x74 */
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void SetColorTop(const Color*); /* 0x98 */
    virtual void v37();
    virtual void v38();
    virtual void SetColorBottom(const Color*); /* 0xa4 */
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void SetAlign(int, int); /* 0xd4 */
    virtual void v52(); /* 0xd8 */
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void Attach(void*, void*); /* 0xf8 */
    virtual void Release(); /* 0xfc */

    u8 mData[0x1B8];
};

#endif
