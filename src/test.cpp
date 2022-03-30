#include <chrono>
#include <thread>
#include <vector>

#include "allocate.hpp"

class Test {
public:
    virtual icu::UnicodeString title() = 0;
    virtual void test() = 0;

    virtual void runtest() {
        std::cout << "## " << title() << " ##" << std::endl;
        test();
    };

    virtual void timedtest() {
        std::cout << "## " << title() << " ##" << std::endl;
        auto start = std::chrono::system_clock::now();
        test();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "time passed " << diff.count() << " s\n";
    }

    virtual void error(const icu::UnicodeString& e) {
        std::cout << "error: " << e << std::endl;
        exit(1);
    }
};

class Test00 : public Test {
public:
    icu::UnicodeString title() override {
        return "sizeof test";
    };

    void test() override {
        std::cout << "sizeof(vm_object_t)\t = " << sizeof(vm_object_t)
                  << std::endl;
        std::cout << "sizeof(vm_integer_t)\t = " << sizeof(vm_integer_t)
                  << std::endl;
        std::cout << "sizeof(vm_float_t)\t = " << sizeof(vm_float_t)
                  << std::endl;
        std::cout << "sizeof(vm_char_t)\t = " << sizeof(vm_char_t) << std::endl;
        std::cout << "sizeof(vm_text_t)\t = " << sizeof(vm_text_t) << std::endl;
        std::cout << "sizeof(vm_combinator_t)\t = " << sizeof(vm_combinator_t)
                  << std::endl;
        std::cout << "sizeof(vm_opaque_t)\t = " << sizeof(vm_opaque_t)
                  << std::endl;
        std::cout << "sizeof(vm_array_t)\t = " << sizeof(vm_array_t)
                  << std::endl;
    };
};

class NoisyF : public VMCombinator {
public:
    NoisyF() {
        std::cout << "F got created" << std::endl;
    }
    ~NoisyF() {
        std::cout << "F got destroyed" << std::endl;
    }
};

class NoisyO : public VMOpaque {
public:
    NoisyO() {
        std::cout << "O got created" << std::endl;
    }
    ~NoisyO() {
        std::cout << "O got destroyed" << std::endl;
    }
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

        std::cout << "create test NoisyO" << std::endl;
        auto O = new NoisyO();
        auto o0 = vm_opaque_create(O);
        vm_object_inc(o0);
        std::cout << "is_opaque: " << vm_is_opaque(o0) << std::endl;
        std::cout << "value: " << vm_opaque_value(o0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(o0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(o0);
        vm_object_dec(o0);

        std::cout << "create test NoisyF" << std::endl;
        auto F = new NoisyF();
        auto a0 = vm_combinator_create(F);
        vm_object_inc(a0);
        std::cout << "is_combinator: " << vm_is_combinator(a0) << std::endl;
        std::cout << "value: " << vm_combinator_value(a0) << std::endl;
        std::cout << "refcount:" << vm_object_rc(a0) << std::endl;
        std::cout << "decref" << std::endl;
        vm_object_dec(a0);
        vm_object_dec(a0);
        delete F;
    };
};

class Test02 : public Test {
    icu::UnicodeString title() override {
        return "allocate arrays";
    };

    vm_object_t* make_array(int size) {
        auto a = vm_array_create(size);
        for (int n = 0; n < size; n++) {
            auto p = vm_integer_create(n);
            vm_array_set(a, n, p);
        };
        return a;
    }

    void print_array(vm_object_t* a) {
        if (vm_is_array(a)) {
            std::cout << "array ";
            auto size = vm_array_size(a);
            std::cout << "[" << size << "] ";
            for (int n = 0; n < size; n++) {
                auto p = vm_array_get(a, n);
                if (vm_is_integer(p)) {
                    std::cout << vm_integer_value(p) << " ";
                } else {
                    error("not an int");
                }
            }
            std::cout << std::endl;
        } else {
            error("not an array");
        }
    }

    void test() override {
        for (int size = 0; size < 10; size++) {
            auto a = make_array(size);
            print_array(a);
            vm_object_dec(a);
        }
        auto a = make_array(1000);
        print_array(a);
        vm_object_dec(a);
    }
};

class Test03 : public Test {
public:
    icu::UnicodeString title() override {
        return "concurrent reference count";
    };

    const int COUNT = 100000;

    static volatile bool start;

