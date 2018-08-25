#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <map>

typedef  std::pair<std::string, std::string> ns_json_field;
typedef  std::map<std::string, std::string>::const_iterator ns_json_map_const_iterator;
typedef  std::vector<std::string>::const_iterator ns_json_vector_const_iterator;
typedef  std::pair<std::string, nlohmann::json> ns_json_object;

struct ns_wson_arena {
	ns_wson_arena():style("petri"),size(2,-1){}
	std::string style, orientation;
	std::vector<double> size;
	bool specified() const { return size.size() >= 2 && size[0] != -1 && size[1] != -1; }

	nlohmann::json to_json() const{
		nlohmann::json j;
		j["style"] = style;
		j["orientation"] = orientation;
		j["size"] = size;
		return j;
	}
};
struct ns_wson_interpolation {
	std::string method;
	std::vector<std::string > values;
	bool specified() const { return !method.empty(); }
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["method"] = method;
		j["values"] = values;
		return j;
	}
};
struct ns_wson_tracker {
	std::string name, version;
	bool specified() const { return !name.empty(); }
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["name"] = name;
		j["version"] = version;
	}
};
struct ns_wson_software_metadata {
	ns_wson_tracker tracker;
	std::string featureID;
	bool specified() const { return tracker.specified(); }
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["tracker"] = tracker.to_json();
		j["featureID"] = featureID;
	}
};
struct ns_wson_pattern_properties{
	std::string title, description;

	nlohmann::json to_json() const {
		nlohmann::json j;
		j["title"] = title;
		j["description"] = description;
	}
};
struct ns_wson_metadata {
	ns_wson_metadata() :timestamp(0), temperature(-1), humidity(-1),stage(ns_stage_na),sex(ns_sex_na),age(-1){}
	typedef enum {ns_stage_na,L1,L2,L3,L4,adult, dauer} ns_stage;
	typedef enum {ns_sex_na,hermaphrodite, male } ns_sex;
	static std::string to_string(const ns_stage & stage) {
		char buf[256];
		switch (stage) {
			case L1: return "L1";
			case L2: return "L2";
			case L3: return "L3";
			case L4: return "L4";
			case adult: return "adult";
			case dauer: return "dauer";
			default:
				snprintf(buf,256, "%d", (int)stage);
				throw std::exception((std::string("Unknown stage type") + buf).c_str());
		}
	}
	static std::string to_string(const ns_sex & sex) {
		char buf[256];
		switch (sex) {
			case hermaphrodite: return "hermaphrodite";
			case male: return "male";
			default: 
				snprintf(buf, 256, "%d", (int)sex);
				throw std::exception((std::string("Unknown sex type") + buf).c_str());
		}
	}
	std::string id;
	std::map<std::string,std::string> lab;
	std::vector<std::string> who;
	unsigned long timestamp;
	double temperature, humidity;
	ns_wson_arena arena;
	ns_stage stage;
	ns_sex sex;
	std::string food, media, strain;
	double age;
	ns_wson_interpolation interpolate;
	ns_wson_software_metadata software;
	std::map<std::string, std::string> additional_properties;
	bool is_specified() const {
		return to_json().size() > 0;
	}
	nlohmann::json to_json() const {
		nlohmann::json j;
		if (!id.empty()) j["id"] = id;
		if (!lab.empty()) j["lab"] = lab;
		if (!who.empty()) j["who"] = who;
		if (timestamp != 0) j["timestamp"] = timestamp;
		if (temperature != -1) j["temperature"] = temperature;
		if (humidity != -1) j["humidity"] = humidity;
		if (arena.specified()) j["arena"] = arena.to_json();
		if (stage != ns_stage_na) j["stage"] = to_string(stage);
		if (sex != ns_sex_na) j["sex"] = to_string(sex);
		if (!food.empty()) j["food"] = food;
		if (!media.empty()) j["media"] = media;
		if (!strain.empty()) j["strain"] = strain;
		if (age != -1) j["age"] = age;
		if (interpolate.specified())j["interpolate"] = interpolate.to_json();
		if (software.specified())j["software"] = interpolate.to_json();
		if (!additional_properties.empty())j["additionalProperties"] = additional_properties;
		return j;
	}
};
struct ns_wson_units {
	typedef enum { ns_wson_seconds, ns_wson_minutes, ns_wson_hours, ns_wson_days, ns_wson_years, ns_wson_mm, ns_wson_cm, ns_wson_m, ns_wson_km, ns_wson_inches, ns_wson_feet, ns_wson_yards, ns_wson_miles } ns_units;
	static std::string to_string(const ns_units & t) {
		switch (t) {
			case ns_wson_seconds: return "s";
			case ns_wson_minutes: return "m";
			case ns_wson_hours: return "h";
			case ns_wson_days: return "d";
			case ns_wson_years: return "y";
			case ns_wson_mm: return "mm";
			case ns_wson_cm: return "cm";
			case ns_wson_m:return "m";
			case ns_wson_km:return "km";
			case ns_wson_inches:return "in";
			case ns_wson_feet:return "ft";
			case ns_wson_yards:return "yd";
			case ns_wson_miles:return "mi";
			default:
			char buf[256];
			snprintf(buf, 256, "%d", (int)t);
			throw std::exception((std::string("Unknown unit:") + buf).c_str());
		}
	}
	ns_units time;
	ns_units x, y;
	std::map<std::string, ns_units> additional_units;
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["t"] = to_string(time);
		j["x"] = to_string(x);
		j["y"] = to_string(y);
		for (std::map<std::string, ns_units>::const_iterator p = additional_units.begin(); p != additional_units.end(); p++)
			j[p->first] = to_string(p->second);
		return j;
	}
};
struct ns_wson_file_list_entry {
	std::string prev, current, next;
	bool is_specified() const { return !prev.empty() || !next.empty(); }
	
