// Copyright (c) 2014 Bauhaus-Universitaet Weimar
// This Software is distributed under the Modified BSD License, see license.txt.
//
// Virtual Reality and Visualization Research Group 
// Faculty of Media, Bauhaus-Universitaet Weimar
// http://www.uni-weimar.de/medien/vr

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <lamure/ren/model_database.h>
#include <lamure/bounding_box.h>

#include <lamure/types.h>
#include <lamure/ren/dataset.h>
#include <lamure/ren/bvh.h>
#include <lamure/ren/lod_stream.h>

#define VERBOSE
#define DEFAULT_PRECISION 15

char* get_cmd_option(char** begin, char** end, const std::string & option) {
    char** it = std::find(begin, end, option);
    if (it != end && ++it != end)
        return *it;
    return 0;
}

bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

struct xyzall_surfel_t {
  float x_, y_, z_;
  uint8_t r_, g_, b_, fake_;
  float size_;
  float nx_, ny_, nz_;
};

int main(int argc, char *argv[]) {
    
    if (argc == 1 || 
      cmd_option_exists(argv, argv+argc, "-h") ||
      !cmd_option_exists(argv, argv+argc, "-f")) {
        
      std::cout << "Usage: " << argv[0] << "<flags> -f <input_file>" << std::endl <<
         "INFO: " << argv[0] << std::endl <<
         "\t-f: selects .bvh input file" << std::endl <<
         "\t    (-f flag is required) " << std::endl <<
         std::endl;
      return 0;
    }

    std::string bvh_filename = std::string(get_cmd_option(argv, argv + argc, "-f"));

    std::string ext = bvh_filename.substr(bvh_filename.size()-3);
    if (ext.compare("bvh") != 0) {
        std::cout << "please specify a .bvh file as input" << std::endl;
        return 0;
    }

    
    std::string y_file = bvh_filename.substr(0, bvh_filename.size()-3)+"y";
    std::cout << "input: " << bvh_filename << std::endl;
   
    lamure::ren::bvh* bvh = new lamure::ren::bvh(bvh_filename);
    
    if(bvh->get_num_nodes() > std::numeric_limits<uint32_t>::max()) {
      std::cerr << " I'm sorry, Dave. I'm afraid I can't do that." << std::endl;
      std::cerr << "Javascript can only natively handle uint32 for num_nodes but this dataset uses more bytes" << std::endl;
      return 0;
    }

    std::ofstream out_stream;
    out_stream.open(y_file, std::ios::out | std::ios::binary);
    std::cout << "output: " << y_file << std::endl;
    

    const uint32_t depth = bvh->get_depth();
    out_stream.write((char*) &depth, 4);

    const uint32_t prims_per_node = bvh->get_primitives_per_node();
    out_stream.write((char*) &prims_per_node, 4);

    const uint32_t fan_factor = bvh->get_fan_factor();
    out_stream.write((char*) &fan_factor, 4);

    const uint32_t num_nodes = bvh->get_num_nodes();
    out_stream.write((char*) &num_nodes, 4);

    const scm::math::vec3f translation = bvh->get_translation();
    out_stream.write((char*) &translation, 12);
    
    //bvh->get_size_of_primitive() ALWAYS 32 bytes!

    for (uint32_t node_id = 0; node_id < bvh->get_num_nodes(); ++node_id) {
      const scm::math::vec3f bb_min = bvh->get_bounding_box(node_id).min_vertex();
      out_stream.write((char*) &bb_min, 12);

      const scm::math::vec3f bb_max = bvh->get_bounding_box(node_id).max_vertex();
      out_stream.write((char*) &bb_max, 12);

      const scm::math::vec3f centroid = bvh->get_centroid(node_id);
      out_stream.write((char*) &centroid, 12);

      const float representative_radius = bvh->get_avg_primitive_extent(node_id);
      out_stream.write((char*) &representative_radius, 4);
    }

    out_stream.close();
    std::cout << "done." << std::endl;

    delete bvh;


    return 0;
}



