/* Created by Rudolph on 2023/10/26 */

#ifndef VULKAN_TETRIS_SVG_H
#define VULKAN_TETRIS_SVG_H

#include <string>
#include "tinyxml2.h"
#include "../structs.h"

class Svg {
public:
    explicit Svg(const std::string& file_name);
    std::unordered_map<std::string, std::vector<Vertex>> getReferenceVertexes();

    std::vector<Vertex> populateDisplayVertexes(std::unordered_map<std::string, std::vector<Vertex>> &refVertexes);

private:
    tinyxml2::XMLDocument doc;
    std::string file_name;



    const std::string e_file_load = "Failed to load file: ";
    const std::string e_file_invalid = "Invalid file: ";

    std::vector<Vertex> parsePath(std::string path);

    tinyxml2::XMLElement* getNodeWithId(const char *needle, tinyxml2::XMLElement *source) const;

    static std::vector<Vertex> calcTransformedVertex(std::vector<Vertex> &vector, const std::string& transform);
};


#endif //VULKAN_TETRIS_SVG_H
