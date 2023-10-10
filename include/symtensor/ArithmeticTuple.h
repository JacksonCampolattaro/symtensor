#ifndef SYMTENSOR_ARITHMETICTUPLE_H
#define SYMTENSOR_ARITHMETICTUPLE_H

#include <tuple>
#include <symtensor/util.h>

namespace symtensor {

    template<class Implementation, typename ...Types>
    class ArithmeticTupleBase : public std::tuple<Types...> {

        using Self = ArithmeticTupleBase<Implementation, Types...>;
        using Tuple = std::tuple<Types...>;

    public:

        using std::tuple<Types...>::tuple;

        inline constexpr const std::tuple<Types...> &as_tuple() const { return *this; }

        inline constexpr std::tuple<Types...> &as_tuple() { return *this; }

    public: // Tuple-scalar operations

        template<typename T>
        inline constexpr Implementation &operator+=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements += scalar), ...);
            }, as_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator-=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements -= scalar), ...);
            }, as_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator*=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements *= scalar), ...);
            }, as_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator/=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements /= scalar), ...);
            }, as_tuple());
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation operator+(const T &scalar) const { return Self{*this} += scalar; }

        template<typename T>
        inline constexpr Implementation operator-(const T &scalar) const { return Self{*this} -= scalar; }

        template<typename T>
        inline constexpr Implementation operator*(const T &scalar) const { return Self{*this} *= scalar; }

        template<typename T>
        inline constexpr Implementation operator/(const T &scalar) const { return Self{*this} /= scalar; }

    public: // Tuple-tuple operations

        inline constexpr Implementation &operator+=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(*this) += std::get<i>(other)), ...);
            }(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator-=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(*this) -= std::get<i>(other)), ...);
            }(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator*=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(*this) *= std::get<i>(other)), ...);
            }(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator/=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(*this) /= std::get<i>(other)), ...);
            }(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation operator+(const Implementation &other) const { return Self{*this} += other; }

        inline constexpr Implementation operator-(const Implementation &other) const { return Self{*this} -= other; }

        inline constexpr Implementation operator*(const Implementation &other) const { return Self{*this} *= other; }

        inline constexpr Implementation operator/(const Implementation &other) const { return Self{*this} /= other; }

    public: // Friend functions

        friend std::ostream &operator<<(std::ostream &out, const Implementation &self) {
            std::apply([&](const Types &... tupleElements) {
                ((out << tupleElements << " "), ...);
            }, self.as_tuple());
            return out;
        }
    };

    template<typename ...Types>
    class ArithmeticTuple : public ArithmeticTupleBase<ArithmeticTuple<Types...>, Types...> {
        using Base = ArithmeticTupleBase<ArithmeticTuple<Types...>, Types...>;
        using Base::Base;
    };

    namespace {

        template<typename, typename>
        struct AsArithmeticTupleBaseHelper;

        template<typename Implementation, typename ...Types>
        struct AsArithmeticTupleBaseHelper<Implementation, std::tuple<Types...>> {
            using type = ArithmeticTupleBase<Implementation, Types...>;
        };

        template<typename>
        struct AsArithmeticTupleHelper;

        template<typename ...Types>
        struct AsArithmeticTupleHelper<std::tuple<Types...>> {
            using type = ArithmeticTuple<Types...>;
        };

    }

    template<typename Implementation, typename Tuple>
    using AsArithmeticTupleBase = typename AsArithmeticTupleBaseHelper<Implementation, Tuple>::type;

    template<typename Tuple>
    using AsArithmeticTuple = typename AsArithmeticTupleHelper<Tuple>::type;

}

// Implements tuple functionality for any class which subclasses from ArithmeticTupleBase
namespace std {

    // fixme: the following is preferable, but doesn't work on gcc due to a compiler bug
    // struct tuple_size<T> : tuple_size<symtensor::underlying_tuple<T>> {
    template<symtensor::derived_from_template<symtensor::ArithmeticTupleBase> T>
    struct tuple_size<T> : symtensor::underlying_tuple_size<T> {
    };

    // Similarly, I would prefer:
    // tuple_element<I, symtensor::underlying_tuple<T>>
    template<size_t I, symtensor::derived_from_template<symtensor::ArithmeticTupleBase> T>
    struct tuple_element<I, T> : symtensor::underlying_tuple_element<I, T> {
    };
}

#endif //SYMTENSOR_ARITHMETICTUPLE_H
