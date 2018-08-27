#include "ns_wson.h"
#include "ns_wson_rich_data_record.h"
#include <fstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////
//
// ns_wson_test.cpp
// 
// ns_worm_tracker_commons_object implements an extendable parser/writer for the
// Worm tracker Commons Object Notation schema
// wcon_schema.json 
// as in specified August 2018.
//
// Worm data is specified by a template argument timepoint_data_t
// By default, the parser/writer uses the minimal specification described in the WCON schema
//
// The extendability of this parser/writer is demonstrated in ns_wson_rich_data_record
// which implements the full schema.
//
// Nicholas Stroustrup 
// Centre for Genomic Regulation
// 2018
//
//////////////////////////////////////////////////////////////////////////

int main()
{

	try {

		//////////////////
		//TEST 1: build a minimal wson object and write it to the file simple.wson
		//////////////////
		ns_worm_tracker_commons_object<> object;
		//add metadata
		object.metadata.age = 1.5;
		object.metadata.arena.size[0] = 6;
		object.metadata.arena.size[1] = 7;
		object.metadata.food = "OP50";
		object.metadata.humidity = 0;
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
		object.metadata.protocol.push_back("profit");
		object.metadata.sex = ns_wson_metadata::hermaphrodite;
		object.metadata.stage = ns_wson_metadata::L4;
		object.metadata.strain = "TJ1052";
		object.metadata.temperature = 22.5;
		object.metadata.timestamp = 12508089;
		ns_wson_software_metadata sm;
		sm.tracker.name = "Lifespan Machine";
		sm.tracker.version = "2.0";
		sm.feature_id = "foo";
		object.metadata.software.push_back(sm);

		//set units
		object.units.time = ns_wson_units::ns_wson_seconds;
		object.units.x = object.units.y = ns_wson_units::ns_wson_mm;

		//add data
		object.data.data.resize(50);
		for (int i = 0; i < 50; i++) {
			object.data.data[i].id = "W1";
			object.data.data[i].t = i;
			object.data.data[i].x = sin(2 * 3.141*i / 10.0);
			object.data.data[i].y = i / 10.0;
		}
		object.data.sort_by_time_and_id();
		{
			std::ofstream outf("simple.wson");
			outf << object;
			outf.close();
		}
		//////////////////
		//TEST 2: Build a more complex wson object, and output it to the file rich.wson
		//////////////////

		ns_worm_tracker_commons_object<ns_wson_rich_data_record_element> object2;

		object2.metadata.who.push_back("Jopplin J.");
		object2.metadata.who.push_back("Page R.");
		object2.metadata.lab["Institution"] = "IRB";
		object2.metadata.lab["Room #"] = "201";

		object2.units.time = ns_wson_units::ns_wson_seconds;
		object2.units.x = object2.units.y = ns_wson_units::ns_wson_mm;

		object2.data.data.resize(50);
		for (int i = 0; i < 50; i++) {
			object2.data.data[i].id = "W1";
			object2.data.data[i].t = i;
			object2.data.data[i].x = sin(2 * 3.141*i / 10.0);
			object2.data.data[i].y = i / 10.0;
			object2.data.data[i].ox = sin(2 * 3.141*i / 15.0);
			object2.data.data[i].oy = cos(2 * 3.141*i / 15.0);
			object2.data.data[i].cx = sin(2 * 3.141*i / 5.0);
			object2.data.data[i].cy = cos(2 * 3.141*i / 5.0);
			object2.data.data[i].px.resize(10);
			object2.data.data[i].py.resize(10);
			for (int j = 0; j < 10; j++) {
				object2.data.data[i].px[j] = sin(2 * 3.141*j / 10.0);
				object2.data.data[i].py[j] = cos(2 * 3.141*j / 10.0);
			}
			object2.data.data[i].ventral = (i % 2) ? ns_wson_rich_data_record_element::CW : ns_wson_rich_data_record_element::CCW;
			object2.data.data[i].head = (i % 2) ? ns_wson_rich_data_record_element::L : ns_wson_rich_data_record_element::R;
			object2.data.data[i].walk.n = { 1,5 };
			object2.data.data[i].walk.px = { 5,4,1 };
			object2.data.data[i].walk.four = "MIME_ENCODED_STRING";

		}
		object2.data.sort_by_time_and_id();
		{
			std::ofstream outf("rich.wson");
			outf << object2;
			outf.close();
		}
		//////////////////
		//TEST 3: Write the complex wson object as a split format
		//////////////////
		{
			object2.write_split_timepoint = true;

			std::ofstream outf("rich_with_measurements_split.wson");
			outf << object2.to_json(false).dump(2, ' ', true);
			outf.close();
		}

		//////////////////
		//TEST 4: Read the simple wson object
		//////////////////
		if (1){
			std::ifstream infile("simple.wson");

			ns_worm_tracker_commons_object<> obj;
			infile >> obj;
			infile.close();
		}

		//////////////////
		//TEST 5: Read the complex wson object
		//////////////////
		if (1){
			std::ifstream infile("rich.wson");

			ns_worm_tracker_commons_object<ns_wson_rich_data_record_element> obj;
			infile >> obj;
			infile.close();
			obj.data.sort_by_time_and_id();
			if (object2 == obj)
				std::cout << "Test 5 Passed\n";
		}
		//////////////////
		//TEST 6: Read the complex wson object in split format
		//////////////////
		if (1){
			std::ifstream infile("rich_with_measurements_split.wson");

			ns_worm_tracker_commons_object<ns_wson_rich_data_record_element> obj;
			infile >> obj;

			obj.data.sort_by_time_and_id();
			if (object2 == obj) 
				std::cout << "Test 6 Passed\n";
			else throw std::exception("Failed test 6");
			infile.close();
		}

	}
	catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		char a; a = std::cin.get();
		return 1;
	}
	return 0;
}

