#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "tinyxml2.h"
#include "types.h"

using namespace std;
using namespace tinyxml2;

vector<string> splitter(string &str, char delimiter) {
    stringstream ss(str);
    string item;
    vector<string> splittedStrs;

    while (getline(ss, item, delimiter)) {
        splittedStrs.push_back(item);
    }

    return splittedStrs;
}

const char *getValueOrDefault(const char *r, int is_color) {
    if (r != nullptr) {
        return r;
    } else if (is_color == 1) return "1";
    return "0";
}

const char *getOptionalAttribute(XMLElement *e, const char *attribute, int is_color) {
    return getValueOrDefault(e->Attribute(attribute), is_color);
}

float getFloatAttribute(XMLElement *e, const char *attribute, int is_color) {
    return strtof(getOptionalAttribute(e, attribute, is_color), nullptr);
}

Action readTranslate(XMLElement *e) {
    auto action = new struct action();
    action->name = "translate";
    action->translate = new struct point();

    action->translate->x = getFloatAttribute(e, "X", 0);
    action->translate->y = getFloatAttribute(e, "Y", 0);
    action->translate->z = getFloatAttribute(e, "Z", 0);

    return action;
}

Action readRotate(XMLElement *e) {
    auto action = new struct action();
    action->name = "rotate";
    action->rotate = new struct rotate();
    action->rotate->point = new struct point();

    action->rotate->time = getFloatAttribute(e, "time", 0);
    action->rotate->point->x = getFloatAttribute(e, "X", 0);
    action->rotate->point->y = getFloatAttribute(e, "Y", 0);
    action->rotate->point->z = getFloatAttribute(e, "Z", 0);

    return action;
}

Action readMove(XMLElement *e) {
    auto action = new struct action();
    action->name = "move";
    action->movement = new struct move();

    float axis[3] = {0, 1, 0};
    action->movement->axis = axis;
    action->movement->time = getFloatAttribute(e, "time", 0);

    for (XMLNode *g = e->FirstChild(); g != nullptr; g = g->NextSibling()) {
        auto point = new struct point();

        point->x = getFloatAttribute(g->ToElement(), "X", 0);
        point->y = getFloatAttribute(g->ToElement(), "Y", 0);
        point->z = getFloatAttribute(g->ToElement(), "Z", 0);
        action->movement->points.push_back(point);
    }
    return action;
}

Action readScale(XMLElement *e) {
    auto action = new struct action();
    action->name = "scale";
    action->scale = new struct point();

    action->scale->x = getFloatAttribute(e, "X", 0);
    action->scale->y = getFloatAttribute(e, "Y", 0);
    action->scale->z = getFloatAttribute(e, "Z", 0);

    return action;
}

void readColor(XMLElement *e, Action action) {
    action->model->color = new struct color();

    action->model->color->red = getFloatAttribute(e, "R", 1);
    action->model->color->green = getFloatAttribute(e, "G", 1);
    action->model->color->blue = getFloatAttribute(e, "B", 1);
}

void readFile(XMLElement *e, Action action) {
    ifstream file(e->Attribute("file"));
    if (file.is_open()) {
        string line;
        getline(file, line);
        int size = 0;
        while (getline(file, line)) {
            vector<string> s = splitter(line, ' ');
            auto point = new struct point();
            point->x = stof(s.at(0));
            point->y = stof(s.at(1));
            point->z = stof(s.at(2));
            size++;
            action->model->points.push_back(point);
        }
    }
    file.close();
}

Action readModel(XMLElement *e) {
    auto action = new struct action();
    action->name = "model";
    action->model = new struct model();

    readFile(e, action);
    readColor(e, action);

    return action;
}

void readModels(XMLElement *models, vector<Action> *actions) {
    XMLElement *model = models->FirstChildElement("model");
    while (model != nullptr) {
        actions->push_back(readModel(model));
        model = model->NextSiblingElement("model");
    }
}

Group readGroups(XMLNode *node) {
    auto group = new struct group();

    for (XMLNode *g = node->FirstChild(); g != nullptr; g = g->NextSibling()) {
        const char *name = g->Value();
        if (!strcmp(name, "models")) {
            auto *e = (XMLElement *) g;
            readModels(e, &group->actions);
        } else if (!strcmp(name, "translate")) {
            auto *e = (XMLElement *) g;
            if (getFloatAttribute(e, "time", 0) != 0) {
                group->actions.push_back(readMove(e));
            } else {
                group->actions.push_back(readTranslate(e));
            }
        } else if (!strcmp(name, "rotate")) {
            auto *e = (XMLElement *) g;
            group->actions.push_back(readRotate(e));
        } else if (!strcmp(name, "scale")) {
            auto *e = (XMLElement *) g;
            group->actions.push_back(readScale(e));
        } else if (!strcmp(name, "group")) {
            Group subgroup = readGroups(g);
            auto action = new struct action();
            action->name = "group";
            action->group = subgroup;
            group->actions.push_back(action);
        }
    }

    return group;
}

Config readConfig(const char *filename) {
    Config config;

    XMLDocument document;
    XMLError result = document.LoadFile(filename);
    if (result != XML_SUCCESS) return nullptr;
    XMLNode *root = document.FirstChildElement("scene");
    if (root == nullptr) return nullptr;
    XMLElement *element = root->FirstChildElement("group");

    config = new struct config();

    while (element != nullptr) {
        Group group = readGroups(element);
        config->groups.push_back(group);
        element = (XMLElement *) element->NextSibling();
    }

    return config;
}
