#pragma once

#include <atomic>

typedef enum vm_tag_t {
    VM_INT_TAG,
    VM_FLOAT_TAG,
    VM_CHAR_TAG,
    VM_TEXT_TAG,
    VM_COMBINATOR_TAG,
    VM_ARRAY_TAG
};

typedef vm_tagbits_t unsigned int;

const VM_TAG_BITS   = 3;
const VM_TAG_MASK   = (1<<VM_TAG_BITS)-1;
const VM_RC_ONE     = 1<<VM_TAG_BITS;
const VM_RC_MASK    = ~VM_TAG_MASK;

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

union vm_base_t {
    vm_tagbits_t    tagbits;
    vm_base_t*      next;
};

inline vm_tag_t vm_base_tag(const vm_base_t* p) {
    return vm_tagbits_tag(p->tagbits);
};

inline vm_tag_t vm_base_rc(const vm_base_t* p) {
    return vm_tagbits_rc(p->tagbits);
};

inline void vm_base_inc(const vm_base_t* p) {
    bool updated = false;
    while (!updated) {
        auto bb0 = p->tagbits;
        auto bb1 = vm_tagbits_inc(bb0);
        updated = std::atomic_compare_exchange_weak(&p->tagbits, bb0, bb1);
    }
};

inline void vm_base_free(const vm_base_t* p);

inline void vm_base_dec(const vm_base_t* p) {
    bool updated = false;
    while (!updated) { // an update may fail
        auto bb0 = p->tagbits;
        auto bb1 = vm_tagbits_dec(bb0);
        updated = std::atomic_compare_exchange_weak(&p->tagbits, bb0, bb1);
        if (updated && (vm_tagbits_rc(bb1) == 0)) { // only one thread should free
            vm_base_free(p);
        }
    }
};

struct vm_integer_t {
    vm_base_t       base;
    int             value;
};

inline vm_base_t* vm_integer_create(int v) {
    auto p = (vm_integer_t*) malloc(size_of(vm_integer_t));
    p->base.tagbits = VM_RC_ONE | VM_INT_TAG;
    p->value   = v;
};

inline bool vm_is_integer(const vm_base_t* p) {
    return vm_base_tag(p) == VM_INT_TAG;
};

inline int vm_integer_value(const vm_base_t* p) {
    return ((vm_integer_t*) p)->value;
};

struct vm_float_t {
    vm_base_t       base;
    float           value;
};

inline vm_base_t* vm_float_create(float v) {
    auto p = (vm_float_t*) malloc(size_of(vm_float_t));
    p->base.tagbits = VM_RC_ONE | VM_FLOAT_TAG;
    p->value   = v;
};

inline bool vm_is_float(const vm_base_t* p) {
    return vm_base_tag(p) == VM_FLOAT_TAG;
};

inline float vm_float_value(const vm_base_t* p) {
    return ((vm_float_t*) p)->value;
};

struct vm_char_t {
    vm_base_t       base;
    float           value;
};

inline vm_base_t* vm_char_create(char v) {
    auto p = (vm_char_t*) malloc(size_of(vm_char_t));
    p->base.tagbits = VM_RC_ONE | VM_CHAR_TAG;
    p->value   = v;
};

inline bool vm_is_char(const vm_base_t* p) {
    return vm_base_tag(p) == VM_CHAR_TAG;
};

inline char vm_char_value(const vm_base_t* p) {
    return ((vm_char_t*) p)->value;
};

struct vm_text_t {
    vm_base_t       base;
    void*           value;
};

inline vm_base_t* vm_text_create(void* v) {
    auto p = (vm_text_t*) malloc(size_of(vm_text_t));
    p->base.tagbits = VM_RC_ONE | VM_TEXT_TAG;
    p->value   = v;
};

inline bool vm_is_text(const vm_base_t* p) {
    return vm_base_tag(p) == VM_TEXT_TAG;
};

inline void* vm_text_value(const vm_base_t* p) {
    return ((vm_text_t*) p)->value;
};

struct vm_combinator_t {
    vm_base_t       base;
    void*           value;
};

inline vm_base_t* vm_combinator_create(void* v) {
    auto p = (vm_combinator_t*) malloc(size_of(vm_combinator_t));
    p->base.tagbits = VM_RC_ONE | VM_COMBINATOR_TAG;
    p->value   = v;
};

inline bool vm_is_combinator(const vm_base_t* p) {
    return vm_base_tag(p) == VM_COMBINATOR_TAG;
};

inline void* vm_combinator_value(const vm_base_t* p) {
    return ((vm_combinator_t*) p)->value;
};

struct vm_array_t {
    vm_base_t       base;
    int             size;
    vm_base_t[]     value;
};

inline vm_base_t* vm_array_create(int sz) {
    auto p = (vm_combinator_t*) malloc(size_of(vm_combinator_t));
    p->base.tagbits = VM_RC_ONE | VM_ARRAY_TAG;
    p->size    = sz;
    for (int n = 0; n < sz; n++) {
        p->value[n] = nullptr;
    }
};

inline bool vm_is_array(const vm_base_t* p) {
    return vm_base_tag(p) == VM_ARRAY_TAG;
};

inline int vm_array_size(const vm_base_t* p) {
    return ((vm_array_t*) p)->size;
};

inline vm_base_t* vm_array_get(const vm_base_t* p, int n) {
    return ((vm_array_t*) p)->value[n];
};

inline void vm_array_set(const vm_base_t* p, int n, const vm_base_t* v) {
    ((vm_array_t*) p)->value[n] = v;
};

struct vm_list {
    vm_list*    next;
};

inline void vm_list_append(const vm_base_t* p, const vm_list* ll) {
    auto ll0 = (vm_list*) p;
    ll0->next = 
    return ((vm_list*) p)->next = 
};

inline void vm_array_free(const vm_base_t* p) {
    
};

inline void vm_base_free(const vm_base_t* p) {
    auto t = vm_base_tag(p);
    if (t != VM_ARRAY_TAG) {
        free(p);
    } else {
        vm_array_free(p);
    }
};

