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
    tinyxml2::XMLElement *svg;
    tinyxml2::XMLElement *group;
    tinyxml2::XMLElement *ref;
    tinyxml2::XMLElement *ass;
    tinyxml2::XMLElement *path;
    tinyxml2::XMLError result;

    const char *id;
    const char *d;

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
    } while (group != nullptr);

    if (ref == nullptr || ass == nullptr) {
        throw std::exception(INVALID_FILE);
    }

    std::vector<Vertex> temp;
    path = ref->FirstChildElement();
    do {
        path->QueryStringAttribute("id", &id);
        path->QueryStringAttribute("d", &d);

        temp = parseSvg(d);
        vertices.insert(vertices.end(), temp.begin(), temp.end());
        path = path->NextSiblingElement();
    } while (path != nullptr);


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
    if (path[0] != 'm' && path[path.length()] != 'z') {
        throw std::exception(INVALID_FILE);
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
                    p[in] = std::atoi(path.substr(li, i - li).c_str());
                } else {
                    p[in] = std::atoi(path.substr(li + 1, i - li - 1).c_str());
                }
                in++;
            }

            if (path[i] != '-' && path[i] != ' ') {
                path_methods += path[i];
            }
            li = i;
        }
    }

    ;

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
