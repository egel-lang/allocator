#pragma once

#include <atomic>

typedef enum vm_tag_t {
    VM_INT_TAG,
    VM_FLOAT_TAG,
    VM_CHAR_TAG,
    VM_TEXT_TAG,
    VM_OBJECT_TAG,
    VM_ARRAY_TAG
};

typedef vm_tagbits_t unsigned int;

const VM_TAG_BITS   3;
const VM_TAG_MASK   (1<<(VM_TAG_BITS+1))-1;
const VM_RC_ONE     (1<<(VM_TAG_BITS+1));
const VM_RC_MASK    ~VM_TAG_MASK;

inline vm_tag_t vm_tagbits_tag(const vm_tagbits_t bb) {
    return (vm_tag_t) (bb & VM_TAG_MASK);
};

inline vm_tagbits_t vm_tagbits_rc(const vm_tagbits_t bb) {
    return bb>>VM_TAG_BITS;
};

inline vm_tagbits_t vm_tagbits_inc(const vm_tagbits_t bb) {
    return bb+VM_RC_ONE;
};

inline vm_tagbits_t vm_tagbits_dec(const vm_tagbits_t bb) {
    return bb-VM_RC_ONE;
};

typedef struct vm_header_tm {
    vm_tagbits_t    tagbits;
};

inline vm_tag_t vm_header_tag(const vm_header_tm* p) {
    return vm_tagbits_tag(p->tagbits);
};

inline vm_tag_t vm_header_rc(const vm_header_tm* p) {
    return vm_tagbits_rc(p->tagbits);
};

inline void vm_header_inc(const vm_header_tm* p) {
    bool updated = false;
    while (!updated) {
        auto bb0 = p->tagbits;
        auto bb1 = vm_tagbits_inc(bb0);
        updated = std::atomic_compare_exchange_weak(&p->tagbits, bb0, bb1);
    }
};

inline void vm_header_dec(const vm_header_tm* p) {
    bool updated = false;
    while (!updated) {
        auto bb0 = p->tagbits;
        auto bb1 = vm_tagbits_dec(bb0);
        updated = std::atomic_compare_exchange_weak(&p->tagbits, bb0, bb1);
    }
};

