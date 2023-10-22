//
// Created by Rudolph on 2023/10/10.
//

#include <iostream>
#include <map>
#include <tinyxml2.h>
#include "intro.h"
#include "tetris.h"
#include "glm/glm.hpp"

Scene::Result Intro::tick(Action action) {
    Scene::tick(action);
    if (ticks >= 120) {
        return Result::noop;
    } else {
        if (ticks % 5 == 0) {
            return Result::update;
        }

        return Result::noop;
    }
}

std::vector<Vertex> Intro::getVertices() {
    return vertices;
}

Scene *Intro::nextScene() {
    return new Tetris{};
}

Intro::Intro() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* svg;
    tinyxml2::XMLElement* group;
    tinyxml2::XMLElement* ref;
    tinyxml2::XMLElement* ass;
    tinyxml2::XMLElement* path;
    tinyxml2::XMLError result;

    const char * id;
    const char * d;

    std::cout << "LOADING SVG DOC " << std::endl;
    result = doc.LoadFile("ass/intro.o.svg");
    if (result != tinyxml2::XML_SUCCESS) {
        throw std::exception("Failed to load ass/intro.o.svg");
    }
    std::cout << "LOADED SVG DOC " << std::endl;

    svg = doc.FirstChildElement("svg");
    if (svg == nullptr) {
        throw std::exception(INVALID_FILE);
    }

    group = svg->FirstChildElement();
    if (group == nullptr) {
        throw std::exception(INVALID_FILE);
    }
    do {
        group->QueryStringAttribute("id", &id);
        if (strcmp(id, "REF") == 0) {
            ref = group;
        } else if (strcmp(id, "ASS") == 0) {
            ass = group;
        }
        group = group->NextSiblingElement();
    }
    while (group != nullptr);

    if (ref == nullptr || ass == nullptr) {
        throw std::exception(INVALID_FILE);
    }

    std::vector<Vertex> temp;
    path = ref->FirstChildElement();
    do {
        path->QueryStringAttribute("id", &id);
        path->QueryStringAttribute("d", &d);

        vertices = parseSvg(d);
        break;
//        vertices.insert(vertices.end(), temp.begin(), temp.end());
        path = path->NextSiblingElement();
    }
    while (path != nullptr);


//    path3052    d: "m0 8 14 8v-16z"
//    path3058    d: "m0 0v16l14-8z"
//    path3084    d: "m0 0v16l14-8z"
//    path3092    d: "m0 8 14 8v-16z"
//    path3106    d: "m0 0v16l14-8z"
//    path3108    d: "m0 8 14 8v-16z"
//    path4966    d: "m0 8 14 8v-16z"
//    path17427   d: "m0 0v16l14-8z"


//    path = ass->FirstChildElement()->FirstChildElement();
//    do {
//        path->QueryStringAttribute("transform", &id);
//        path->QueryStringAttribute("xlink:href", &d);
//        std::cout << "Ref: " << id << " Transfrom: " << d << std::endl;
//
//        path = path->NextSiblingElement();
//    }
//    while (path != nullptr);

}

std::vector<Vertex> Intro::parseSvg(std::string path) {
    size_t iNonNumeric[6] = {0}, cNonNumeric = 0;
    for (size_t i = 0; i < path.length(); i++) {
        if (!isdigit(path[i]) && path[i] != '-') {
            iNonNumeric[cNonNumeric] = i;
            cNonNumeric++;
        }

        if ((cNonNumeric >= 6) || path[i] == 'z') {
            break;
        }
    }

    if (cNonNumeric != 6 && path[0] != 'm' && path[path.length()] != 'z') {
        throw std::exception(INVALID_FILE);
    }

    int x1 = 0, x2 = 0, x3= 0, y1= 0, y2= 0, y3 = 0;

    x1 = std::atoi(path.substr(iNonNumeric[0] + 1, iNonNumeric[1] - iNonNumeric[0] - 1).c_str());
    y1 = std::atoi(path.substr(iNonNumeric[1] + 1, iNonNumeric[2] - iNonNumeric[1] - 1).c_str());
    x2 = std::atoi(path.substr(iNonNumeric[2] + 1, iNonNumeric[3] - iNonNumeric[2] - 1).c_str());
    y2 = std::atoi(path.substr(iNonNumeric[3] + 1, iNonNumeric[4] - iNonNumeric[3] - 1).c_str());
    x3 = std::atoi(path.substr(iNonNumeric[4] + 1, iNonNumeric[5] - iNonNumeric[4] - 1).c_str());

    std::cout << "x1 " << x1 << " y1 " << y1 << " x2 " << x2 << " y2 " << y2 << " x3 " << x3 << " y3 " << y3 << std::endl;
    // x1 0 y1 8 x2 14 y2 8 x3 -16 y3 0


    return {
            {{(x1+x2)/400.0,(y2+y1)/400.0}, 5, 32},
            {{x1/400.0,y1/400.0}, 5, 32},
            {{(x1+x2)/400.0,(y2+y1+x3)/400.0}, 5, 32}
    };
}