    void up_down(int n, vm_object_t* o) {
        std::cout << n << " launched " << std::endl;
        while (!Test03::start)
            ;
        std::cout << n << " counting up" << std::endl;
        for (int n = 0; n < COUNT; n++) {
            vm_object_inc(o);
        }
        std::cout << n << " refcount " << vm_object_rc(o) << std::endl;
        for (int n = 0; n < COUNT; n++) {
            vm_object_dec(o);
        }
        std::cout << n << " refcount " << vm_object_rc(o) << std::endl;
        std::cout << n << " done counting" << std::endl;
    }

    const int THREADS = 10;

    void test() override {
        Test03::start = false;
        auto o = vm_integer_create(42);
        std::thread tt[THREADS];
        for (int n = 0; n < THREADS; n++) {
            tt[n] = std::thread(&Test03::up_down, this, n, o);
        }
        Test03::start = true;
        for (int n = 0; n < THREADS; n++) {
            tt[n].join();
        }
        std::cout << "final refcount " << vm_object_rc(o) << std::endl;
        vm_object_dec(o);
    }
};
volatile bool Test03::start;

class Test04 : public Test {
public:
    icu::UnicodeString title() override {
        return "noisy list";
    };

    vm_object_t* make_list(int n) {
        if (n == 0) {
            return vm_opaque_create(new NoisyO());
        } else {
            auto cons = vm_array_create(2);
            auto el = vm_opaque_create(new NoisyO());
            auto tail = make_list(n-1);
            vm_array_set(cons, 0, el);
            vm_array_set(cons, 1, tail);
            return cons;
        }
    }

    void test() override {
        std::cout << "make list of 5 elements and nil" << std::endl;
        auto l = make_list(5);
        std::cout << "decref list" << std::endl;
        vm_object_dec(l);
    }
};

class Test05 : public Test {
public:
    icu::UnicodeString title() override {
        return "binary trees";
    };

    static constexpr auto MIN_DEPTH = 4;
    static constexpr auto MAX_DEPTH = 21;
    static constexpr auto STRETCH_DEPTH = MAX_DEPTH + 1;

    vm_object_t* make_node(vm_object_t* l, vm_object_t* r) {
        auto t = vm_array_create(2);
        vm_array_set(t, 0, l);
        vm_array_set(t, 1, r);
        return t;
    }

    vm_object_t* make_tree(int n) {
        if (n == 0) {
            return nullptr;
        } else {
            return make_node(make_tree(n - 1), make_tree(n - 1));
        }
    }

    int check(vm_object_t* t) {
        if (t == nullptr) {
            return 1;
        } else if (vm_is_array(t) && (vm_array_size(t) == 2)) {
            return 1 + check(vm_array_get(t, 0)) + check(vm_array_get(t, 1));
        } else {
            error("not a tree");
            return 0;
        }
    }

    static volatile bool start;

    void test_stretch() {
        auto t = make_tree(STRETCH_DEPTH);
        std::cout << "stretch tree of depth " << STRETCH_DEPTH << "\t "
                  << "check: " << check(t) << std::endl;
        vm_object_dec(t);
    }

    int run_parallel(
        unsigned depth, int iterations,
        unsigned int workers = std::thread::hardware_concurrency()) {
        std::vector<std::thread> threads;
        threads.reserve(workers);

        std::atomic_int counter = iterations;
        std::atomic_int output = 0;

        auto me = this;

        for (unsigned i = 0; i < workers; ++i) {
            threads.push_back(std::thread([&counter, depth, &output, me] {
                int checksum = 0;

                while (--counter >= 0) {
                    auto t = me->make_tree(depth);
                    checksum += me->check(t);
                    vm_object_dec(t);
                }

                output += checksum;
            }));
        }

        for (unsigned i = 0; i < workers; ++i) {
            threads[i].join();
        }

        return output;
    }

    void test() override {
        test_stretch();
        auto long_tree = make_tree(MAX_DEPTH);

        for (int d = MIN_DEPTH; d <= MAX_DEPTH; d += 2) {
            const int iterations = 1 << (MAX_DEPTH - d + MIN_DEPTH);
            auto const c = run_parallel(d, iterations);

            std::cout << iterations << "\t trees of depth " << d
                      << "\t check: " << c << "\n";
        }

        std::cout << "long lived tree of depth " << MAX_DEPTH << "\t "
                  << "check: " << check(long_tree) << "\n";
    }
};

int main(int argc, char* argv[]) {
    Test00().runtest();
    Test01().runtest();
    Test02().runtest();
    Test03().runtest();
    Test04().runtest();
    Test05().timedtest();
};