	nlohmann::json to_json() const {
		nlohmann::json j;
		if (!prev.empty()) j["prev"] = prev;
		if (!prev.empty()) j["current"] = current;
		if (!next.empty()) j["next"] = next;
		return j;
	}
};


class ns_wson_default_data_record_element {
public:
	//required elements
	std::string id;
	double t, x, y;

	nlohmann::json to_json() const {
		nlohmann::json j;
		j["id"] = id;
		j["t"] = t;
		j["x"] = x;
		j["y"] = y;
		return j;
	}

	//fields required by extendible data type implementation
	const double & get_t() const { return t; }
	const double & get_x() const { return x; }
	const double & get_y() const { return y; }
	const std::string & get_id() const { return id; }

	static int number_of_additional_json_fields() { return 0; }
	static std::string additional_json_field_name(const int &i) {return "";}
	static bool additional_json_field_is_string(const int &i) {return false;}
	std::string additional_json_field_value_string(const int &i) const { return ""; }
	const double additional_json_field_value_double(const int &i) const { return 0; }
};

class ns_wson_rich_data_record_element{
public:
	ns_wson_rich_data_record_element():ventral(ventral_NA), head(head_NA){}
	std::string id;
	double t, x, y;
	const std::string & get_id() const { return id; }
	const double & get_t() const { return t; }
	const double & get_x() const { return x; }
	const double & get_y() const { return y; }
	typedef enum { L, R, head_NA } ns_head;
	typedef enum { CW, CCW, ventral_NA } ns_ventral;
	double ox, oy, cx, cy, px, py;
	ns_head head;
	ns_ventral ventral;
	static std::string to_string(const ns_head & head) {
		switch (head) {
		case L: return "L";
		case R: return "R";
		case head_NA: return "?";
		default:throw std::exception("Unkonwn head spec");
		}
	}
	static std::string to_string(const ns_ventral & ventral) {
		switch (ventral) {
		case L: return "CW";
		case R: return "CCW";
		case ventral_NA: return "?";
		default:throw std::exception("Unkonwn head spec");
		}
	}
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["id"] = id;
		j["t"] = t;
		j["x"] = x;
		j["y"] = y;
		j["ox"] = ox;
		j["oy"] = oy;
		j["cx"] = cx;
		j["cy"] = cy;
		j["px"] = px;
		j["py"] = py;
		j["head"] = to_string(head);
		j["ventral"] = to_string(ventral);
		return j;
	}

	static int number_of_additional_json_fields() { return 8; }
	static std::string additional_json_field_name(const int &i) {
		switch (i) {
		case 0: return "ox";
		case 1: return "oy";
		case 2: return "cx";
		case 3: return "cy";
		case 4: return "px";
		case 5: return "py";
		case 6: return "head";
		case 7: return "ventral";
		default: throw std::exception("Unknown field id");
		}
	}
	static bool additional_json_field_is_string(const int &i) {
		return (i == 6 || i == 7);
	}
	double additional_json_field_value_double(const int &i) const {
		switch (i) {
		case 0: return ox;
		case 1: return oy;
		case 2: return cx;
		case 3: return cy;
		case 4: return px;
		case 5: return py;
		default: throw std::exception("Unknown field id");
		}
	}
	std::string additional_json_field_value_string(const int &i) const {
		char buf[256];
		switch (i) {
		case 6: return to_string(head);
		case 7: return to_string(ventral);
		default: throw std::exception("Unknown field id");
		}
		return buf;
	}
};

