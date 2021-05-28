// #include "_data_structures.h"
#ifndef API_PARSER
#define API_PARSER

#include "_map_utils.h"
#include "jsonxx.h"

#define _BASE "https://smarandhg:gN63C4nMUwB4zYX@opensky-network.org/api/"
#define _ALL_STATE "states/all"
#define _FLTS_INTVL "flights/all"
#define _FLTS_ARCFT "flights/aircraft" //flights by aircrafts
#define _AR_ARPT "flights/arrival"     //arrival by aiport
#define _DP_ARPT "flights/departure"   //departure by airport
#define _TRK_ARCFT "tracks/all"        //track by aricraft
/*________________FLAGS FOR RESPONSE TYPE_______________*/

#define STATES 000
#define FLIGHTS 001
#define TRACK 010

/*________________FLAGS FOR FLIGHT INFO FROM_______________*/

#define INTERVAL 011
#define AIRCRAFT 100
#define AR_AIRPORT 101
#define DP_AIRPORT 110

using namespace jsonxx;
struct JSONKeyError
{
    string msg;
};

static linkedlist_<string> _rsp_all_state = {"icao24", "callsign", "origin_airport", "time_position", "last_contact", "longitude", "latitude", "baro_altitude", "on_ground", "velocity", "true_track", "vertical_rate", "sensors", "geo_altitude", "squawk", "spi", "position_source"};
// response fo flight by time,by aricraft,arrival by aiport ,departure by airport
static linkedlist_<string> _rsp_flts = {"icao24", "firstSeen", "estDepartureAirport", "lastSeen", "estArrivalAirport", "callsign", "estDepartureAirportHorizDistance", "estDepartureAirportVertDistance", "estArrivalAirportVertDistance", "departureAirportCandidatesCount", "arrivalAirportCandidatesCount"};
static linkedlist_<string> _rsp_trck = {"icao24", "startTime", "endTime", "callsign", "path"};

// path or waypoints of the trajectory is accully the array and it filds are
static linkedlist_<string> _path = {"time", "latitude", "longitude", "baro_altitude", "true_track", "on_ground"};

static BinaryTree_<string, linkedlist_<string>> _all_states_icao24 = {};
static BinaryTree_<string, linkedlist_<string>> _all_states_callsign = {};
static BinaryTree_<string, linkedlist_<string>> _flt_dep_apt = {};
static BinaryTree_<string, linkedlist_<string>> _flt_arr_apt = {};

auto __gtCommand(linkedlist_<string> args = {}, const int of = STATES, const int from = INTERVAL) -> string
{
    string ret = _BASE;
    auto map_kwargs = [&](SparseLinkedList_<string> kw) -> linkedlist_<string> {
        int i = 0;
        ret += "?";
        linkedlist_<string> ret_;
        for (auto a : args)
            ret_.einsert(kw[i] + "=" + a), i++;
        return ret_;
    };

    switch (of)
    {
    case STATES:
        ret += _ALL_STATE;

        if (args.size())
            for (auto a : map_kwargs({{"icao24", args.size() - 1}, {"time", 1}}))
                ret += a + "&";
        else
            ret += " ";
        break;

    case FLIGHTS:

        switch (from)
        {
        case INTERVAL:
            ret += _FLTS_INTVL;
            for (auto arg : map_kwargs({{"begin", 1}, {"end", 1}}))
                ret += arg + "&";
            break;

        case AIRCRAFT:
            ret += _FLTS_ARCFT;
            for (auto arg : map_kwargs({{"icao24", 1}, {"begin", 1}, {"end", 1}}))
                ret += arg + "&";
            break;

        case AR_AIRPORT:
            ret += _AR_ARPT;
            for (auto arg : map_kwargs({{"airport", 1}, {"begin", 1}, {"end", 1}}))
                ret += arg + "&";
            break;

        case DP_AIRPORT:
            ret += _DP_ARPT;
            for (auto arg : map_kwargs({{"airport", 1}, {"begin", 1}, {"end", 1}}))
                ret += arg + "&";
            break;
        }
        break;
    case TRACK:
        ret += _TRK_ARCFT;
        for (auto arg : map_kwargs({{"icao24", 1}, {"time", 1}}))
            ret += arg + "&";
        break;
    default:
        break;
    }
    ret.erase(ret.end() - 1);
    print(ret);
    return ret;
}

auto fetch_all_states(const time_t &time = 0, linkedlist_<string> icao24s = {}) -> bool
{
    print("Fetching fight data");
    if (time)
        icao24s.einsert(tostr(time));
    string command = __gtCommand(icao24s, STATES);
    command = "curl \"" + command + "\" -o ./all_state.json";
    print(command);
    system(command.c_str());
    return 1;
}

