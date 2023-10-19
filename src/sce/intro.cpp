//
// Created by Rudolph on 2023/10/10.
//

#include <iostream>
#include <tinyxml2.h>
#include "intro.h"
#include "tetris.h"

Scene::Result Intro::tick(Action action) {
    Scene::tick(action);
    if (ticks >= 120) {
        return Result::next;
    } else {
        if (ticks % 5 == 0) {
            return Result::update;
        }

        return Result::noop;
    }
}

std::vector<Vertex> Intro::getVertices() {
    return {
            {{0.5, 0.5},  2, 1},
            {{0.6, 0.5},  2, 1},
            {{0.5, 0.55}, 2, 1},
            {{0.6, 0.55}, 2, 1},
            {{0.5, 0.55}, 2, 1},
            {{0.6, 0.5},  2, 1}
    };
}

Scene *Intro::nextScene() {
    return new Tetris{};
}

Intro::Intro() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* svg;
    tinyxml2::XMLElement* ref;
    tinyxml2::XMLElement* ass;
    tinyxml2::XMLError result;

    const char * invalid_file = "ass/intro.o.svg is not valid asset file";
    const char * group_id;

    std::cout << "LOADING SVG DOC " << std::endl;
    result = doc.LoadFile("ass/intro.o.svg");
    if (result != tinyxml2::XML_SUCCESS) {
        throw std::exception("Failed to load ass/intro.o.svg");
    }
    std::cout << "LOADED SVG DOC " << std::endl;

    // Log start

    // Structure of the XML file:
    // - Element "PLAY"      the root Element, which is the
    //                       FirstChildElement of the Document
    // - - Element "TITLE"   child of the root PLAY Element
    // - - - Text            child of the TITLE Element

    // Navigate to the title, using the convenience function,
    // with a dangerous lack of error checking.
    svg = doc.FirstChildElement("svg");
    if (svg == nullptr) {
        throw std::exception(invalid_file);
    }

    ref = svg->FirstChildElement();
    if (ref == nullptr) {
        throw std::exception(invalid_file);
    }
    do {
        ref->QueryStringAttribute("id", &group_id);
        ref = ref->NextSiblingElement();
    }
    while (strcmp(group_id, "REF") != 0 && ref != nullptr);
    if (strcmp(group_id, "REF") != 0) {
        throw std::exception(invalid_file);
    }

    ass = svg->FirstChildElement();
    if (ass == nullptr) {
        throw std::exception(invalid_file);
    }
    do {
        ass->QueryStringAttribute("id", &group_id);
        ass = ref->NextSiblingElement();
    }
    while (strcmp(group_id, "ASS") != 0 && ass != nullptr);
    if (strcmp(group_id, "ASS") != 0) {
        throw std::exception(invalid_file);
    }
}
