/* Created by Rudolph on 2023/10/26 */

#include <exception>
#include "svg.h"
#include <string>

Svg::Svg(const std::string& file_name): file_name(file_name) {
    tinyxml2::XMLError result;
    tinyxml2::XMLElement *svg;

    result = doc.LoadFile(file_name.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        throw std::exception();
    }

    svg = doc.FirstChildElement("svg");
    if (svg == nullptr) {
        throw std::exception();
    }

    if (svg->FirstChildElement() == nullptr) {
        throw std::exception();
    }
}

std::unordered_map<std::string, std::vector<Vertex>> Svg::getReferenceVertexes() {
    const char *id;
    const char *d;
    const char *style;
    std::unordered_map<std::string, std::vector<Vertex>> reference_vertexes;

    tinyxml2::XMLElement *source;
    tinyxml2::XMLElement *path;
    source = doc.FirstChildElement("svg");
    source = getNodeWithId("REF", source);

    path = source->FirstChildElement();
    do {
        path->QueryStringAttribute("id", &id);
        path->QueryStringAttribute("d", &d);
        path->QueryStringAttribute("style", &style);

        glm::vec3 colour = this->parseStyle(style);
        reference_vertexes[id] = this->parsePath(d, colour);

        path = path->NextSiblingElement();
    } while (path != nullptr);

    return reference_vertexes;
}

std::vector<Vertex> Svg::parsePath(std::string path, glm::vec3 colour) {
    if (path[0] != 'm' || path[path.length() - 1] != 'z') {
        throw std::exception();
    }

    int p[5] = {0, 0, 0, 0, 0};
    size_t in = 0, li = 0;
    std::string path_methods;
    // m0 0v16l14-8z
    // m0 8 14 8v-16z
    for (size_t i = 0; i < path.length(); i++) {
        if (isdigit(path[i])) {

        } else {
            if (li != i && i > li + 1) {
                if (path[li] == '-') {
                    p[in] = std::strtol(path.substr(li, i - li).c_str(), nullptr, 10);
                } else {
                    p[in] = std::strtol(path.substr(li + 1, i - li - 1).c_str(), nullptr, 10);
                }
                in++;
            }

            if (path[i] != '-' && path[i] != ' ') {
                path_methods += path[i];
            }
            li = i;
        }
    }

    if (std::strcmp(path_methods.c_str(), "mvz") == 0) {
        return {
                {{p[0] / 400.0,          1 - p[1] / 800.0},                 colour},
                {{(p[0] + p[2]) / 400.0, 1 - (p[3] + p[1]) / 800.0},        colour},
                {{(p[0] + p[2]) / 400.0, 1 - (p[3] + p[1] + p[4]) / 800.0}, colour}
        };
    } else {
        return {
                {{p[0] / 400.0,          1 - p[1] / 800.0},                 colour},
                {{p[0] / 400.0,          1 - (p[1] + p[2]) / 800.0},        colour},
                {{(p[0] + p[3]) / 400.0, 1 - (p[1] + p[2] + p[4]) / 800.0}, colour}
        };
    };

}

glm::vec3 Svg::parseStyle(const std::string& style) {
    // Extract the fill from "fill:#0663ac;paint-order:markers stroke fill"
    size_t i = style.find("fill:");
    size_t j = style.find(';', i);
    std::string fill = style.substr(i + 5, j - i - 5);

    // Convert the hex colour to rgb
    int r = std::strtol(fill.substr(1, 2).c_str(), nullptr, 16);
    int g = std::strtol(fill.substr(3, 2).c_str(), nullptr, 16);
    int b = std::strtol(fill.substr(5, 2).c_str(), nullptr, 16);
    return {r / 255.0, g / 255.0, b / 255.0};
}

std::vector<Vertex> Svg::populateDisplayVertexes(std::unordered_map<std::string, std::vector<Vertex>> &refVertexes) {
    tinyxml2::XMLElement *source;
    tinyxml2::XMLElement *assVertex;
    const char *transform;
    const char *refVertexId;
    std::vector<Vertex> dspVertexes;
    std::vector<Vertex> tfVertexes;

    source = doc.FirstChildElement("svg");
    source = getNodeWithId("ASS", source);
    source = getNodeWithId("INTRO", source);

    assVertex = source->FirstChildElement();
    do {
        assVertex->QueryStringAttribute("xlink:href", &refVertexId);
        assVertex->QueryStringAttribute("transform", &transform);

        //remove # from start of refVertexId = "#path3052"
        std::string temp(refVertexId);
        temp = temp.substr(1);

        assVertex = assVertex->NextSiblingElement();
        if (refVertexes.contains(temp)) {
            tfVertexes = calcTransformedVertex(refVertexes[temp], transform);
            dspVertexes.insert(dspVertexes.end(), tfVertexes.begin(), tfVertexes.end());
        }
    } while (assVertex != nullptr);

    return dspVertexes;
}

tinyxml2::XMLElement* Svg::getNodeWithId(const char *needle, tinyxml2::XMLElement *source) const {
    if (source == nullptr) {
        throw std::exception();
    }
    source = source->FirstChildElement();
    if (source == nullptr) {
        throw std::exception();
    }

    const char *id;
    do {
        source->QueryStringAttribute("id", &id);
        if (strcmp(id, needle) == 0) {
            break;
        }
        source = source->NextSiblingElement();
    } while (source != nullptr);

    if (source == nullptr) {
        throw std::exception();
    }
    return source;
}

std::vector<Vertex> Svg::calcTransformedVertex(std::vector<Vertex> &vector, const std::string& transform) {
    int32_t x, y;

    // Parse the x y value from "translate(x,y)"
    size_t i = transform.find('(');
    size_t j = transform.find(',');
    size_t k = transform.find(')');
    x = std::strtol(transform.substr(i + 1, j - i - 1).c_str(), nullptr, 10);
    y = std::strtol(transform.substr(j + 1, k - j - 1).c_str(), nullptr, 10);

    std::vector<Vertex> dspVertexes;
    for (auto &vertex : vector) {
        dspVertexes.push_back({{vertex.pos.x + x / 400.0, vertex.pos.y - y / 800.0}, vertex.col});
    }
    return dspVertexes;
}