auto fetch_flight(linkedlist_<string> args, const int from = INTERVAL) -> bool
{
    print("Fetching flight data");
    string command = __gtCommand(args, FLIGHTS, from);
    command = "curl \"" + command + "\" -o ./flight.json";
    print(command);

    system(command.c_str());
    return 1;
}

auto fetch_tracking_data(const string &icao24, const time_t &time = 0) -> bool
{
    string command = __gtCommand({icao24, tostr(time)}, TRACK);
    command = "curl \"" + command + "\" -o ./track.json";
    print(command);

    system(command.c_str());
    return 1;
}

auto __toJSONObj(string fpath, OUT Object &o)
{
    try
    {
        string a = "";
        fstream f(fpath);
        f.seekg(0, ios::end);

        a.clear();
        a.reserve(f.tellg());
        f.seekg(0, ios::beg);
        a.assign(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
        if (a.front() != '{')
            o.parse("{\"start\":" + a + "}");
        else
            o.parse(a);
        f.clear();
        f.close();
    }
    catch (exception e)
    {
        std::cout << "hamrai error" << std::endl;
    }
}

auto parse_all_state(string key = "callsign") -> BinaryTree_<string, linkedlist_<string>>
{
    print("Parsing all state data");
    Object o;
    __toJSONObj("all_state.json", o);
    Array datas_ = o.get<Array>("states");
    BinaryTree_<string, linkedlist_<string>> ret;
    bool numeric_key_detected_ = 0;

    auto _gtValue = [&](Array items_, int key, bool for_key = 0) -> string {
        string ret = "";
        if (items_.has<Value>(key))
        {
            Value val = items_.get<Value>(key);
            if (val.type_ == val.STRING_)
                ret = *(val.string_value_);
            else if (val.type_ == val.NUMBER_)
            {
                ret = to_string(val.number_value_);
                if (for_key)
                    numeric_key_detected_ = 1;
            }
            else
                ret = "NA";
        }
        else
        {
            throw JSONKeyError{"Array Index Error! while extracting value from json object" + tostr(key)};
        }
        return ret;
    };

    auto _fetchdata = [&](int index) {
        Array items_ = datas_.get<Array>(index);

        linkedlist_<string> data_;
        for (int j = 0; j < items_.size(); j++)
            data_.einsert(_gtValue(items_, j));

        auto key_ = strip(_gtValue(items_, _rsp_all_state.index(key), 1));
        if (!key_.empty() and key_ != "NA") /* if field to be key is not null */
            if (numeric_key_detected_)
                ret.insert(key_, data_, [](string p, string c) -> bool { return stof(p) > stof(c); });
            else
                ret.insert(key_, data_);
    };

    for (int i = 0; i < datas_.size(); i++)
    {
        try
        {
            _fetchdata(i);
        }
        catch (JSONKeyError e)
        {
            print(e.msg);
            // continue;
        }
    }

    return ret;
}

auto parse_flight(string key1 = "-1", int from = INTERVAL) -> BinaryTree_<string, linkedlist_<string>>
{

    string key = _rsp_flts[0]; //icao24
    switch (from)
    {
    case AR_AIRPORT:
        key = _rsp_flts[4]; //estimated arrival airport
        break;
    case DP_AIRPORT:
        key = _rsp_flts[2]; //estimated departure airport
        /* code */
        break;
    }
    if (key1 != "-1")
        key = key1;
    print("Parsing flight data with", key);
    bool numeric_key_detected_ = 0;

    Object o;
    BinaryTree_<string, linkedlist_<string>> ret;
    __toJSONObj("flight.json", o);
    if (o.has<Array>("start"))
    {
        Array datas_ = o.get<Array>("start");

        auto _gtValue = [&](Object items_, string key, bool for_key = 0) {
            string ret = "";
            if (items_.has<Value>(key))
            {
                Value val = items_.get<Value>(key);
                if (val.type_ == val.STRING_)
                    ret = *(val.string_value_);
                else if (val.type_ == val.NUMBER_)
                {
                    ret = to_string(val.number_value_);
                    if (for_key)
                        numeric_key_detected_ = 1;
                }
                else
                    ret = "NA";
            }
            else
            {
                throw JSONKeyError{"JSON Object Key Error! while extracting value from json object" + tostr(key)};
            }
            return ret;
        };

        auto _fetchdata = [&](int index) {
            Object item_ = datas_.get<Object>(index);
            linkedlist_<string> data_;

            for (auto k : _rsp_flts)
                data_.einsert(_gtValue(item_, k));

            auto key_ = strip(_gtValue(item_, key, 1));
            if (!key_.empty() and key_ != "NA") /* if field to be key is not null */
                if (numeric_key_detected_)
                    ret.insert(key_, data_, [](string p, string c) -> bool { return stof(p) > stof(c); });
                else
                    ret.insert(key_, data_);
        };

        linkedlist_<thread *> threadpool;
        for (int i = 0; i < datas_.size(); i++)
        {
            try
            {
                threadpool.einsert(new thread{_fetchdata, i});
            }
            catch (JSONKeyError e)
            {
                print(e.msg, "at index", i);
                continue;
            }
        }
        for (auto t : threadpool)
            t->join();
    }

    return ret;
}

auto parse_track(const string &icao24) -> BinaryTree_<string, string>
{
    if (_all_states_icao24.empty())
        _all_states_icao24 = parse_all_state("icao24");
    auto tracking_data = _all_states_icao24[icao24];
    int i = 0;
    BinaryTree_<string, string> ret;
    if (tracking_data)
    {
        auto track = *tracking_data;
        for (auto key : _rsp_all_state)
        {
            print(key, ":", track[i]);
            ret.insert(key, track[i]);
            i++;
        }
    }
    else
    {
        print("Tracking data not available for flight", icao24);
    }
    return ret;
}

auto fetch_busiest_airports() -> BinaryTree_<int, string>
{
    int hrsBefore = 24;
    int interval = 2;
    //time_t start = time(nullptr) - (hrsBefore + interval) * 3600;
    //time_t end = time(nullptr) - (hrsBefore)*3600;
    //fetch_flight({to_string(start), to_string(end)});
    if (_flt_arr_apt.empty())
        _flt_arr_apt = parse_flight("estArrivalAirport");

    BinaryTree_<string, int> temp;
    string airport = "";
    for (auto f : _flt_arr_apt.Keys())
    {
        airport = gt_ap_name(f);
        if (airport != "NA")
        {
            ++temp.getDefault({airport, 1});
        }
    }
    BinaryTree_<int, string> ret;
    temp.traverse([&](string airport, int count) -> void {
        ret.insert(count, airport, [](auto p, auto c) -> bool { return c > p; });
    });
    return ret;
}

auto find_flight(const string &dep_ct, const string &arr_ct)
{
    if (_flt_arr_apt.empty())
        _flt_arr_apt = parse_flight("estArrivalAirport");
    string arrival_ct = "", depart_ct = "";
    linkedlist_<string> match;
    for (auto f : _flt_arr_apt.Keys())
    {
        depart_ct = gt_ct_name(_flt_arr_apt[f]->at(2));
        arrival_ct = gt_ct_name(f);
        if (arrival_ct != depart_ct && arrival_ct != "NA" && depart_ct != "NA")
        {
            match.einsert(_flt_arr_apt[f]->at(5));
            std::cout << gt_ct_name(f) << " to " << gt_ct_name(_flt_arr_apt[f]->at(2)) << std::endl;
        }
    }
    return match;
}

auto find_flight_by_departure(const string &dep_ct) -> pair<BinaryTree_<string, string>, BinaryTree_<string, string>>
{
    print("Booking flight by departure country,", dep_ct);
    if (_flt_dep_apt.empty())
        _flt_dep_apt = parse_flight("estDepartureAirport");

    string arrival_ct = "", depart_ct = "";
    BinaryTree_<string, string> intl_flights = {}, dom_flights = {};
    auto fill_ = [&](auto f) {
        arrival_ct = upper(gt_ct_name(_flt_dep_apt[f]->at(4)));
        depart_ct = upper(gt_ct_name(f));
        if (dep_ct == depart_ct)
        {
            print(dep_ct, "to", arrival_ct);
            /*-----------INTERNATIONAL FLIGHTS-------------*/
            if (arrival_ct != "NA" && arrival_ct != dep_ct)
                intl_flights.getDefault({gt_ap_name(f)+", "+gt_ct_name(f), gt_ap_name(_flt_dep_apt[f]->at(4))+", "+gt_ct_name(_flt_dep_apt[f]->at(4))});

            /*-------------DOMESTIC FLIGHTS--------------*/
            else if (arrival_ct == dep_ct)
                dom_flights.getDefault({gt_ap_name(f), gt_ap_name(_flt_dep_apt[f]->at(4))});
        }
    };

    for (auto f : _flt_dep_apt.Keys())
        fill_(f);

    return {intl_flights, dom_flights};
}

#endif
