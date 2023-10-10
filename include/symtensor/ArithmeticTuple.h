#ifndef SYMTENSOR_ARITHMETICTUPLE_H
#define SYMTENSOR_ARITHMETICTUPLE_H

#include <tuple>

namespace symtensor {

    template<class Implementation, typename ...Types>
    class ArithmeticTupleBase {

        using Self = ArithmeticTupleBase<Implementation, Types...>;

        std::tuple<Types...> _tuple;

    public: // Constructors

        template<typename ...Args>
        explicit ArithmeticTupleBase(Args &&... args) : _tuple(std::forward<Args>(args)...) {}

    public: // Tuple-scalar operations

        template<typename T>
        inline constexpr Implementation &operator+=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements += scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator-=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements -= scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator*=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements *= scalar), ...);
            }, _tuple);
            return *static_cast<Implementation *>(this);
        }

        template<typename T>
        inline constexpr Implementation &operator/=(const T &scalar) {
            std::apply([&](Types &... tupleElements) {
                ((tupleElements /= scalar), ...);
            }, _tuple);
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
                ((std::get<i>(_tuple) += std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<decltype(_tuple)>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator-=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) -= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<decltype(_tuple)>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator*=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) *= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<decltype(_tuple)>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation &operator/=(const Implementation &other) {
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                ((std::get<i>(_tuple) /= std::get<i>(other._tuple)), ...);
            }(std::make_index_sequence<std::tuple_size_v<decltype(_tuple)>>{});
            return *static_cast<Implementation *>(this);
        }

        inline constexpr Implementation operator+(const Implementation &other) const { return Self{*this} += other; }

        inline constexpr Implementation operator-(const Implementation &other) const { return Self{*this} -= other; }

        inline constexpr Implementation operator*(const Implementation &other) const { return Self{*this} *= other; }

        inline constexpr Implementation operator/(const Implementation &other) const { return Self{*this} /= other; }


    public: // Comparators

        auto operator<=>(const Self &other) const = default;

    public: // Friend functions

        friend std::ostream &operator<<(std::ostream &out, const Implementation &self) {
            std::apply([&](const Types &... tupleElements) {
                ((out << tupleElements << " "), ...);
            }, self._tuple);
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

#endif //SYMTENSOR_ARITHMETICTUPLE_H
