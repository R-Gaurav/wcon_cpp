#include "ns_wson.h"
#include <fstream>
#include <iostream>

int main()
{
	//first, we build a minimal wson object and write it to the file simple.wson
	ns_worm_tracker_commons_object<> object;

	object.metadata.age = 1.5;
	object.metadata.arena.size[0] = 6;
	object.metadata.arena.size[1] = 7;
	object.metadata.food = "OP50";
	object.metadata.humidity= 0;
	object.metadata.id = "A15";
	ns_wson_interpolation interp;
	interp.method = "cubic bsplines";
	interp.values.push_back("15");
	object.metadata.interpolate.push_back(interp);
	object.metadata.who.push_back("Duke G.W.");
	object.metadata.who.push_back("Stardust Ziggy");
	object.metadata.lab["Institution"] = "CRG";
	object.metadata.lab["Room #"] = "571";
	object.metadata.media = "NGM";
	object.metadata.protocol.push_back("add worms");
	object.metadata.protocol.push_back("...");
	object.metadata.protocol.push_back("proffit");
	object.metadata.sex = ns_wson_metadata::hermaphrodite;
	object.metadata.stage = ns_wson_metadata::L4;
	object.metadata.strain = "TJ1052";
	object.metadata.temperature = 22.5;
	object.metadata.timestamp = 12508089;
	ns_wson_software_metadata sm;
	sm.tracker.name ="Lifespan Machine";
	sm.tracker.version = "2.0";
	sm.featureID = "foo";
	object.metadata.software.push_back(sm);


	object.units.time = ns_wson_units::ns_wson_seconds;
	object.units.x = object.units.y = ns_wson_units::ns_wson_mm;

	object.data.data.resize(50);
	for (int i = 0; i < 50; i++) {
		object.data.data[i].t = i;
		object.data.data[i].x = sin(2*3.141*i/10.0);
		object.data.data[i].y = i/10.0;
	}

	try {
		std::ofstream outf("simple.wson");
		outf << object.to_json().dump(2, ' ', true);
		outf.close();
	}
	catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		char a; a = std::cin.get();
		return 1;
	}

	//next, we build a more complex wson object, and output it to the file rich.wson
	ns_worm_tracker_commons_object<ns_wson_rich_data_record_element, ns_wson_pixel_walk_record> object2;

	object2.metadata.who.push_back("Jopplin J.");
	object2.metadata.who.push_back("Page R.");
	object2.metadata.lab["Institution"] = "IRB";
	object2.metadata.lab["Room #"] = "201";

	object2.units.time = ns_wson_units::ns_wson_seconds;
	object2.units.x = object2.units.y = ns_wson_units::ns_wson_mm;

	object2.data.data.resize(50);
	for (int i = 0; i < 50; i++) {
		object2.data.data[i].t = i;
		object2.data.data[i].x = sin(2 * 3.141*i / 10.0);
		object2.data.data[i].y = i / 10.0;
		object2.data.data[i].ox = sin(2 * 3.141*i / 15.0);
		object2.data.data[i].oy = cos(2 * 3.141*i / 15.0);
		object2.data.data[i].cx= sin(2 * 3.141*i / 5.0);
		object2.data.data[i].cy = cos(2 * 3.141*i / 5.0);
		object2.data.data[i].px = cos(2 * 3.141*i / 50);
		object2.data.data[i].py = cos(2 * 3.141*i / 50);
		object2.data.data[i].ventral = ns_wson_rich_data_record_element::CW;
		object2.data.data[i].head = ns_wson_rich_data_record_element::L;
	}
	object2.data.timeless_data.px.resize(10);
	object2.data.timeless_data.n = 10;
	object2.data.timeless_data.four = 10;
	try {
		std::ofstream outf("rich.wson");
		outf << object2.to_json().dump(2, ' ', true);
		outf.close();

		outf.open("rich_with_measurements_split.wson");
		outf << object2.to_json(false).dump(2, ' ', true);
		outf.close();

	}
	catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		char a; a = std::cin.get();
		return 1;
	}


	try{
		nlohmann::json default_parser;
		std::ifstream infile("simple.wson");
		infile >> default_parser;
		infile.close();
		infile.open("simple.wson");

		ns_worm_tracker_commons_object<> in_sax_object;
		ns_wson_sax_parser<> sax_parser(in_sax_object);
		nlohmann::detail::input_adapter adaptor(infile);
		nlohmann::detail::parser<nlohmann::json> parser(adaptor, nullptr);
		parser.sax_parse(&sax_parser);
    
	}
	catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		char a; a = std::cin.get();
		return 1;
	}
	return 0;
}

