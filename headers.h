#ifndef HEADERS
#define HEADERS

#include <iostream>
#include <memory>
#include <math.h>
#include <string>
#include <time.h>
#include <future>
#include <regex>
#include <stack>
#include <codecvt>
#include <cassert>
#include <random>
#include <iterator>
#include <map>
#include <unordered_map>
#include <cstring>
#include <stdexcept>
#include <valarray>
#include <vector>
#include <array>
#include <algorithm>
#include <ratio>
#include <type_traits>
#include <queue>
#include <ctime>
#include <sstream>
#include <streambuf>
#include <tuple>
#include <type_traits>
#include <fstream>
#include <typeinfo>
#include <chrono>
#include <cstdarg>
#include <initializer_list>
#include <thread>

#define len(a) sizeof(a) / sizeof(a[0])
// #define is ==
#define equals ==
#define cls clear();
#define ln "\n"
#define in :
#define isnot !=
#define OUT

/*---------------------------Colors---------------------------*/
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#define RECT "\e[7m"
#define yellow RECT, YELLOW
#define cyan RECT, CYAN
#define green RECT, GREEN
#define red RECT, RED
#define blue RECT, BLUE
#define white RECT, WHITE
#define boldwhite RECT, BOLDWHITE
#define boldcyan RECT, BOLDCYAN
#define boldyellow RECT, BOLDYELLOW
#define boldgreen RECT, BOLDGREEN

/*---------------------------SuperScript---------------------------*/

#define sup0 "\u2070"
#define sup1 "\u00b9"
#define sup2 "\u00b2"
#define sup3 "\u00b3"
#define sup4 "\u2074"
#define sup5 "\u2075"
#define sup6 "\u2076"
#define sup7 "\u2077"
#define sup8 "\u2078"
#define sup9 "\u2079"

#define sub0 "\u2080"
#define sub1 "\u2081"
#define sub2 "\u2082"
#define sub3 "\u2083"
#define sub4 "\u2084"
#define sub5 "\u2085"
#define sub6 "\u2086"
#define sub7 "\u2087"
#define sub8 "\u2088"
#define sub9 "\u2089"

using namespace std;
using namespace std::chrono;

namespace std
{

    /*---------------------------Utility Functions---------------------------*/

    template <typename type>
    bool inline is_ready(future<type> &f)
    {
        return f.wait_for(0ms) == future_status::ready;
    }

    constexpr unsigned fact(unsigned n)
    {
        return n <= 0 ? 1 : fact(n - 1) * n;
    }
    template <typename T, typename Fn>
    auto map_fn(Fn fn, vector<T> xs)
    {
        vector<decltype(fn(xs.at(0)))> ret;
        for_each(begin(xs), end(xs), [&](T x) { ret.push_back(fn(x)); });
        return ret;
    }
    vector<int> parseInt(const string &a)
    {
        istringstream istrm(a);
        vector<int> ints;
        copy(istream_iterator<int>(istrm), istream_iterator<int>(), back_inserter(ints));
        return ints;
    }

    vector<string> tokenize(const string &str, const string &rgx = " ")
    {
        const regex re{rgx};
        const vector<string> tokens(sregex_token_iterator(begin(str), end(str), re, -1), sregex_token_iterator());
        return tokens;
    }
    auto upper(const string &a) -> string
    {
        string ret = "";
        for (auto c in a)
        {
            c = toupper(c);
            ret += string{c};
        }
        return ret;
    }

    auto lower(const string &a) -> string
    {
        string ret = "";
        for (auto c in a)
        {
            c = tolower(c);
            ret += string{c};
        }
        return ret;
    }

    vector<double>
    parseDouble(const string &a)
    {
        istringstream istrm(a);
        vector<double> ds;
        copy(istream_iterator<double>(istrm), istream_iterator<double>(), back_inserter(ds));
        return ds;
    }
    vector<int> parseDigits(int a)
    {
        vector<int> digits;
        while (a / 10 != 0)
        {
            digits.push_back(a % 10);
            a /= 10;
        }
        digits.push_back(a);
        reverse(begin(digits), end(digits));
        return digits;
    }
    template <typename T>
    string type(T a)
    {
        try
        {
            return typeid(T).name();
        }
        catch (exception)
        {
            return "Null";
        };
    }
    template <int N>
    auto constexpr range() -> decltype(auto)
    {

        return range(make_integer_sequence<unsigned, N>{});
    }
    template <typename T, T... ints>
    auto range(integer_sequence<T, ints...> int_seq) -> vector<int>
    {
        vector<int> a;
        ((a.push_back(ints)), ...);

        return a;
    }
    template <typename T>
    auto tostr(T a) -> string
    {
        std::ostringstream strs;
        strs << a;
        return strs.str();
    }
    auto strip(const string &a, int from = 0) -> string
    {
        /* -1 left 0 both 1 right */

        int start = 0, end = a.size();

        if (from != 1)
            start = a.find_first_not_of(' ');
        if (from != -1)
            end = a.find_last_not_of(' ');
        if (start == end) /* incase whole whitespaces */
            return "";
        return a.substr(start, end + 1 - start);
    }

    template <typename Fn>
    auto timeit(Fn fn)
    {
        auto b = system_clock::now();
        auto ret = fn();
        auto e = system_clock::now();
        duration<double, milli> d = e - b;
        cout << "Time taken: " << d.count() << endl;
        return ret;
    }

