#include "allocate.hpp"

#include <thread>

class Test {
public:
    virtual icu::UnicodeString title() = 0;
    virtual void test() = 0;

    virtual void runtest() {
        std::cout << "## " << title() << " ##" << std::endl;
        test();
    };
};

class Test00 : public Test {
public:
    icu::UnicodeString title() override {
        return "sizeof test";
    };

    void test() override {
        std::cout << "sizeof(vm_object_t)\t = " << sizeof(vm_object_t) << std::endl;
        std::cout << "sizeof(vm_integer_t)\t = " << sizeof(vm_integer_t) << std::endl;
        std::cout << "sizeof(vm_float_t)\t = " << sizeof(vm_float_t) << std::endl;
        std::cout << "sizeof(vm_char_t)\t = " << sizeof(vm_char_t) << std::endl;
        std::cout << "sizeof(vm_text_t)\t = " << sizeof(vm_text_t) << std::endl;
        std::cout << "sizeof(vm_combinator_t)\t = " << sizeof(vm_combinator_t) << std::endl;
        std::cout << "sizeof(vm_opaque_t)\t = " << sizeof(vm_opaque_t) << std::endl;
        std::cout << "sizeof(vm_array_t)\t = " << sizeof(vm_array_t) << std::endl;
    };

};

class Test01 : public Test {
    icu::UnicodeString title() override {
        return "allocate atoms";
    };

    void test() override {
        std::cout << "create integer 42" << std::endl;
        auto i0 = vm_integer_create(42);
        vm_object_inc(i0);
        std::cout << "is_integer: " << vm_is_integer(i0) << std::endl;
        std::cout << "value: " << vm_integer_value(i0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(i0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(i0);
        vm_object_dec(i0);

        std::cout << "create float 3.14" << std::endl;
        auto f0 = vm_float_create(3.14);
        vm_object_inc(f0);
        std::cout << "is_float: " << vm_is_float(f0) << std::endl;
        std::cout << "value: " << vm_float_value(f0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(f0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(f0);
        vm_object_dec(f0);

        std::cout << "create char 'Q'" << std::endl;
        auto c0 = vm_char_create('Q');
        vm_object_inc(c0);
        std::cout << "is_char: " << vm_is_char(c0) << std::endl;
        std::cout << "value: " << vm_char_value(c0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(c0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(c0);
        vm_object_dec(c0);

        std::cout << "create test 'hello'" << std::endl;
        auto t0 = vm_text_create("hello");
        vm_object_inc(t0);
        std::cout << "is_text: " << vm_is_text(t0) << std::endl;
        std::cout << "value: " << vm_text_value(t0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(t0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(t0);
        vm_object_dec(t0);
    };
};

int main(int argc, char *argv[]) {
    Test00().runtest();
    Test01().runtest();

};
