/* Created by Rudolph on 2023/10/26 */

#include <exception>
#include "svg.h"
#include <string>

Svg::Svg(const std::string& file_name): file_name(file_name) {
    tinyxml2::XMLError result;
    tinyxml2::XMLElement *svg;

    result = doc.LoadFile(file_name.c_str());
    if (result != tinyxml2::XML_SUCCESS) {
        throw std::exception((e_file_load + file_name).c_str());
    }

    svg = doc.FirstChildElement("svg");
    if (svg == nullptr) {
        throw std::exception((e_file_invalid + file_name).c_str());
    }

    if (svg->FirstChildElement() == nullptr) {
        throw std::exception((e_file_invalid + file_name).c_str());
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

        const std::vector<Vertex> &temp = this->parsePath(d);
        reference_vertexes[id] = temp;
        path = path->NextSiblingElement();
    } while (path != nullptr);

    return reference_vertexes;
}

std::vector<Vertex> Svg::parsePath(std::string path) {
    if (path[0] != 'm' || path[path.length() - 1] != 'z') {
        throw std::exception((e_file_invalid + file_name).c_str());
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
                {{p[0] / 400.0,          1 - p[1] / 800.0},                 5, 32},
                {{(p[0] + p[2]) / 400.0, 1 - (p[3] + p[1]) / 800.0},        5, 32},
                {{(p[0] + p[2]) / 400.0, 1 - (p[3] + p[1] + p[4]) / 800.0}, 5, 32}
        };
    } else {
        return {
                {{p[0] / 400.0,          1 - p[1] / 800.0},                 5, 32},
                {{p[0] / 400.0,          1 - (p[1] + p[2]) / 800.0},        5, 32},
                {{(p[0] + p[3]) / 400.0, 1 - (p[1] + p[2] + p[4]) / 800.0}, 5, 32}
        };
    };

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
        throw std::exception((e_file_invalid + file_name).c_str());
    }
    source = source->FirstChildElement();
    if (source == nullptr) {
        throw std::exception((e_file_invalid + file_name).c_str());
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
        throw std::exception((e_file_invalid + file_name + " failed to find " + needle + " child").c_str());
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
        dspVertexes.push_back({{vertex.pos.x + x / 400.0, vertex.pos.y - y / 800.0}, vertex.type, vertex.age});
    }
    return dspVertexes;
}
