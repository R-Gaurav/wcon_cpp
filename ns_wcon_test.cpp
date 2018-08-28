#include "ns_wcon.h"
#include "ns_wcon_rich_data_record.h"
#include <fstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////
//
// ns_wcon_test.cpp
//
// ns_worm_tracker_commons_object implements an extendable parser/writer for the
// Worm tracker Commons Object Notation schema
// wcon_schema.json
// as specified in August 2018.
//
// This file demonstrates de novo construction of wcon
//
// The extendability of this parser/writer is demonstrated in ns_wcon_rich_data_record
// which implements the full schema.
//
//
// Nicholas Stroustrup
// Centre for Genomic Regulation
// 2018
//
//
//////////////////////////////////////////////////////////////////////////

int main()
{

	try {

		//////////////////
		//TEST 1: build a minimal wcon object and write it to the file simple.wcon
		//////////////////
		ns_worm_tracker_commons_object<> object;
		//add metadata
		object.metadata.age = 1.5;
		object.metadata.arena.size[0] = 6;
		object.metadata.arena.size[1] = 7;
		object.metadata.food = "OP50";
		object.metadata.humidity = 0;
		object.metadata.id = "A15";
		ns_wcon_interpolation interp;
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
		object.metadata.sex = ns_wcon_metadata::hermaphrodite;
		object.metadata.stage = ns_wcon_metadata::L4;
		object.metadata.strain = "TJ1052";
		object.metadata.temperature = 22.5;
		object.metadata.timestamp = "12508089";
		ns_wcon_software_metadata sm;
		sm.tracker.name = "Lifespan Machine";
		sm.tracker.version = "2.0";
		sm.feature_id = "foo";
		object.metadata.software.push_back(sm);

		//set units
		object.units.time = ns_wcon_units::ns_wcon_seconds;
		object.units.x = object.units.y = ns_wcon_units::ns_wcon_mm;

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
			std::ofstream outf("simple.wcon");
			outf << object;
			outf.close();
		}
		//////////////////
		//TEST 2: Build a more complex wcon object, and output it to the file rich.wcon
		//////////////////

		ns_worm_tracker_commons_object<ns_wcon_rich_data_record_element> object2;

		object2.metadata.who.push_back("Jopplin J.");
		object2.metadata.who.push_back("Page R.");
		object2.metadata.lab["Institution"] = "IRB";
		object2.metadata.lab["Room #"] = "201";

		object2.units.time = ns_wcon_units::ns_wcon_seconds;
		object2.units.x = object2.units.y = ns_wcon_units::ns_wcon_mm;

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
			object2.data.data[i].ventral = (i % 2) ? ns_wcon_rich_data_record_element::CW : ns_wcon_rich_data_record_element::CCW;
			object2.data.data[i].head = (i % 2) ? ns_wcon_rich_data_record_element::L : ns_wcon_rich_data_record_element::R;
			object2.data.data[i].walk.n = { 1,5 };
			object2.data.data[i].walk.px = { 5,4,1 };
			object2.data.data[i].walk.four = "MIME_ENCODED_STRING";

		}
		object2.data.sort_by_time_and_id();
		{
			std::ofstream outf("rich.wcon");
			outf << object2;
			outf.close();
		}
		//////////////////
		//TEST 3: Write the complex wcon object as a split format
		//////////////////
		{
			object2.write_split_timepoint = true;

			std::ofstream outf("rich_with_measurements_split.wcon");
			outf << object2.to_json(false).dump(2, ' ', true);
			outf.close();
		}

		//////////////////
		//TEST 4: Read the simple wcon object
		//////////////////
		if (0){
			std::ifstream infile("simple.wcon");

			ns_worm_tracker_commons_object<> obj;
			infile >> obj;
			infile.close();
		}

		//////////////////
		//TEST 5: Read the complex wcon object
		//////////////////
		if (0){
			std::ifstream infile("rich.wcon");

			ns_worm_tracker_commons_object<ns_wcon_rich_data_record_element> obj;
			infile >> obj;
			infile.close();
			obj.data.sort_by_time_and_id();
			if (object2 == obj)
				std::cout << "Test 5 Passed\n";
		}
		//////////////////
		//TEST 6: Read the complex wcon object in split format
		//////////////////
		if (0){
			std::ifstream infile("rich_with_measurements_split.wcon");

			ns_worm_tracker_commons_object<ns_wcon_rich_data_record_element> obj;
			infile >> obj;

			obj.data.sort_by_time_and_id();
			if (object2 == obj)
				std::cout << "Test 6 Passed\n";
			else throw std::runtime_error("Failed test 6");
			infile.close();
		}
		//////////////////
		//TEST 7: Read the complex wcon object in split format
		//////////////////
		if (1) {
			std::vector<std::string> test_files =
			{ "intermediate.wcon",
				"maximal_0.wcon",
				"maximal_1.wcon",
				"maximal_2.wcon",
				"minimal.wcon",
				"minimax.wcon",
				"multiworm.wcon",
				"offset_and_centroid.wcon",
				"offset_no_centroid_yes.wcon",
				"offset_none.wcon",
				"offset_only.wcon",
				"perimeter_points.wcon" };
			for (int i = 0; i < test_files.size(); i++) {
				std::string filename = "..\\..\\..\\tests\\";
				filename += test_files[i];
				std::ifstream infile(filename);
				if (infile.fail()) {
					std::cerr << "Could not open " << filename << "\n";
					std::ofstream out(filename);
					out << "f";
					out.close();
					continue;
				}
				std::cout << "Parsing " << test_files[i] << "...\n";
				ns_worm_tracker_commons_object<ns_wcon_rich_data_record_element> obj;
				infile >> obj;
				std::cout << " Success.\n";
				infile.close();
			}

		}

	}
	catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		char a; a = std::cin.get();
		return 1;
	}
	return 0;
}

