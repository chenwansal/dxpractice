#pragma once
#include "ofbx.h"
#include <cstdio>

ofbx::IScene *load_fbx(const char *filepath);

bool saveAsOBJ(ofbx::IScene &scene, const char *path);