struct ns_wson_default_timeless_data_record {
	bool is_specified() const { return false; }
	static std::string json_name() { return ""; }
	nlohmann::json to_json() const {
		throw std::exception("Calling to_json on empty timeless data record");
	}
};

struct ns_wson_pixel_walk_record {
	ns_wson_pixel_walk_record() :n(0) {}
	std::vector<double> px;
	int n;
	std::string four;
	bool is_specified() const { return !px.empty(); }
	static std::string json_name() { return "pixel_walk_record"; }
	nlohmann::json to_json() const {
		nlohmann::json j;
		j["n"] = n;
		j["4"] = four;
		j["px"] = px;
		return j;
	}
};

template<class timepoint_data_t, class additional_timeless_data_t>
class ns_wson_data_record {
public:
	std::vector<timepoint_data_t> data;
	additional_timeless_data_t timeless_data;

	//split timepoints is the standar wson configuration
	//split measurements is more compact
	nlohmann::json to_json(bool split_timepoints = true) const {
		if (split_timepoints) return to_json_split_timepoints();
		else return to_json_split_measurements();
	}

	nlohmann::json to_json_split_timepoints() const {
		if (data.size() == 0)
			throw std::exception(" ns_wson_data_record::to_json()::No data was provided"); 
		nlohmann::json j;
		std::vector<nlohmann::json> tmp;
		tmp.reserve(data.size());
		for (std::vector<double>::size_type i = 0; i < data.size(); i++)
			tmp.push_back(data[i].to_json());
		return nlohmann::json(tmp);
	}
	nlohmann::json to_json_split_measurements() const {
		if (data.size() == 0)
			throw std::exception(" ns_wson_data_record::to_json()::No data was provided");
		nlohmann::json j;

		std::vector<double> tmp;
		std::vector<std::string> tmp_s;
		tmp.resize(data.size());
		tmp_s.resize(data.size());

		//add required t, x, and y	
		for (std::vector<double>::size_type i = 0; i < data.size(); i++)	tmp_s[i] = data[i].get_id();
		j["id"] = tmp_s;
		for (std::vector<double>::size_type i = 0; i < data.size(); i++)	tmp[i] = data[i].get_t();
		j["t"] = tmp;
		for (std::vector<double>::size_type i = 0; i < data.size(); i++)	tmp[i] = data[i].get_x();
		j["x"] = tmp;
		for (std::vector<double>::size_type i = 0; i < data.size(); i++)	tmp[i] = data[i].get_y();
		j["y"] = tmp;

		//add any additional time-varying fields
		for (int k = 0; k < typename timepoint_data_t::number_of_additional_json_fields(); k++) {
			if (typename timepoint_data_t::additional_json_field_is_string(k)) {
				
				for (std::vector<std::string>::size_type i = 0; i < data.size(); i++)
					tmp_s[i] = data[i].additional_json_field_value_string(k);
				j[data[0].additional_json_field_name(k)] = tmp_s;
			}
			else {
				for (std::vector<double>::size_type i = 0; i < data.size(); i++)
					tmp[i] = data[i].additional_json_field_value_double(k);
				j[data[0].additional_json_field_name(k)] = tmp;
			}
		}
		//add any timeless fields
		if (timeless_data.is_specified())
			j[timeless_data.json_name()] = timeless_data.to_json();
		return j;
	}
};

template<class timepoint_data_t = ns_wson_default_data_record_element, class timeless_data_t = ns_wson_default_timeless_data_record>
class ns_worm_tracker_commons_object {
public:
	ns_wson_file_list_entry files;
	ns_wson_units units;
	ns_wson_metadata metadata;
	ns_wson_data_record<timepoint_data_t, timeless_data_t> data;
	nlohmann::json to_json(bool split_timepoints=true) const {
		nlohmann::json j;

		j["data"] = data.to_json(split_timepoints);

		if (files.is_specified()) j["files"] = files.to_json();
		
		j["units"] = units.to_json();
		
		if (metadata.is_specified()) j["metadata"] = metadata.to_json();
		
		return j;
	}
};