    /*---------------------------Modifiers---------------------------*/
    auto sup(int a) -> string
    {
        string ret = "";
        for (auto d in parseDigits(a))
        {
            switch (d)
            {
            case 0:
                ret += sup0;
                break;
            case 1:
                ret += sup1;
                break;
            case 2:
                ret += sup2;
                break;
            case 3:
                ret += sup3;
                break;
            case 4:
                ret += sup4;
                break;
            case 5:
                ret += sup5;
                break;
            case 6:
                ret += sup6;
                break;
            case 7:
                ret += sup7;
                break;
            case 8:
                ret += sup8;
                break;
            case 9:
                ret += sup9;
                break;
            }
        }
        return ret;
    }
    auto sub(int a) -> string
    {
        string ret = "";
        for (auto d in parseDigits(a))
        {
            switch (d)
            {
            case 0:
                ret += sub0;
                break;
            case 1:
                ret += sub1;
                break;
            case 2:
                ret += sub2;
                break;
            case 3:
                ret += sub3;
                break;
            case 4:
                ret += sub4;
                break;
            case 5:
                ret += sub5;
                break;
            case 6:
                ret += sub6;
                break;
            case 7:
                ret += sub7;
                break;
            case 8:
                ret += sub8;
                break;
            case 9:
                ret += sub9;
                break;
            }
        }
        return ret;
    }
    /*---------------------------Matric * vector---------------------------*/

    /*---------------------------Prints---------------------------*/
    bool print(bool line = true)
    {

        line ? cout << " " << endl : cout << " ";
        return 1;
    }

    template <typename T>
    bool print(const T a, bool line = true)
    {
        line ? cout << a << endl : cout << a;
        return 1;
    }

    bool printArr(int a[], int end, int start = 0, bool line = true)
    {
        print("(", false);
        for (int i = start; i < end; i++)
        {
            cout << a[i] << " , ";
        }
        end - start != 0 ? print("\b\b)", false) : print(")", false);
        line ? cout << endl : cout << " ";
        return true;
    }
    bool print(const int a[], int l, bool line = true)
    {
        cout << "( ";
        for (int i = 0; i < l; i++)
        {
            cout << a[i] << " , ";
        }
        l != 0 ? print("\b\b)", false) : print(")", false);
        line ? cout << endl : cout << "";
        return 1;
    }

    template <typename T>
    bool print(const vector<T> a, bool line = true)
    {
        print("[", false);
        for_each(a.begin(), a.end(), [&](T n) { cout << "" ;
    print(n,false);
    cout<< ", "; });
        a.size() != 0 ? print("\b\b]", false) : print("]", false);
        line ? cout << endl : cout << " ";
        return true;
    }

    template <typename T1, typename T2>
    bool print(const pair<T1, T2> &a, bool line = true)
    {
        print("(", false);
        print(a.first, false);
        print(",", false);
        print(a.second, false);
        print(")", false);
        line ? cout << endl : cout << " ";
    }

    template <typename K, typename V>
    bool print(const map<K, V> a, bool line = true)
    {
        print("{", false);
        for_each(a.begin(), a.end(), [&](pair<K, V> n) { cout << "( \'" << n.first << "\' : \'" << n.second << "\' ), "; });
        a.size() != 0 ? print("\b\b}", false) : print("}", false);
        line ? cout << endl : cout << " ";
        return true;
    }
    template <typename T>
    bool print(const vector<T> a, int start, int end, bool line = true)
    {
        print("[", false);
        for (int i = start; i < end; i++)
        {
            print(a[i], false);
            cout
                << " , ";
        }
        a.size() != 0 ? print("\b\b]", false) : print("]", false);
        line ? cout << endl : cout << " ";
        return true;
    }
    template <typename T>
    bool print(const string &s, const vector<T> a, bool line = true)
    {

        print(s + "  ", false);
        print("(", false);
        for_each(a.begin(), a.end(), [&](T n) { print(n,false);cout  << ", "; });
        a.size() != 0 ? print("\b\b)", false) : print(")", false);
        line ? cout << endl : cout << " ";
        return true;
    }
    template <typename T>
    bool print(stack<T> a, bool line = true)
    {
        print("[", false);
        while (!a.empty())
        {
            print(a.top(), false);
            a.pop();
        }
        print("]", false);
        line ? cout << endl : cout << " ";
        return 1;
    }

    bool clear()
    {
        system("clear");
        print();
        return 1;
    }
    bool operator""_p(const char *a, size_t)
    {
        print(a);
        return true;
    }

    bool operator""_p(long double a)
    {
        print(a, true);
        return 1;
    }
    bool operator""_p(unsigned long long a)
    {
        print(a, true);
        return 1;
    }
    template <char sep = ' ', typename... Rest>
    bool print(const Rest &... rest)
    {
        ((print(rest, false), sep isnot 'h' and print(sep, false) or print(' ', false)), ...);

        sep == 'h' and print( false) or print();

        return 1;
    }

    /*--------------------------SAMPANNA'S ADDITIONS----------------*/
    auto sort_map_by_value_ascending(map<string, int> &M)
    {
        multimap<int, string> MM;
        for (auto &it : M)
        {
            MM.insert({it.second, it.first});
        }
        return MM;
    }

    auto sort_map_by_value_descending(map<string, int> &M)
    {

        // Declare a multimap
        multimap<int, string, greater<int>> MM;

        // Insert every (key-value) pairs from
        // map M to multimap MM as (value-key)
        // pairs
        for (auto &it : M)
        {
            MM.insert({it.second, it.first});
        }

        return MM;
    }

} // namespace std

#endif
