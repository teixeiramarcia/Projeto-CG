#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <il.h>

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
        int num_points = 0;
        getline(file, line);
        num_points = stoi(line);
        int size = 0;
        while (size < num_points && getline(file, line)) {
            vector<string> s = splitter(line, ' ');
            auto point = new struct point();
            point->x = stof(s.at(0));
            point->y = stof(s.at(1));
            point->z = stof(s.at(2));
            action->model->points.push_back(point);
            size++;
        }
        size = 0;
        int num_normals;
        if (getline(file, line)) {
            num_normals = stoi(line);
            if (num_normals != 0) {
                while (size < num_normals && getline(file, line)) {
                    vector<string> s = splitter(line, ' ');
                    auto point = new struct point();
                    point->x = stof(s.at(0));
                    point->y = stof(s.at(1));
                    point->z = stof(s.at(2));
                    action->model->normals.push_back(point);
                    size++;
                }
            } else {
                action->model->normals = {};
            }
            size = 0;
            int num_textures;
            getline(file, line);
            num_textures = stoi(line);
            if (num_textures != 0) {
                while (size < num_textures && getline(file, line)) {
                    vector<string> s = splitter(line, ' ');
                    auto texture_point = new struct texture_point();
                    texture_point->x = stof(s.at(0));
                    texture_point->y = stof(s.at(1));
                    action->model->texture_points.push_back(texture_point);
                    size++;
                }
            } else {
                action->model->texture_points = {};
            }
        }
    }
    file.close();
}

GLuint loadTexture(const string &s) {
    unsigned int t, tw, th;
    unsigned char *texData;
    unsigned int texID;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) s.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

Action readModel(XMLElement *e) {
    auto action = new struct action();
    action->name = "model";
    action->model = new struct model();
    if (e->Attribute("texture") != nullptr) {
        action->model->texture = loadTexture(e->Attribute("texture"));
        action->model->emissiveR = getFloatAttribute(e, "emR", 0);
        action->model->emissiveG = getFloatAttribute(e, "emG", 0);
        action->model->emissiveB = getFloatAttribute(e, "emB", 0);
    } else {
        readColor(e, action);
    }

    readFile(e, action);

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

Light readLight(XMLElement *element) {
    auto light = new struct light();
    light->type = element->Attribute("type");
    light->posX = stof(element->Attribute("posX"));
    light->posY = stof(element->Attribute("posY"));
    light->posY = stof(element->Attribute("posZ"));
    return light;
}

void readLights(XMLElement *element, Config config) {
    element = element->FirstChildElement("light");
    while (element != nullptr) {
        Light light = readLight(element);
        config->lights.push_back(light);
        element = (XMLElement *) element->NextSibling();
    }
}

void readGroup(XMLElement *element, Config config) {
    while (element != nullptr) {
        Group group = readGroups(element);
        config->groups.push_back(group);
        element = (XMLElement *) element->NextSibling();
    }
}

Config readConfig(const char *filename) {
    Config config;

    XMLDocument document;
    XMLError result = document.LoadFile(filename);
    if (result != XML_SUCCESS) return nullptr;
    XMLNode *root = document.FirstChildElement("scene");
    if (root == nullptr) return nullptr;

    config = new struct config();

    if (root->FirstChildElement("lights")) {
        readLights(root->FirstChildElement("lights"), config);
    }

    if (root->FirstChildElement("group")) {
        readGroup(root->FirstChildElement("group"), config);
    }
    return config;
}
