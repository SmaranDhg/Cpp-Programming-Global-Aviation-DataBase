#include "_data_structures.h"
//its just the directory where data of airline project is held

// get airport, country name

static BinaryTree_<string, string> icao2country = {};
static BinaryTree_<string, string> icao2airport = {};
BinaryTree_<char, pair<int, int>> p2l = {}; // prefix to line no

// here we are implementing binary tree
struct
{

private:
    auto __create_bin_tree_for_line()
    {
        /** \b fetch-the-binary-tree-with-prefix-of-icao-code-as-key
         * \b and-pair-of-starting-line-and-ending-line-as-value
         */
        ifstream f_(dd_ + "/prefix2line.csv");
        char pre_;
        f_.seekg(0, ios::beg);
        int ln_start_ = 0, ln_end_ = 0; // line number
        for (string str; getline(f_, str);)
        {
            pre_ = tokenize(str, ",").at(0).at(0);
            ln_start_ = ln_end_;
            ln_end_ = stoi(tokenize(str, ",").at(1));
            p2l.insert(pre_, make_pair(ln_start_, ln_end_));
        }
        f_.clear();
        f_.close();
    }
    /* return the airport name  */
    auto __gt_country_name(const string &icao) -> string
    {
        if (p2l[icao.at(0)])
        {
            auto [start_pt, end_pt] = *(p2l[icao.at(0)]);

            int i = 0;
            fstream f_(dd_ + "/icao2country.csv");
            f_.seekg(ios::beg);
            auto tokens = tokenize("");
            for (i = 0; i < start_pt - 1; ++i) /* move to starting line for the prefix  */
                f_.ignore(numeric_limits<streamsize>::max(), '\n');

            for (string str; getline(f_, str) && i <= end_pt; i++)
            {
                tokens = tokenize(str, ",");
                if (tokens.size() == 2)
                    icao2country.getDefault({tokens.at(0), strip(tokens.at(1))});
                if (icao == tokens.at(0))
                    return tokens.at(1);
            }

            f_.clear();
            f_.close();
        }
        return "NA";
    }

public:
    auto operator()(const string &icao) -> string
    {
        string *ret = icao2country[icao];
        if (ret)
            return *ret;
        if (p2l.empty())
            __create_bin_tree_for_line();
        return __gt_country_name(upper(icao));
        // return "N";
    }

} gt_ct_name;
struct
{

private:
    auto __create_bin_tree_for_line()
    {
        /** \b fetch-the-binary-tree-with-prefix-of-icao-code-as-key
         * \b and-pair-of-starting-line-and-ending-line-as-value  
         */
        ifstream f_(dd_ + "/prefix2line.csv");
        char pre_;
        f_.seekg(0, ios::beg);
        auto token = tokenize(" ");
        int ln_start_ = 0, ln_end_ = 0; // line number
        for (string str; getline(f_, str);)
        {
            token = tokenize(str, ",");
            pre_ = token.at(0).at(0);
            ln_start_ = ln_end_;
            ln_end_ = stoi(token.at(1));
            p2l.insert(pre_, make_pair(ln_start_, ln_end_));
        }

        f_.clear();
        f_.close();
    }
    /* return the airport name  */
    auto __gt_airport_name(const string &icao) -> string
    {
        if (p2l[icao.at(0)])
        {
            auto [start_pt, end_pt] = *(p2l[icao.at(0)]);

            int i = 0;
            fstream f_(dd_ + "/icao2airport.csv");
            f_.seekg(ios::beg);

            auto token = tokenize(" ");
            for (i = 0; i < start_pt - 1; ++i) /* move to starting line for the prefix  */
                f_.ignore(numeric_limits<streamsize>::max(), '\n');

            for (string str; getline(f_, str) && i <= end_pt; i++)
            {
                token = tokenize(str, ",");
                if (token.size() == 2)
                    icao2airport.getDefault({token.at(0), strip(token.at(1))});
                if (icao == token.at(0))
                    return strip(token.at(1));
            }

            f_.clear();
            f_.close();
        }
        return "NA";
    }

public:
    auto operator()(const string &icao) -> string
    {

        string *ret = icao2airport[icao];
        if (ret)
            return *ret;
        if (p2l.empty())
            __create_bin_tree_for_line();
        return __gt_airport_name(upper(icao));
        // return "N";
    }

} gt_ap_name;

/* maps epoch time to its human readable form */
string epc_to_loc(const time_t &rawtime)
{
    return string(asctime(localtime(&rawtime)));
}
