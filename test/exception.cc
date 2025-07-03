#include <exception/exception.hh>

using namespace exception;

struct NoDefaultConstructor_ {
    int n_;
    constexpr NoDefaultConstructor_() noexcept = delete;
    constexpr NoDefaultConstructor_(int n) : n_{n} {}
};

consteval void test_optional() noexcept {
    constexpr auto x = optional<int>{1};
    constexpr auto y = optional<int>{nullopt_t{}};
    static_assert(x.has_value() == true);
    static_assert(y.has_value() == false);
    static_assert(x.value() == 1);
    static_assert(y.value_or(1) == 1);
    constexpr auto _7 = optional<NoDefaultConstructor_>{nullopt_t{}};
    static_assert(_7.has_value() == false);
}

consteval void test_expected() noexcept {
    constexpr int num{};
    constexpr auto x = expected<int, int>{1};
    constexpr auto y = expected<int, int>{::exception::unexpected{1}};
    constexpr auto z = expected<int, int>{num};
    static_assert(x.has_value() == true);
    static_assert(y.has_value() == false);
    static_assert(z.has_value() == true);
    static_assert(x.value() == 1);
    static_assert(y.error() == 1);
    static_assert(x.value_or(2) == 1);
    static_assert(y.value_or(2) == 2);
    // static_assert(y.value_or(2.5) == 2); // error, implicit conversion is not allowed
}

inline void test_optional_in_runtime() noexcept {
    auto x = optional<int>{1};
    x = 2;
    assert_true(x.value() == 2);
    x = nullopt_t{};
    assert_true(x.has_value() == false);
    // has_value(x) = false; // this should be error
}

inline void test_expected_in_runtime() noexcept {
    auto x = expected<int, int>{1};
    x = 2;
    assert_true(x.value() == 2);
    auto y = expected<NoDefaultConstructor_, int>{::exception::unexpected{0}};
    y = NoDefaultConstructor_{1};
    assert_true(y.value().n_ == 1);

    auto z = expected<int, int>{::exception::unexpected{1}};
    x.swap(z);
    assert_true(x.error() == 1);
    assert_true(z.value() == 2);
}

int main() noexcept {
    test_optional_in_runtime();
    test_expected_in_runtime();
    return 0;
}
